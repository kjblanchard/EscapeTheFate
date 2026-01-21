#include <bindings/engine.hpp>
#include <ui/uiHLG.hpp>
using namespace Etf;

UIHLG::UIHLG(UIHLGArgs& args) {
	_location = args.Rect;
	_name = args.Name;
	_visible = args.Visible;
	_priority = args.Priority;
	_debugBox = args.DebugBox;
	_spacing = args.Spacing;
}

void UIHLG::Draw(float offsetX, float offsetY) {
	if (!_visible) {
		return;
	}
	int iters = 0;
	for (auto& child : _children) {
		child->Draw(offsetX + _location.x + (_spacing * iters), offsetY + _location.y + (_spacing * iters));
		++iters;
	}
	if (_debugBox) {
		RectangleF debugRect = {offsetX + _location.x, offsetY + _location.y, _location.w, _location.h};
		Engine::DrawRectPrimitive(debugRect, {255, 255, 0, 255}, false, false);
	}
}
