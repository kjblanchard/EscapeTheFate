#include <imgui.h>

#include <debug/DebugUI.hpp>
#include <ui/ui.hpp>
using namespace Etf;
void Etf::DisplayUITab() {
	if (ImGui::CollapsingHeader("UI")) {
		for (auto& obj : UI::_rootUIObject->_children) {
			obj->DebugDrawInternal();
		}
	}
}
