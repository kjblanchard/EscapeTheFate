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
#include "SDL3/SDL_events.h"
#include "debug/DebugCamera.hpp"
#include "debug/DebugPlayers.hpp"
#include "debug/DebugUI.hpp"
#include "debug/DebugWindow.hpp"

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
	Engine::InitializeEngine();
	GameConfig::LoadGameConfig("./assets/config/gameConfig.json");
	auto& gameConfig = GameConfig::GetGameConfig();
	Engine::SetLogLevel(gameConfig.debug.debugLevel);
	Engine::SetupWindow(gameConfig.window.xWin, gameConfig.window.yWin, gameConfig.window.title);
	Engine::Audio::SetGlobalBGMVolume(gameConfig.audio.bgmVolume);
}

void start() {
	auto& gameConfig = GameConfig::GetGameConfig();
	GraphicsSetLogicalWorldSize(gameConfig.window.x, gameConfig.window.y);
	Engine::LoadScene("", 0.1f, 1.75, false);
	PlayerSystem::StartPlayerSystem();
#ifdef imgui
	Engine::ImGui::StartImGui();
	AddTabFuncToMainWindow(DisplayPlayersTab);
	AddTabFuncToMainWindow(DisplayCameraTab);
	AddTabFuncToMainWindow(DisplayUITab);
#endif
}

int handleEvent(void* event) {
#ifdef imgui
	auto sdlEvent = static_cast<SDL_Event*>(event);
	ImGui_ImplSDL3_ProcessEvent(sdlEvent);
#endif
	return false;
}

void update() {
	GameState::DeltaTimeSeconds = DeltaTimeSeconds;
	GameState::DeltaTimeMilliseconds = DeltaTimeMilliseconds;
	// If we are currently loading, do not update things.
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

#ifdef imgui
static void drawImGUI() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();  // Show demo window! :)
}
#endif

void draw() {
	DrawGameObjectSystem();
#ifdef imgui
	drawImGUI();
	CreateMainWindow();
#endif
}

void drawUI() {
	UI::DrawUI();
}

void postDraw() {
#ifdef imgui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
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

#ifdef imgui
static void shutdownImGUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}
#endif

void quit() {
	ShutdownGameObjectSystem();
	UI::DestroyUI();
	DialogSystem::ShutdownDialogSystem();
	Engine::ShutdownEngine();
#ifdef imgui
	shutdownImGUI();
#endif
}
}  // namespace Etf

void (*_initializeFunc)(void) = Etf::initialize;
void (*_startFunc)(void) = Etf::start;
void (*_updateFunc)(void) = Etf::update;
void (*_drawFunc)(void) = Etf::draw;
void (*_quitFunc)(void) = Etf::quit;
void (*_inputFunc)(void) = Etf::handleInput;
int (*_handleEventFunc)(void*) = Etf::handleEvent;
void (*GraphicsPostFBODrawUIFunc)(void) = Etf::drawUI;
void (*GraphicsPostFBODrawDebugFunc)(void) = Etf::postDraw;
