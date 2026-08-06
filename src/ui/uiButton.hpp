#pragma once
#include <functional>
#include <ui/uiObject.hpp>

namespace Etf {

struct UIButtonArgs {
	RectangleF Rect;
	std::string Name;
	int Priority = 0;
	bool Visible = true;
	int Layer = 0;
};

class UIButton : public UIObject {
   public:
	UIButton(UIButtonArgs& args);
	bool HitTest(float gameX, float gameY) const;
	void SetClickCallback(std::function<void()> cb);
	void SetHoverCallback(std::function<void()> cb);
	void SetRightClickCallback(std::function<void()> cb);
	void Fire();
	void FireRightClick();
	void Hover();
	int GetLayer() const { return _layer; }
	void SetLayer(int layer) { _layer = layer; }
	void OnDraw(float offsetX, float offsetY) override final {}

   private:
	bool isAncestorVisible() const;
	std::function<void()> _clickCallback;
	std::function<void()> _hoverCallback;
	std::function<void()> _rightClickCallback;
	int _layer = 0;
};

}  // namespace Etf
