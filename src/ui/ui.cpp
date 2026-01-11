#include <Supergoon/Primitives/rectangle.h>
#include <Supergoon/log.h>

#include <glaze/core/context.hpp>
#include <glaze/core/reflect.hpp>
#include <glaze/json/generic.hpp>
#include <ui/ui.hpp>
#include <ui/uiImage.hpp>
#include <ui/uiNineSlice.hpp>
#include <ui/uiObject.hpp>
#include <ui/uiText.hpp>
#include <unordered_map>

using namespace Etf;
using namespace std;

unordered_map<string, glz::generic> _cachedUIFiles;

template <typename T>
static void getObjectFromField(T& rect, const string& field, const glz::generic& data) {
	auto error = glz::read_json(rect, data.at(field).get_object());
	if (error) {
		string buffer;
		sgLogError("Error parsing rect:%d, %s", error.ec, glz::format_error(error, buffer).c_str());
	}
}

static UIText* createText(const glz::generic& data) {
	UITextArgs args;
	args.FontName = data.at("font").get_string();
	args.FontSize = data.at("fontSize").get_number();
	getObjectFromField(args.Rect, "rect", data);
	args.TextToDraw = data.at("text").get_string();
	args.CenteredX = data.at("centeredX").get_boolean();
	args.CenteredY = data.at("centeredY").get_boolean();
	args.WordWrap = data.at("wordWrap").get_boolean();
	args.NumCharsToDraw = data.at("numCharsToDraw").get_number();
	args.DebugBox = data.contains("debug") ? data.at("debug").get_boolean() : false;
	return new UIText(args);
}

static UIObject* createUIObject(const std::string& name, const glz::generic& data) {
	UIObjectArgs objectArgs;
	objectArgs.Rect = {0, 0, 0, 0};
	if (data.contains("rect")) {
		getObjectFromField(objectArgs.Rect, "rect", data);
	}
	objectArgs.Name = name;
	objectArgs.Priority = data.contains("priority") ? data.at("priority").get_number() : 0;
	objectArgs.Visible = data.contains("visible") ? data.at("priority").get_boolean() : true;
	objectArgs.DebugBox = data.contains("debug") ? data.at("debug").get_boolean() : false;
	objectArgs.DoNotDestroy = data.contains("doNotDestroy") ? data.at("doNotDestroy").get_boolean() : false;
	return new UIObject(objectArgs);
}
static UIImage* createImage(const string& name, const glz::generic& data) {
	UIImageArgs args;
	getObjectFromField(args.Rect, "rect", data);
	getObjectFromField(args.SourceRect, "srcRect", data);
	getObjectFromField(args.DrawColor, "color", data);
	args.Filename = data.at("file").get_string();
	args.Scale = data.at("scale").get_number();
	args.DebugBox = data.contains("debug") ? data.at("debug").get_boolean() : false;
	args.Visible = data.contains("visible") ? data.at("visible").get_boolean() : true;
	return new UIImage(args);
}

static UINineSlice* createNineSliceObject(const string& name, const glz::generic& data) {
	UINineSliceArgs args;
	getObjectFromField(args.Rect, "rect", data);
	getObjectFromField(args.SourceRect, "srcRect", data);
	getObjectFromField(args.DrawColor, "color", data);
	args.Filename = data.at("file").get_string();
	args.Scale = data.at("scale").get_number();
	args.Xoffset = data.at("xOffset").get_number();
	args.Yoffset = data.at("yOffset").get_number();
	args.DebugBox = data.contains("debug") ? data.at("debug").get_boolean() : false;
	return new UINineSlice(args);
}

static UIObject* handleTypeCreation(const string& name, const string& type, const glz::generic& data) {
	sgLogDebug("Creating UI object of type %s", type.empty() ? "panel" : type.c_str());
	if (type == "text") {
		return createText(data);
	} else if (type == "nineSlice") {
		return createNineSliceObject(name, data);
	} else if (type == "image") {
		return createImage(name, data);
	} else {
		return createUIObject(name, data);
	}
	sgLogWarn("Could not create ui object %s, did not match anything", name.c_str());
	return nullptr;
}

static UIObject* handleUIArgs(const string& name, const glz::generic& data) {
	string spawnType = "";
	if (data.contains("type")) {
		spawnType = data.at("type").get_string();
	}

	auto newGuy = handleTypeCreation(name, spawnType, data);
	if (!newGuy) {
		sgLogError("Could not load ui object properly for %s", name.c_str());
		return newGuy;
	}
	if (data.contains("children")) {
		auto& children = data.at("children").get_object();
		for (auto& [childName, childObj] : children) {
			auto newChild = handleUIArgs(childName, childObj);
			if (!newChild) continue;
			newGuy->AddChild(newChild);
		}
	}
	return newGuy;
}

std::unique_ptr<UIObject> UI::RootUIObject;

static bool loadJsonFromFile(const string& filename) {
	sgLogDebug("Loading file %s to be cached", filename.c_str());
	auto& entry = _cachedUIFiles[filename];
	auto fullFileErrorContext = glz::read_file_jsonc(entry, filename, std::string{});
	if (fullFileErrorContext) {
		string buffer;
		sgLogError("Error %d  parsing UI file %s, %s", fullFileErrorContext.ec, filename.c_str(), glz::format_error(fullFileErrorContext, buffer).c_str());
		_cachedUIFiles.erase(filename);
		return false;
	}
	return true;
}

void UI::destroyOldUIPanelsIfNeeded(const std::string& newFile) {
}

void UI::LoadUIFromFile(const string& filename) {
	// Initialize the root object if needed
	if (!RootUIObject) RootUIObject = make_unique<UIObject>();
	glz::generic* fullUIFileGeneric;
	// Check the cached files to avoid filesystem reads, and return if there is an error loading
	if (!_cachedUIFiles.contains(filename)) {
		if (!loadJsonFromFile(filename)) return;
	}
	fullUIFileGeneric = &_cachedUIFiles[filename];
	// Store list so we can remove everything else afterwards
	vector<string> newPanelNames;
	// Loop through the json object and load each panel in it.
	for (auto& [panelName, panelObj] : fullUIFileGeneric->get_object()) {
		newPanelNames.push_back(panelName);
		if (RootUIObject->HasChildOfName(panelName)) {
			sgLogDebug("Not creating panel, it already exists %s", panelName.c_str());
			continue;
		}
		auto topPanel = handleUIArgs(panelName, panelObj);
		if (!topPanel) continue;
		RootUIObject->AddChild(topPanel);
	}
	RootUIObject->DestroyChildIfNotName(newPanelNames);
}
void UI::DrawUI() {
	RootUIObject->Draw(0, 0);
}

void UI::DestroyUI() {
	_cachedUIFiles.clear();
	RootUIObject.reset();
}
