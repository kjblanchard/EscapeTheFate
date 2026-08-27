#include <Supergoon/Input/mouse.h>
#ifdef IS_MOBILE
#include <Supergoon/Input/touch.h>
#endif

#include <algorithm>
#include <systems/PointerInputSystem.hpp>
#include <ui/uiObject.hpp>
#include <vector>

using namespace Etf;

namespace {
struct GroupEntry {
	enum class Type { UIObject, Rect } type;
	union {
		UIButtonGroup* obj;
		UIButtonGroupRect* rect;
	};
};

std::vector<GroupEntry> _stack;
}  // namespace

// --- UIButtonGroup ---

UIButtonGroup::UIButtonGroup(FocusCallback onFocus, ActivateCallback onActivate)
	: _onFocus(std::move(onFocus)), _onActivate(std::move(onActivate)) {}

void UIButtonGroup::AddButton(UIObject* target, int logicalIndex) {
	_buttons.push_back({target, logicalIndex});
}

void UIButtonGroup::ClearButtons() {
	_buttons.clear();
	_focusedIndex = -1;
}

void UIButtonGroup::SetFocusedIndex(int index) {
	_focusedIndex = index;
}

void UIButtonGroup::Update(float pointerX, float pointerY, bool justClicked) {
	if (!justClicked || pointerX < 0 || pointerY < 0) return;

	for (auto& btn : _buttons) {
		if (!btn.target) continue;
		auto rect = btn.target->AbsolutePosition();
		if (pointerX >= rect.x && pointerX < rect.x + rect.w &&
			pointerY >= rect.y && pointerY < rect.y + rect.h) {
			if (_focusedIndex == btn.index) {
				if (_onActivate) _onActivate(btn.index);
			} else {
				_focusedIndex = btn.index;
				if (_onFocus) _onFocus(btn.index);
			}
			return;
		}
	}
}

// --- UIButtonGroupRect ---

UIButtonGroupRect::UIButtonGroupRect(FocusCallback onFocus, ActivateCallback onActivate)
	: _onFocus(std::move(onFocus)), _onActivate(std::move(onActivate)) {}

void UIButtonGroupRect::AddButton(RectangleF rect, int logicalIndex) {
	_buttons.push_back({rect, logicalIndex});
}

void UIButtonGroupRect::ClearButtons() {
	_buttons.clear();
	_focusedIndex = -1;
}

void UIButtonGroupRect::SetFocusedIndex(int index) {
	_focusedIndex = index;
}

void UIButtonGroupRect::Update(float pointerX, float pointerY, bool justClicked) {
	if (!justClicked || pointerX < 0 || pointerY < 0) return;

	for (auto& btn : _buttons) {
		if (pointerX >= btn.rect.x && pointerX < btn.rect.x + btn.rect.w &&
			pointerY >= btn.rect.y && pointerY < btn.rect.y + btn.rect.h) {
			if (_focusedIndex == btn.index) {
				if (_onActivate) _onActivate(btn.index);
			} else {
				_focusedIndex = btn.index;
				if (_onFocus) _onFocus(btn.index);
			}
			return;
		}
	}
}

// --- PointerInputSystem ---

void PointerInputSystem::Start() {
	_stack.clear();
}

void PointerInputSystem::Update() {
	if (_stack.empty()) return;

	float px, py;
	bool justClicked = false;

#ifdef IS_MOBILE
	GetGameTouchPos(&px, &py);
	justClicked = IsTouchJustPressed();
#else
	GetGameMousePos(&px, &py);
	justClicked = IsMouseButtonJustPressed(MouseButtonsLeftClick);
#endif

	if (!justClicked) return;

	auto& top = _stack.back();
	if (top.type == GroupEntry::Type::UIObject) {
		top.obj->Update(px, py, justClicked);
	} else {
		top.rect->Update(px, py, justClicked);
	}
}

void PointerInputSystem::PushGroup(UIButtonGroup* group) {
	if (!group) return;
	_stack.push_back({GroupEntry::Type::UIObject, {.obj = group}});
}

void PointerInputSystem::PushGroupRect(UIButtonGroupRect* group) {
	if (!group) return;
	_stack.push_back({GroupEntry::Type::Rect, {.rect = group}});
}

void PointerInputSystem::PopGroup(UIButtonGroup* group) {
	if (!group) return;
	auto it = std::find_if(_stack.begin(), _stack.end(), [group](const GroupEntry& e) {
		return e.type == GroupEntry::Type::UIObject && e.obj == group;
	});
	if (it != _stack.end()) _stack.erase(it);
}

void PointerInputSystem::PopGroupRect(UIButtonGroupRect* group) {
	if (!group) return;
	auto it = std::find_if(_stack.begin(), _stack.end(), [group](const GroupEntry& e) {
		return e.type == GroupEntry::Type::Rect && e.rect == group;
	});
	if (it != _stack.end()) _stack.erase(it);
}
