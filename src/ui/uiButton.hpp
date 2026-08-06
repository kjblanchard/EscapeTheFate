#pragma once
#include <functional>
#include <ui/uiObject.hpp>

namespace Etf {

struct UIButtonArgs {
	RectangleF Rect;
	std::string Name;
	int Priority = 0;
	bool Visible = true;
};

class UIButton : public UIObject {
   public:
	UIButton(UIButtonArgs& args);
	bool HitTest(float gameX, float gameY) const;
	void SetClickCallback(std::function<void()> cb);
	void SetHoverCallback(std::function<void()> cb);
	void Fire();
	void Hover();
	void OnDraw(float offsetX, float offsetY) override final {}

   private:
	std::function<void()> _clickCallback;
	std::function<void()> _hoverCallback;
};

}  // namespace Etf
