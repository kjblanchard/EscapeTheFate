#pragma once
#include <Supergoon/Primitives/Color.h>
#include <Supergoon/Primitives/rectangle.h>

#include <string>
#include <ui/uiObject.hpp>
namespace Etf {
struct UILayoutGroupArgs {
	std::string Name;
	RectangleF Rect;
	int Priority;
	int Spacing;
	bool Visible, Horizontal, DebugBox = false;
};
class UILayoutGroup : public UIObject {
   public:
	UILayoutGroup(UILayoutGroupArgs& args);
	void Draw(float offsetX, float offsetY) override final;

   private:
	int _spacing;
	bool _isHorizontal = false;
};
}  // namespace Etf
