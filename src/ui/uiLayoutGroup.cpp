#include <bindings/engine.hpp>
#include <ui/uiLayoutGroup.hpp>
using namespace Etf;

UILayoutGroup::UILayoutGroup(UILayoutGroupArgs& args) : _spacing(args.Spacing), _isHorizontal(args.Horizontal) {
	_location = args.Rect;
	_name = args.Name;
	_visible = args.Visible;
	_priority = args.Priority;
	_debugBox = args.DebugBox;
}

void UILayoutGroup::Draw(float offsetX, float offsetY) {
	if (!_visible) {
		return;
	}
	int iters = 0;
	for (auto& child : _children) {
		auto x = _isHorizontal ? _location.x + (_spacing * iters) : _location.x;
		auto y = _isHorizontal ? _location.y : _location.y + (_spacing * iters);
		child->Draw(offsetX + x, offsetY + y);
		++iters;
	}
	if (_debugBox) {
		RectangleF debugRect = {offsetX + _location.x, offsetY + _location.y, _location.w, _location.h};
		Engine::DrawRectPrimitive(debugRect, {255, 255, 0, 255}, false, false);
	}
}
