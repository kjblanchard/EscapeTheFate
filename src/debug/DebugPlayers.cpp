#include <imgui.h>

#include <debug/DebugPlayers.hpp>
#include <systems/PlayerControllerSystem.hpp>
using namespace Etf;
using namespace std;

void Etf::DisplayPlayerControllerTab() {
	if (ImGui::CollapsingHeader("Players")) {
		for (auto i = 0; i < 2; ++i) {
			auto& player = PlayerSystem::GetPlayerByNum(i);
			ImGui::Text("PlayerNum: %s", to_string(player->PlayerNum_).c_str());
		}
	}
}
