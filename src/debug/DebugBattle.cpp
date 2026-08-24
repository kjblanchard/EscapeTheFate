#include <imgui.h>

#include <battle/battlerData.hpp>
#include <battle/statuseffects/statusEffects.hpp>
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
		case Etf::BattleStates::BattleSpoils:
			return "Spoils";
		case Etf::BattleStates::BattleEnd:
			return "End";
		case Etf::BattleStates::BattleGameOver:
			return "Game Over";
		default:
			return "Not implemented";
	}
}

static const char* getBattlerStateText(BattlerStates state) {
	switch (state) {
		case BattlerStates::Default:
			return "Default";
		case BattlerStates::Initialized:
			return "Initialized";
		case BattlerStates::ATBCharging:
			return "ATB Charging";
		case BattlerStates::ATBFullyCharged:
			return "ATB Fully Charged";
		case BattlerStates::CommandSelection:
			return "Command Selection";
		case BattlerStates::TargetSelection:
			return "Target Selection";
		case BattlerStates::BattleEndStart:
			return "Battle End Start";
		case BattlerStates::BattleEndIdle:
			return "Battle End Idle";
		case BattlerStates::BattleEnd:
			return "Battle End";
		default:
			return "Unknown";
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
			for (int i = 0; i < (int)battlerDatabase_.size(); ++i) {
				auto& bd = battlerDatabase_[i];
				auto nodeLabel = to_string(i) + ": " + bd.Name;
				if (ImGui::TreeNode(nodeLabel.c_str())) {
					ImGui::Text("Sprite: %s  Idle: %s", bd.Sprite.c_str(), bd.IdleAnimation.c_str());
					ImGui::Text("HP:%-4d Str:%-3d Mag:%-3d Def:%-3d MDef:%-3d Spd:%-3d Pow:%-3d",
								bd.HP, bd.Str, bd.Mag, bd.Def, bd.MDef, bd.Spd, bd.Pow);
					ImGui::Text("Rect: x=%.0f y=%.0f w=%.0f h=%.0f",
								bd.Location.x, bd.Location.y, bd.Location.w, bd.Location.h);
					ImGui::TreePop();
				}
			}
		}
		if (ImGui::CollapsingHeader("BattleGroups")) {
			for (int g = 0; g < (int)_battleGroups.size(); ++g) {
				bool isActive = (g == GameState::Battle::NextBattleGroup);
				auto groupLabel = "Group " + to_string(g) + (isActive ? " [active]" : "");
				if (ImGui::TreeNode(groupLabel.c_str())) {
					auto& group = _battleGroups[g];
					for (int s = 0; s < (int)group.size(); ++s) {
						int id = group[s];
						if (id == 0) {
							ImGui::Text("  Slot %d: empty", s);
						} else if (id < (int)battlerDatabase_.size()) {
							ImGui::Text("  Slot %d: %s (id=%d)", s, battlerDatabase_[id].Name.c_str(), id);
						} else {
							ImGui::Text("  Slot %d: INVALID id=%d", s, id);
						}
					}
					ImGui::TreePop();
				}
			}
		}
		if (ImGui::CollapsingHeader("Battlers")) {
			if (_battlers.empty()) {
				ImGui::Text("No battlers (battle not started)");
			} else {
				for (int i = 0; i < (int)_battlers.size(); ++i) {
					auto* battler = _battlers[i];
					if (!battler) continue;
					bool isPlayer = battler->IsPlayer();
					auto* data = battler->DebugData();
					auto slotLabel = "Slot " + to_string(i) + ": " + data->Name +
									 (isPlayer ? " [Player]" : " [Enemy]");
					if (ImGui::TreeNode(slotLabel.c_str())) {
						if (isPlayer) {
							auto* pb = static_cast<PlayerBattler*>(battler);
							ImGui::Text("State: %s", getBattlerStateText(pb->DebugState()));
						}
						int maxHP = data->HP;
						if (isPlayer) {
							ImGui::SetNextItemWidth(80.0f);
							ImGui::InputInt("HP", &battler->DebugHP());
							ImGui::SameLine();
							ImGui::Text("/ %d (base)", maxHP);
						} else {
							ImGui::Text("HP: %d / %d", (int)battler->CurrentHP(), maxHP);
						}
						float& atb = battler->DebugATBCharge();
						int maxATB = battler->DebugMaxATB();
						if (isPlayer) {
							ImGui::SetNextItemWidth(150.0f);
							ImGui::SliderFloat("ATB", &atb, 0.0f, (float)maxATB);
							ImGui::SetNextItemWidth(80.0f);
							ImGui::SliderInt("AP", &battler->DebugAP(), 0, battler->DebugMaxAP());
						} else {
							ImGui::Text("ATB: %.1f / %d", atb, maxATB);
							ImGui::Text("AP: %d / %d", battler->CurrentAP(), battler->DebugMaxAP());
						}
						ImGui::Separator();
						if (isPlayer) {
							ImGui::SetNextItemWidth(60.0f);
							ImGui::InputInt("Str", &data->Str);
							ImGui::SameLine();
							ImGui::SetNextItemWidth(60.0f);
							ImGui::InputInt("Mag", &data->Mag);
							ImGui::SameLine();
							ImGui::SetNextItemWidth(60.0f);
							ImGui::InputInt("Def", &data->Def);
							ImGui::SetNextItemWidth(60.0f);
							ImGui::InputInt("MDef", &data->MDef);
							ImGui::SameLine();
							ImGui::SetNextItemWidth(60.0f);
							ImGui::InputInt("Spd", &data->Spd);
							ImGui::SameLine();
							ImGui::SetNextItemWidth(60.0f);
							ImGui::InputInt("Pow", &data->Pow);
						} else {
							ImGui::Text("Str:%-3d Mag:%-3d Def:%-3d MDef:%-3d Spd:%-3d Pow:%-3d",
										data->Str, data->Mag, data->Def, data->MDef, data->Spd, data->Pow);
						}
						ImGui::TreePop();
					}
				}
			}
		}
		if (ImGui::CollapsingHeader("Relics")) {
			auto& relics = GameState::Battle::PlayerRelics;
			ImGui::Text("Player Relics (%d):", (int)relics.size());
			for (int i = 0; i < (int)relics.size(); ++i) {
				const char* name = "Unknown";
				switch (relics[i]) {
					case StatusEffects::RelicDamageBonus: name = "+1 Damage"; break;
					case StatusEffects::RelicSpeedBoost: name = "+2 Speed (2 turns)"; break;
					case StatusEffects::RelicShield: name = "Shield (5 dmg)"; break;
					default: break;
				}
				ImGui::BulletText("%s", name);
			}
			if (ImGui::Button("Add +1 Damage")) relics.push_back(StatusEffects::RelicDamageBonus);
			ImGui::SameLine();
			if (ImGui::Button("Add Speed Boost")) relics.push_back(StatusEffects::RelicSpeedBoost);
			ImGui::SameLine();
			if (ImGui::Button("Add Shield")) relics.push_back(StatusEffects::RelicShield);
			if (ImGui::Button("Clear Relics")) relics.clear();
		}
	}
#else
	return;
#endif
}
