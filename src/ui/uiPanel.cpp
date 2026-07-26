#include <engine.hpp>
#include <ui/uiPanel.hpp>

using namespace Etf;

UIPanel::UIPanel(const UIPanelArgs& args) {
	_location = args.Rect;
	_name = args.Name;
	_visible = args.Visible;
	_priority = args.Priority;
	_fillColor = args.FillColor;
	_borderColor = args.BorderColor;
}

void UIPanel::OnDraw(float offsetX, float offsetY) {
	RectangleF rect = {offsetX + _location.x, offsetY + _location.y, _location.w, _location.h};
	DrawRect(&rect, &_fillColor, true, false);
	Engine::Debug::DrawRectPrimitive(rect, _borderColor, false, false);
}
