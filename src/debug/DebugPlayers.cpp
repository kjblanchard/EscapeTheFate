#include <imgui.h>

#include <debug/DebugPlayers.hpp>
#include <systems/PlayerSystem.hpp>
using namespace Etf;
using namespace std;

void Etf::DisplayPlayersTab() {
	if (ImGui::CollapsingHeader("Players")) {
		for (auto i = 0; i < 2; ++i) {
			auto& player = GetPlayerByNum(i);
			ImGui::Text("PlayerNum: %s", to_string(player->PlayerNum_).c_str());
			ImGui::Text("JoystickAssined: %s", player->GetController().DoesGamepadHaveJoystickAssigned() ? "true" : "false");
			ImGui::Text("GamepadNum %d", (player->GetController().JoystickAssigned_));
		}
	}
}
