#pragma once
#include <Supergoon/Primitives/Color.h>

#include <ui/uiObject.hpp>

namespace Etf {

struct UIButtonArgs {
	// std::string Name;
	// RectangleF Rect;
	// Color FillColor = {20, 20, 20, 255};
	// Color BorderColor = {100, 100, 120, 255};
	// int Priority = 0;
	// bool Visible = true;
};

class UIButton : public UIObject {
   public:
	UIButton(const UIObjectArgs& oargs, const UIButtonArgs& args);
	void OnDraw(float offsetX, float offsetY) override final;

   private:
	Color _fillColor;
	Color _borderColor;
};
}  // namespace Etf
