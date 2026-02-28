#include <imgui.h>
#include <systems/PlayerSystem.hpp>

#include <debug/DebugPlayers.hpp>
using namespace Etf;
using namespace std;

void Etf::DisplayPlayersTab() {
	if (ImGui::CollapsingHeader("Players")) {
		auto& players = PlayerSystem::Players_;
		for (auto& player : players) {
			ImGui::Text("%s", to_string(player->PlayerNum_).c_str());
			ImGui::Text("%s", to_string(player->GetController().DoesGamepadHaveJoystickAssigned()).c_str());
		}

	}
}
