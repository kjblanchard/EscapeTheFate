#pragma once
#include <Supergoon/Primitives/Color.h>

#include <ui/uiObject.hpp>

namespace Etf {

struct UILineArgs {
	float X1, X2, Y1, Y2;
	int Thickness;
	Color LineColor;
};

class UILine : public UIObject {
   public:
	UILine(const UILineArgs& args, const UIObjectArgs& objArgs);
	void OnDraw(float offsetX, float offsetY) override final;
	void DebugDraw() override final;

   private:
	float X1_, X2_, Y1_, Y2_;
	int Thickness_;
	Color Color_;
};

}  // namespace Etf
