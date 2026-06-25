#include <Supergoon/Graphics/graphics.h>

#include <ui/uiLine.hpp>

#ifdef imgui
#include <imgui.h>
#endif
using namespace Etf;

UILine::UILine(const UILineArgs& args, const UIObjectArgs& objArgs) : UIObject(objArgs), X1_(args.X1), X2_(args.X2), Y1_(args.Y1), Y2_(args.Y2), Thickness_(args.Thickness), Color_(args.LineColor) {}

void UILine::OnDraw(float offsetX, float offsetY) {
	DrawLine(X1_ + offsetX + _location.x,
			 Y1_ + offsetY + _location.y,
			 X2_ + offsetX + _location.x,
			 Y2_ + offsetY + _location.y,
			 Thickness_,
			 &Color_,
			 false);
}

void UILine::DebugDraw() {
#ifdef imgui
	ImGui::SliderFloat("X1", &X1_, -500, 500);
	ImGui::SliderFloat("X2", &X2_, -500, 500);
	ImGui::SliderFloat("Y1", &Y1_, -500, 500);
	ImGui::SliderFloat("Y2", &Y2_, -500, 500);
	ImGui::InputInt("Thickness", &Thickness_);
#endif
}
