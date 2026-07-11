#include <Supergoon/Audio/Audio.h>
#include <Supergoon/Graphics/graphics.h>
#include <Supergoon/Input/keyboard.h>
#include <Supergoon/Platform/sdl/sdlWindow.h>
#include <Supergoon/camera.h>
#include <Supergoon/engine.h>
#include <Supergoon/state.h>

#include <bindings/Controller.hpp>
#include <bindings/engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <gameobject/GameObject.hpp>
#include <systems/GameObjectSystem.hpp>
#include <systems/PlayerSystem.hpp>
#include <systems/battleSystem.hpp>
#include <systems/dialogSystem.hpp>
#include <ui/ui.hpp>

#ifdef imgui
#include <Supergoon/Platform/sdl/sdlWindow.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include <debug/DebugCamera.hpp>
#include <debug/DebugPlayers.hpp>
#include <debug/DebugUI.hpp>
#include <debug/DebugWindow.hpp>
#endif

namespace Etf {
static const int B = 27;

void initialize() {
	Engine::InitializeEngine("gameConfig.json");
}

void start() {
	Engine::StartEngine();
	Engine::LoadScene("", 0.1f, 1.75, false);
	PlayerSystem::StartPlayerSystem();
	//setup debug windows
	Engine::DebugUI::StartImGui();
	AddTabFuncToMainWindow(DisplayPlayersTab);
	AddTabFuncToMainWindow(DisplayCameraTab);
	AddTabFuncToMainWindow(DisplayUITab);
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

static void enterBattle() {
	GameState::NextLoadMapName = Engine::CurrentScene();
	Engine::LoadScene("forest1", 0.25f, 2.75f, false);
}

void handleInput() {
	if (IsKeyboardKeyJustPressed(B)) {
		enterBattle();
	}
}

void quit() {
	ShutdownGameObjectSystem();
	UI::DestroyUI();
	DialogSystem::ShutdownDialogSystem();
	Engine::ShutdownEngine();
	Engine::DebugUI::ShutdownImGui();
}
}  // namespace Etf

void (*_initializeFunc)(void) = Etf::initialize;
void (*_startFunc)(void) = Etf::start;
void (*_updateFunc)(void) = Etf::update;
void (*_drawFunc)(void) = Etf::draw;
void (*_quitFunc)(void) = Etf::quit;
void (*_inputFunc)(void) = Etf::handleInput;
int (*_handleEventFunc)(void*) = Etf::handleEvent;
