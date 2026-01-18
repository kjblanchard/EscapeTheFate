#include <Supergoon/Primitives/rectangle.h>
#include <Supergoon/json.h>
#include <Supergoon/log.h>

#include <bindings/engine.hpp>
#include <ui/ui.hpp>
#include <ui/uiImage.hpp>
#include <ui/uiNineSlice.hpp>
#include <ui/uiObject.hpp>
#include <ui/uiText.hpp>
#include <ui/uiVLG.hpp>
#include <unordered_map>

using namespace Etf;
using namespace std;

unordered_map<string, json_object*> _cachedUIFiles;

static Color getColorFromField(json_object* obj, const char* key) {
	auto rectJson = jobj(obj, key);
	if (!rectJson) return {0, 0, 0, 0};
	return {
		(uint8_t)jint(rectJson, "R"),
		(uint8_t)jint(rectJson, "G"),
		(uint8_t)jint(rectJson, "B"),
		(uint8_t)jint(rectJson, "A"),
	};
}

static UIText* createText(const string& name, json_object* data) {
	UITextArgs args;
	args.FontName = jstr(data, "font");
	args.FontSize = jint(data, "fontSize");
	args.Rect = Engine::Json::GetRectFromObject(data, "rect");
	args.TextToDraw = jstr(data, "text");
	args.Priority = jint(data, "priority");
	args.CenteredX = jbool(data, "centeredX");
	args.CenteredY = jbool(data, "centeredY");
	args.WordWrap = jbool(data, "wordWrap");
	args.NumCharsToDraw = jint(data, "numCharsToDraw");
	args.DebugBox = jbool(data, "debug");
	args.Name = name;
	return new UIText(args);
}

static UIObject* createUIObject(const std::string& name, json_object* data) {
	UIObjectArgs objectArgs;
	objectArgs.Rect = Engine::Json::GetRectFromObject(data, "rect");
	objectArgs.Name = name;
	objectArgs.Priority = jint(data, "priority");
	objectArgs.Visible = jKeyExists(data, "visible") ? jbool(data, "visible") : true;
	objectArgs.DebugBox = jbool(data, "debug");
	objectArgs.DoNotDestroy = jbool(data, "doNotDestroy");
	return new UIObject(objectArgs);
}

static UIImage* createImage(const string& name, json_object* data) {
	UIImageArgs args;
	args.Rect = Engine::Json::GetRectFromObject(data, "rect");
	args.SourceRect = Engine::Json::GetRectFromObject(data, "srcRect");
	args.DrawColor = getColorFromField(data, "color");
	args.Priority = jint(data, "priority");
	args.Filename = jstr(data, "file");
	args.Scale = jfloat(data, "scale");
	args.DebugBox = jbool(data, "debug");
	args.Visible = jbool(data, "visible");
	args.Name = name;
	return new UIImage(args);
}

static UINineSlice* createNineSliceObject(const string& name, json_object* data) {
	UINineSliceArgs args;
	args.Rect = Engine::Json::GetRectFromObject(data, "rect");
	args.SourceRect = Engine::Json::GetRectFromObject(data, "srcRect");
	args.DrawColor = getColorFromField(data, "color");
	args.Priority = jint(data, "priority");
	args.Filename = jstr(data, "file");
	args.Scale = jint(data, "scale");
	args.Xoffset = jint(data, "xOffset");
	args.Yoffset = jint(data, "xOffset");
	args.DebugBox = jbool(data, "debug");
	args.Name = name;
	return new UINineSlice(args);
}

static UIVLG* createVLGObject(const string& name, json_object* data) {
	UIVLGArgs args;
	args.Rect = Engine::Json::GetRectFromObject(data, "rect");
	args.Priority = jint(data, "priority");
	args.DebugBox = jbool(data, "debug");
	args.Visible = jbool(data, "visible");
	args.Spacing = jint(data, "spacing");
	args.Name = name;
	return new UIVLG(args);
}

static UIObject* handleTypeCreation(const string& name, const string& type, json_object* data) {
	sgLogDebug("Creating UI object of type %s", type.empty() ? "panel" : type.c_str());
	if (type == "text") {
		return createText(name, data);
	} else if (type == "nineSlice") {
		return createNineSliceObject(name, data);
	} else if (type == "image") {
		return createImage(name, data);
	} else if (type == "vlg") {
		return createVLGObject(name, data);
	} else {
		return createUIObject(name, data);
	}
	sgLogWarn("Could not create ui object %s, did not match anything", name.c_str());
	return nullptr;
}

static UIObject* handleUIArgs(const string& name, json_object* data) {
	string spawnType = "";
	const char* trySpawnType = jstr(data, "type");
	if (trySpawnType) spawnType = trySpawnType;
	auto newGuy = handleTypeCreation(name, spawnType, data);
	if (!newGuy) {
		sgLogError("Could not load ui object properly for %s", name.c_str());
		return newGuy;
	}
	auto children = jobj(data, "children");
	if (!children) return newGuy;
	Engine::Json::jforeach_lambda(children, [&](const char* key, void* value) {
		json_object* child = (json_object*)value;
		auto newChild = handleUIArgs(key, child);
		if (!newChild) return;
		newGuy->AddChild(newChild);
	});
	return newGuy;
}

std::unique_ptr<UIObject> UI::RootUIObject;

static bool loadJsonFromFile(const string& filename) {
	sgLogDebug("Loading file %s to be cached", filename.c_str());
	auto obj = jGetObjectFromFile(filename.c_str());
	if (!obj) {
		sgLogError("Error parsing UI file %s ", filename.c_str());
		return false;
	}
	_cachedUIFiles[filename] = obj;
	return true;
}

void UI::destroyOldUIPanelsIfNeeded(const std::string& newFile) {
}

void UI::LoadUIFromFile(const string& filename) {
	// Initialize the root object if needed
	if (!RootUIObject) RootUIObject = make_unique<UIObject>();
	// Check the cached files to avoid filesystem reads, and return if there is an error loading
	if (!_cachedUIFiles.contains(filename)) {
		if (!loadJsonFromFile(filename)) return;
	}
	auto fullUIFileGeneric = _cachedUIFiles[filename];
	// Store list so we can remove everything else afterwards
	vector<string> newPanelNames;
	Engine::Json::jforeach_lambda(fullUIFileGeneric, [&](const char* key, void* value) {
		json_object* child = (json_object*)value;
		newPanelNames.push_back(key);
		if (RootUIObject->HasChildOfName(key)) {
			sgLogDebug("Not creating panel, it already exists %s", key);
			return;
		}
		auto topPanel = handleUIArgs(key, child);
		if (!topPanel) return;
		RootUIObject->AddChild(topPanel);
	});
	RootUIObject->DestroyChildIfNotName(newPanelNames);
}
void UI::DrawUI() {
	if (!RootUIObject) return;
	RootUIObject->Draw(0, 0);
}

void UI::DestroyUI() {
	_cachedUIFiles.clear();
	RootUIObject.reset();
}
