#include <Supergoon/Primitives/rectangle.h>
#include <Supergoon/log.h>

#include <algorithm>
#include <bindings/engine.hpp>
#include <memory>
#include <ui/uiObject.hpp>

using namespace std;
using namespace Etf;

UIObject::UIObject(UIObjectArgs args) : _doNotDestroy(args.DoNotDestroy), _visible(args.Visible), _priority(args.Priority), _name(args.Name), _location(args.Rect), _debugBox(args.DebugBox) {
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
		return child->_name == name;
	});
	if (it != _children.end()) return it->get();
	for (const auto& child : _children) {
		auto found = child->GetChildByName(name);
		if (found) return found;
	}
	return nullptr;
}

bool UIObject::HasChildOfName(const std::string& name) {
	return find_if(_children.begin(), _children.end(), [&name](auto& child) { return child->_name == name; }) != _children.end();
}

bool UIObject::HasChildOfNameInAllChildren(const std::string& name) {
	if (HasChildOfName(name)) return true;
	for (auto& child : _children) {
		if (child->HasChildOfNameInAllChildren(name)) return true;
	}
	return false;
}

void UIObject::Dirty() {
	OnDirty();
	for (auto& child : _children) {
		child->Dirty();
	}
	_dirty = false;
}

void UIObject::Draw(float offsetX, float offsetY) {
	if (!_visible) {
		return;
	}

	OnDraw(offsetX, offsetY);
	for (auto& child : _children) {
		child->Draw(offsetX + _location.x, offsetY + _location.y);
	}
	if (_debugBox) {
		RectangleF debugRect = {offsetX + _location.x, offsetY + _location.y, _location.w, _location.h};
		Engine::DrawRectPrimitive(debugRect, {255, 255, 0, 255}, false, false);
	}
}

void UIObject::AddChild(UIObject* newChild) {
	_children.push_back(unique_ptr<UIObject>(newChild));
	sort(_children.begin(), _children.end(), [](const auto& lhs, const auto& rhs) {
		return lhs->_priority < rhs->_priority;
	});
}

void UIObject::DestroyChildByName(const std::string& name, bool force) {
	erase_if(_children, [&name, force](const auto& child) {
		return force ? child->_name == name : child->_name == name && !child->_doNotDestroy;
	});
}

void UIObject::DestroyChildByName(const std::vector<std::string> names, bool force) {
	for (auto& name : names) {
		DestroyChildByName(name, force);
	}
}
void UIObject::DestroyChildIfNotName(const std::string& name, bool force) {
	sgLogDebug("Trying to destsroy everything but %s", name.c_str());
	erase_if(_children, [&name, force](const auto& child) {
		return force ? child->_name != name : child->_name != name && !child->_doNotDestroy;
	});
}
void UIObject::DestroyChildIfNotName(const std::vector<std::string> names, bool force) {
	for (auto& name : names) {
		DestroyChildIfNotName(name, force);
	}
}
