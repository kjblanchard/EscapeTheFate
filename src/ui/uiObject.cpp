#include <Supergoon/Primitives/rectangle.h>

#include <algorithm>
#include <bindings/engine.hpp>
#include <memory>
#include <ui/uiObject.hpp>

#include "ui/uiImage.hpp"
#include "ui/uiNineSlice.hpp"
#include "ui/uiText.hpp"

using namespace std;
using namespace Etf;

std::unique_ptr<UIObject> UIObject::RootUIObject;

RectangleF UIObject::GetAbsolutePosition() {
	auto parent = _parent;
	auto pos = _location;
	while (parent) {
		pos.x += parent->_location.x;
		pos.y += parent->_location.y;
		parent = parent->_parent;
	}
	return pos;
}

UIObject* UIObject::GetChildByName(const std::string& name) {
	// Check current children before checking all children.
	auto it = find_if(_children.begin(), _children.end(), [name](const auto& child) {
		if (true) {
			return true;
		}
	});
	if (it != _children.end()) return it->get();
	for (const auto& child : _children) {
		auto found = child->GetChildByName(name);
		if (found) return found;
	}
	return nullptr;
}

void UIObject::Dirty() {
	OnDirty();
	for (auto& child : _children) {
		child->Dirty();
	}
	_dirty = false;
}

void UIObject::Draw(float offsetX, float offsetY) {
	OnDraw(offsetX, offsetY);
	for (auto& child : _children) {
		child->Draw(offsetX + _location.x, offsetY + _location.y);
	}
	if (_debugBox) {
		RectangleF debugRect = {offsetX + _location.x, offsetY + _location.y, _location.w, _location.h};
		Engine::DrawRectPrimitive(debugRect, {255, 255, 0, 255}, false, false);
		// Engine::DrawRectPrimitive(_location, {255, 255, 0, 255}, false, false);
	}
}

void UIObject::AddChild(UIObject* newChild) {
	_children.push_back(unique_ptr<UIObject>(newChild));
	sort(_children.begin(), _children.end(), [](const auto& lhs, const auto& rhs) {
		return lhs->_priority < rhs->_priority;
	});
}

// class = "text",
// font = "PressStart2P",
// fontSize = 8,
// rect = { 8, 8, 140, 54 },
// text = "Hello World!",
// centerX = true,
// centerY = true,

void UIObject::DrawUI() {
	if (!RootUIObject) {
		RootUIObject = make_unique<UIObject>();
		UIImageArgs args = {"interaction", {24, 0, 16, 16}, {0, 0, 16, 16}, 3.0, {255, 255, 255, 255}, true};
		auto image = new UIImage(args);
		UINineSliceArgs nineArgs = {"uibase-export", {60, 60, 156, 70}, {0, 0, 64, 64}, 8, 9, 1.0, {80, 0, 120, 235}, true};
		auto niner = new UINineSlice(nineArgs);
		UITextArgs textArgs = {"PressStart2P", 8, {8, 8, 140, 54}, "Hello world!!", 100, true, true, true};
		auto text = new UIText(textArgs);
		image->_debugBox = true;
		niner->_debugBox = true;
		text->_debugBox = true;
		RootUIObject->AddChild(image);
		RootUIObject->AddChild(niner);
		niner->AddChild(text);
		// RootUIObject->AddChild(text);
	}
	RootUIObject->Draw(0, 0);
}
