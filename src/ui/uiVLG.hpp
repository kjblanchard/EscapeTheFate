
#pragma once
#include <Supergoon/Primitives/Color.h>
#include <Supergoon/Primitives/rectangle.h>

#include <string>
#include <ui/uiObject.hpp>
namespace Etf {
struct UIVLGArgs {
	std::string Name;
	RectangleF Rect;
	int Priority;
	int Spacing;
	bool Visible, DebugBox = false;
};
class UIVLG : public UIObject {
   public:
	UIVLG(UIVLGArgs& args);
	// ~UIVLG();
	void Draw(float offsetX, float offsetY) override final;

   private:
	int _spacing;
};
}  // namespace Etf
