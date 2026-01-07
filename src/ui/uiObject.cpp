#include <Supergoon/Primitives/rectangle.h>

#include <algorithm>
#include <bindings/engine.hpp>
#include <memory>
#include <ui/uiObject.hpp>

using namespace std;
using namespace Etf;

UIObject::UIObject(UIObjectArgs args) : _visible(args.Visible), _priority(args.Priority), _name(args.Name), _location(args.Rect) {
}

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
