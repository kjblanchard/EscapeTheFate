#include <imgui.h>

#include <debug/DebugPlayers.hpp>
#include <systems/PlayerSystem.hpp>
using namespace Etf;
using namespace std;

void Etf::DisplayPlayersTab() {
	if (ImGui::CollapsingHeader("Players")) {
		for (auto i = 0; i < 2; ++i) {
			auto& player = GetPlayerByNum(i);
			ImGui::Text("Current player num is %s", to_string(player->PlayerNum_).c_str());
			ImGui::Text("Is there a joystick assigned for this player? %s", to_string(player->GetController().DoesGamepadHaveJoystickAssigned()).c_str());
			ImGui::Text("Gamepad assigned is %d", (player->GetController().JoystickAssigned_));
		}
	}
}
