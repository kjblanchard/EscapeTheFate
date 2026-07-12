#include <Supergoon/engine.h>

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

namespace Etf {

void initialize() {
	Engine::DebugUI::AddTab(DisplayCameraTab);
	Engine::DebugUI::AddTab(DisplayPlayerControllerTab);
	Engine::DebugUI::AddTab(DisplayUITab);
}

void start() {
	Engine::StartEngine();
	Engine::LoadScene("", 0.1f, 1.75, false);
	PlayerSystem::StartPlayerSystem();
	Engine::DebugUI::Start();
}

int handleEvent(void* event) {
	Engine::DebugUI::HandleEvent(event);
	return false;
}

void update() {
	// If we are currently loading, do not update things.
	Engine::Update();
	if (!Engine::HandleMapLoad()) {
		return;
	}
	UpdateGameObjectSystem();
	DialogSystem::UpdateDialogSystem();
	PlayerSystem::UpdatePlayerSystem();
	if (GameState::Battle::InBattle) {
		BattleSystem::BattleSystemUpdate();
	}
}

void draw() {
	DrawGameObjectSystem();
	Engine::DebugUI::Draw();
}

void quit() {
	ShutdownGameObjectSystem();
	UI::DestroyUI();
	DialogSystem::ShutdownDialogSystem();
	Engine::ShutdownEngine();
	Engine::DebugUI::ShutdownImGui();
}
}  // namespace Etf
   //
void InitializeEngineFunctions() {
	auto args = Etf::EngineInitializeArgs{
		"gameConfig.json",
		Etf::initialize,
		Etf::start,
		Etf::update,
		Etf::draw,
		Etf::quit,
		nullptr,
		Etf::handleEvent,
		nullptr,
	};
	Etf::Engine::InitializeEngine(args);
}
