#include <bindings/engine.hpp>
#include <debug/DebugCamera.hpp>
#include <debug/DebugPlayers.hpp>
#include <debug/DebugUI.hpp>
#include <debug/DebugWindow.hpp>
#include <gameState.hpp>
#include <systems/GameObjectSystem.hpp>
#include <systems/PlayerSystem.hpp>
#include <systems/battleSystem.hpp>
#include <systems/dialogSystem.hpp>
#include <ui/ui.hpp>

namespace Etf {

static void initialize() {
	Engine::InitializeEngine("gameConfig.json");
}

static void start() {
	Engine::StartEngine();
	Engine::LoadScene("", 0.1f, 1.75, false);
	PlayerSystem::StartPlayerSystem();
#ifdef imgui
	Engine::DebugUI::StartImGui();
	AddTabFuncToMainWindow(DisplayPlayersTab);
	AddTabFuncToMainWindow(DisplayCameraTab);
	AddTabFuncToMainWindow(DisplayUITab);
#endif
}

static int handleEvent(void* event) {
	Engine::DebugUI::HandleEvent(event);
	return false;
}

static void update() {
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

static void draw() {
	DrawGameObjectSystem();
	Engine::DebugUI::Draw();
}

static void quit() {
	ShutdownGameObjectSystem();
	UI::DestroyUI();
	DialogSystem::ShutdownDialogSystem();
	Engine::ShutdownEngine();
	Engine::DebugUI::ShutdownImGui();
}
}  // namespace Etf

void (*_initializeFunc)(void) = Etf::initialize;
void (*_inputFunc)(void) = nullptr;
void (*_startFunc)(void) = Etf::start;
void (*_updateFunc)(void) = Etf::update;
void (*_drawFunc)(void) = Etf::draw;
void (*_quitFunc)(void) = Etf::quit;
int (*_handleEventFunc)(void*) = Etf::handleEvent;
