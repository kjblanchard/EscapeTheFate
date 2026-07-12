#include <imgui.h>

#include <debug/DebugUI.hpp>
#include <ui/ui.hpp>
using namespace Etf;
void Etf::DisplayUITab() {
	if (ImGui::CollapsingHeader("UI")) {
		for (auto& obj : UI::GetRootUIObject()->_children) {
			obj->DebugDrawInternal();
		}
	}
}
