#include <Supergoon/Primitives/rectangle.h>
#include <algorithm>
#include <bindings/engine.hpp>
#include <memory>
#include <ui/uiObject.hpp>

#include "ui/uiImage.hpp"
#include "ui/uiNineSlice.hpp"
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

void UIObject::Draw() {
	OnDraw();
	for (auto& child : _children) {
		child->Draw();
	}
	if (_debugBox) {
		Engine::DrawRectPrimitive(_location);
	}
}

void UIObject::AddChild(UIObject* newChild) {
	_children.push_back(unique_ptr<UIObject>(newChild));
	sort(_children.begin(), _children.end(), [](const auto& lhs, const auto& rhs) {
		return lhs->_priority < rhs->_priority;
	});
}

// visible = false,
// class = "nineSlice",
// color = { 80, 0, 120, 235 },
// rect = { 60, 60, 156, 70 },
// xSize = 8,
// ySize = 9,
// filename = "uibase-export",

void UIObject::DrawUI() {
	if (!RootUIObject) {
		RootUIObject = make_unique<UIObject>();
		UIImageArgs args = {"interaction", {24, 0, 16, 16}, {0, 0, 16, 16}, 3.0, {255, 255, 255, 255}, true};
		auto image = new UIImage(args);
		UINineSliceArgs nineArgs = {"uibase-export", {60, 60, 156, 70}, {0, 0, 64, 64}, 8, 9, 1.0, {80, 0, 120, 235}, true};
		auto niner = new UINineSlice(nineArgs);
		image->_debugBox = true;
		niner->_debugBox = true;
		RootUIObject->AddChild(image);
		RootUIObject->AddChild(niner);
	}
	RootUIObject->Draw();
}
