#include <Supergoon/Graphics/graphics.h>
#include <Supergoon/log.h>

#include <ui/uiProgressBar.hpp>

using namespace Etf;

UIProgressBar::UIProgressBar(const UIProgressBarArgs& args) {
	_location = args.Rect;
	_barRect = args.BarRect;
	_name = args.Name;
	_visible = args.Visible;
	_priority = args.Priority;
	_barColor = args.BarColor;
	_debugBox = args.DebugBox;
}

void UIProgressBar::OnDraw(float offsetX, float offsetY) {
	auto barWidth = _barRect.w * _barPercent / 100.00f;
	RectangleF drawRect = {offsetX + _location.x + _barRect.x, offsetY + _location.y + _barRect.y, barWidth, _barRect.h};
	DrawRect(&drawRect, &_barColor, true, false);
}
