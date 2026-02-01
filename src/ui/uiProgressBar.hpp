
#pragma once
#include <Supergoon/Primitives/Color.h>
#include <Supergoon/Primitives/rectangle.h>

#include <ui/uiObject.hpp>

namespace Etf {

struct UIProgressBarArgs {
	std::string Name;
	RectangleF Rect;
	RectangleF BarRect;
	Color BarColor;
	int Priority;
	bool Visible, DebugBox = false, DoNotDestroy = false;
};
class UIProgressBar : public UIObject {
   public:
	UIProgressBar(const UIProgressBarArgs& args);
	inline float CurrentPercent() { return _barPercent; }
	inline void SetBarPercent(float percent) { _barPercent = percent; }
	void OnDraw(float offsetX, float offsetY) override final;

   private:
	RectangleF _barRect;
	float _barPercent = 0;
	Color _barColor = {255, 255, 255, 255};
};
}  // namespace Etf
