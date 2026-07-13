#include <bindings/engine.hpp>
#include <debug/DebugCamera.hpp>
#include <debug/DebugPlayers.hpp>
#include <debug/DebugUI.hpp>
#include <debug/DebugWindow.hpp>
#include <gameState.hpp>
#include <systems/GameObjectSystem.hpp>
#include <systems/PlayerControllerSystem.hpp>
#include <systems/battleSystem.hpp>
#include <systems/dialogSystem.hpp>
#include <ui/ui.hpp>

#include "systems/SystemCallbacks.hpp"

namespace Etf {

void initialize() {
	Engine::DebugUI::AddTab(DisplayCameraTab);
	Engine::DebugUI::AddTab(DisplayPlayerControllerTab);
	Engine::DebugUI::AddTab(DisplayUITab);
	const std::vector<SystemCallbacks> systems_{
		{
			GameObjectSystem::Start,
			GameObjectSystem::Update,
			GameObjectSystem::Draw,
			GameObjectSystem::Shutdown,
		},
		{
			PlayerControllerSystem::Start,
			PlayerControllerSystem::Update,
			nullptr,
			nullptr,
		},
		{
			nullptr,
			DialogSystem::Update,
			nullptr,
			DialogSystem::Shutdown,
		},
		{
			nullptr,
			BattleSystem::BattleSystemUpdate,
			nullptr,
			nullptr,
		},
	};
	Engine::RegisterSystems(systems_);
}


int handleEvent(void* event) {
	Engine::DebugUI::HandleEvent(event);
	return false;
}

}  // namespace Etf

extern "C" {
void InitializeEngineFunctions() {
	auto args = Etf::EngineInitializeArgs{
		"gameConfig.json",
		Etf::initialize,
		Etf::Engine::StartEngine,
		Etf::Engine::Update,
		Etf::Engine::Draw,
		Etf::Engine::Shutdown,
		nullptr,
		Etf::handleEvent,
		nullptr,
	};
	Etf::Engine::InitializeEngine(args);
}
}
