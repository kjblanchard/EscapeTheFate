#pragma once
#include <Supergoon/Primitives/Color.h>

#include <ui/uiObject.hpp>

namespace Etf {

struct UIPanelArgs {
	std::string Name;
	RectangleF Rect;
	Color FillColor = {20, 20, 20, 255};
	Color BorderColor = {100, 100, 120, 255};
	int Priority = 0;
	bool Visible = true;
};

class UIPanel : public UIObject {
   public:
	UIPanel(const UIPanelArgs& args);
	void OnDraw(float offsetX, float offsetY) override final;

   private:
	Color _fillColor;
	Color _borderColor;
};
}  // namespace Etf
