#include <engine.hpp>
#include <debug/DebugCamera.hpp>
#include <debug/DebugPlayers.hpp>
#include <debug/DebugUI.hpp>
#include <debug/DebugWindow.hpp>
#include <debug/DebugEngine.hpp>
#include <systems/GameObjectSystem.hpp>
#include <systems/PlayerControllerSystem.hpp>
#include <systems/SystemCallbacks.hpp>
#include <systems/battleSystem.hpp>
#include <systems/dialogSystem.hpp>
using namespace Etf;

void InitializeGame() {
	Engine::DebugUI::AddTab({DisplayCameraTab, DisplayPlayerControllerTab, DisplayUITab, DisplayEngineTab});
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
	};
	Engine::RegisterSystems(systems_);
}
