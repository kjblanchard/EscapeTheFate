#include <debug/DebugBattle.hpp>
#include <debug/DebugCamera.hpp>
#include <debug/DebugConsoleWindow.hpp>
#include <debug/DebugEngine.hpp>
#include <debug/DebugGameWindow.hpp>
#include <debug/DebugPlayers.hpp>
#include <debug/DebugUI.hpp>
#include <debug/DebugWindow.hpp>
#include <engine.hpp>
#include <systems/BattleTransitionSystem.hpp>
#include <systems/BattleZoneSystem.hpp>
#include <systems/GameObjectSystem.hpp>
#include <systems/PlayerControllerSystem.hpp>
#include <systems/SystemCallbacks.hpp>
#include <systems/battleSystem.hpp>
#include <systems/dialogSystem.hpp>
using namespace Etf;

void InitializeGame() {
	Engine::DebugUI::AddTab({DisplayCameraTab, DisplayPlayerControllerTab, DisplayUITab, DisplayEngineTab, DisplayBattleTab});
	auto name = (std::string) "Console";
	DebugConsoleWindow::Initialize();
	Engine::DebugUI::AddWindow({{name, DebugConsoleWindow::Draw}});
	name = "Game";
	Engine::DebugUI::AddWindow({{name, DebugGameWindow::Draw}});
	const std::vector<SystemCallbacks> systems_{
		{
			.Start = GameObjectSystem::Start,
			.Update = GameObjectSystem::Update,
			.Draw = GameObjectSystem::Draw,
			.Shutdown = GameObjectSystem::Shutdown,
		},
		{
			.Start = PlayerControllerSystem::Start,
			.Update = PlayerControllerSystem::Update,
		},
		{
			.Update = DialogSystem::Update,
			.Shutdown = DialogSystem::Shutdown,
		},
		{
			.Update = BattleSystem::BattleSystemUpdate,
		},
		{
			.Start = BattleZoneSystem::Start,
			.Update = BattleZoneSystem::Update,
			.Draw = BattleZoneSystem::Draw,
			.Shutdown = BattleZoneSystem::Shutdown,
		},
		{
			.Start = BattleTransitionSystem::Start,
			.Update = BattleTransitionSystem::Update,
			.Shutdown = BattleTransitionSystem::Shutdown,
		}};
	Engine::RegisterSystems(systems_);
}
