#include <algorithm>
#include <memory>
#include <ui/uiObject.hpp>

#include "ui/uiImage.hpp"
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
}

void UIObject::AddChild(UIObject* newChild) {
	_children.push_back(unique_ptr<UIObject>(newChild));
	sort(_children.begin(), _children.end(), [](const auto& lhs, const auto& rhs) {
		return lhs->_priority < rhs->_priority;
	});
}

void UIObject::DrawUI() {
	if (!RootUIObject) {
		RootUIObject = make_unique<UIObject>();
		UIImageArgs args = {"interaction", {24, 0, 16, 16}, {0, 0, 16, 16}, 3.0, {255, 255, 255, 255}, true};
		auto image = new UIImage(args);
		RootUIObject->AddChild(image);
	}
	RootUIObject->Draw();
}
