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
	Engine::Update();
	if (!Engine::HandleMapLoad()) return;
	GameObjectSystem::UpdateGameObjectSystem();
	DialogSystem::UpdateDialogSystem();
	PlayerSystem::UpdatePlayerSystem();
	if (GameState::Battle::InBattle) BattleSystem::BattleSystemUpdate();
}

void draw() {
	GameObjectSystem::DrawGameObjectSystem();
	Engine::DebugUI::Draw();
}

void quit() {
	GameObjectSystem::ShutdownGameObjectSystem();
	UI::DestroyUI();
	DialogSystem::ShutdownDialogSystem();
	Engine::Shutdown();
}
}  // namespace Etf

extern "C" {
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
}
