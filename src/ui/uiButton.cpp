#include <ui/uiButton.hpp>

using namespace Etf;

UIButton::UIButton(UIButtonArgs& args)
	: UIObject(UIObjectArgs{
		  .Rect = args.Rect,
		  .Visible = args.Visible,
		  .DoNotDestroy = false,
		  .DebugBox = false,
		  .Name = args.Name,
		  .Priority = args.Priority,
	  }) {}

bool UIButton::HitTest(float gameX, float gameY) const {
	if (!_visible) return false;
	auto abs = AbsolutePosition();
	return gameX >= abs.x && gameX < abs.x + abs.w &&
		   gameY >= abs.y && gameY < abs.y + abs.h;
}

void UIButton::SetClickCallback(std::function<void()> cb) {
	_clickCallback = std::move(cb);
}

void UIButton::SetHoverCallback(std::function<void()> cb) {
	_hoverCallback = std::move(cb);
}

void UIButton::Fire() {
	if (_clickCallback) _clickCallback();
}

void UIButton::Hover() {
	if (_hoverCallback) _hoverCallback();
}
