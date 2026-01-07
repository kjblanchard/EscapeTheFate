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

using namespace Etf;
using namespace std;

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
	return new UIObject(objectArgs);
}
static UIImage* createImage(const string& name, const glz::generic& data) {
	UIImageArgs args;
	getObjectFromField(args.Rect, "rect", data);
	getObjectFromField(args.SourceRect, "srcRect", data);
	getObjectFromField(args.DrawColor, "color", data);
	args.Filename = data.at("file").get_string();
	args.Scale = data.at("scale").get_number();
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
	return new UINineSlice(args);
}

static UIObject* handleTypeCreation(const string& name, const string& type, const glz::generic& data) {
	if (type == "text") {
		return createText(data);
	} else if (type == "nineSlice") {
		return createNineSliceObject(name, data);
	} else if (type == "image") {
		return createImage(name, data);
	} else {
		return createUIObject(name, data);
	}
	return nullptr;
}

static UIObject* handleUIArgs(const string& name, const glz::generic& data) {
	string spawnType = "";
	if (data.contains("type")) {
		spawnType = data.at("type").get_string();
	}
	sgLogDebug("Should create UI of type %s", spawnType.c_str());
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

void UI::LoadUIFromFile(std::string filename) {
	if (!RootUIObject) {
		RootUIObject = make_unique<UIObject>();
	}
	glz::generic fullUIFileGeneric;
	auto fullFileErrorContext = glz::read_file_json(fullUIFileGeneric, filename, std::string{});
	if (fullFileErrorContext) {
		string buffer;
		sgLogError("Error %d  parsing UI file %s, %s", fullFileErrorContext.ec, filename.c_str(), glz::format_error(fullFileErrorContext, buffer).c_str());
		return;
	}
	for (auto& [panelName, panelObj] : fullUIFileGeneric.get_object()) {
		if (panelObj.contains("children")) {
			auto topPanel = handleUIArgs(panelName, panelObj);
			if (!topPanel) continue;
			RootUIObject->AddChild(topPanel);
		}
	}
}
void UI::DrawUI() {
	// auto text = new UIText(textArgs);
	// image->_debugBox = true;
	// niner->_debugBox = true;
	// text->_debugBox = true;
	// RootUIObject->AddChild(image);
	// RootUIObject->AddChild(niner);
	// niner->AddChild(text);
	RootUIObject->Draw(0, 0);
}

void UI::DestroyUI() {
	RootUIObject.reset();
}
