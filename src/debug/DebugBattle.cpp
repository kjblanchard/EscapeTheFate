#include <imgui.h>

#include <battle/battlerData.hpp>
#include <debug/DebugBattle.hpp>
#include <engine.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/EnemyBattler.hpp>
#include <gameobject/gameobjects/PlayerBattler.hpp>
#include <systems/battleSystem.hpp>
#include <vector>
using namespace Etf;
using namespace std;

// Non statics for debugging
extern vector<BattlerData> battlerDatabase_;
//  Loaded battle groups from the database, used when loading battle and stays loaded
extern vector<vector<int>> _battleGroups;
// Current battlers spawned in, always the size of all positions.
extern vector<Battler*> _battlers;
extern bool battleInitialized_;
extern BattleStates currentBattleState_;
extern BattleStates nextBattleState_;
extern string sceneToLoadAfterBattle_;

static string battleScene = "forest1";

static string getCurrentBattleStateText(BattleStates state) {
	switch (state) {
		case Etf::BattleStates::NotInitialized:
			return "Not Initialized";
		case Etf::BattleStates::NotInBattle:
			return "Not in battle";
		case Etf::BattleStates::BattleStartTriggered:
			return "Battle start triggered";
		case Etf::BattleStates::Battle:
			return "Battle";
		case Etf::BattleStates::BattleVictory:
			return "Victory";
		case Etf::BattleStates::BattleEnd:
			return "End";
	}
}

void Etf::DisplayBattleTab() {
#ifdef imgui
	if (ImGui::CollapsingHeader("Battle")) {
		ImGui::Text("Battle initialized %s", battleInitialized_ ? "True" : "false");
		ImGui::Text("Current battle state %s", getCurrentBattleStateText(currentBattleState_).c_str());
		ImGui::Text("NextBattleState %s", getCurrentBattleStateText(nextBattleState_).c_str());
		ImGui::SliderInt("Next Battle Group", &GameState::Battle::NextBattleGroup, 0, _battleGroups.size() - 1);
		if (ImGui::Button("Enter Battle")) {
			GameState::NextLoadMapName = Engine::CurrentSceneName();
			Engine::LoadScene(battleScene, 0.25f, 2.75f, false);
		}
		if (ImGui::CollapsingHeader("BattlerDatabase")) {
		}
		if (ImGui::CollapsingHeader("BattleGroups")) {
		}
		if (ImGui::CollapsingHeader("Battlers")) {
		}
	}
#else
	return;
#endif
}
