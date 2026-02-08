#include <Supergoon/Audio/Audio.h>
#include <Supergoon/Graphics/graphics.h>
#include <Supergoon/Input/keyboard.h>
#include <Supergoon/Platform/sdl/sdlWindow.h>
#include <Supergoon/camera.h>
#include <Supergoon/engine.h>
#include <Supergoon/log.h>
#include <Supergoon/state.h>
#include <Supergoon/window.h>

#include <bindings/engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <gameobject/GameObject.hpp>
#include <systems/battleSystem.hpp>
#include <systems/dialogSystem.hpp>
#include <ui/ui.hpp>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"

namespace Etf {
static const int B = 27;

static void startImGUI() {
#ifdef imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
	// Setup Platform/Renderer backends
	// auto window = WindowGet();
	// auto ptr = GraphicsGetContextPtr();
	ImGui_ImplSDL3_InitForOpenGL((SDL_Window*)WindowGet()->Handle, GraphicsGetContextPtr());
	ImGui_ImplOpenGL3_Init();
#endif
}

void initialize() {
	GameConfig::LoadGameConfig("./assets/config/gameConfig.json");
	auto& _gameConfig = GameConfig::GetGameConfig();
	SetWindowOptions(_gameConfig.window.xWin, _gameConfig.window.yWin, _gameConfig.window.title.c_str());
	SetGlobalBgmVolume(_gameConfig.audio.bgmVolume);
}

void start() {
	auto& _gameConfig = GameConfig::GetGameConfig();
	GraphicsSetLogicalWorldSize(_gameConfig.window.x, _gameConfig.window.y);
#ifdef __EMSCRIPTEN__
	Engine::PreloadAssets();
#endif
	// Initial load screen.
	Engine::LoadScene("", 0.1f, 1.75, false);
	startImGUI();
}

int handleEvent(void* event) {
	auto sdlEvent = static_cast<SDL_Event*>(event);
#ifdef imgui
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
	GameObject::UpdateGameObjects();
	DialogSystem::UpdateDialogSystem();
	if (GameState::Battle::InBattle) {
		BattleSystem::BattleSystemUpdate();
	}
}

static void drawImGUI() {
#ifdef imgui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();  // Show demo window! :)
#endif
}

void draw() {
	GameObject::DrawGameObjects();
	UI::DrawUI();
#ifdef imgui
	drawImGUI();
#endif
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
static void shutdownImGUI() {
#ifdef imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
#endif
}

void quit() {
	GameObject::DestroyAllGameObjects();
	UI::DestroyUI();
	DialogSystem::ShutdownDialogSystem();
}
}  // namespace Etf

void (*_initializeFunc)(void) = Etf::initialize;
void (*_startFunc)(void) = Etf::start;
void (*_updateFunc)(void) = Etf::update;
void (*_drawFunc)(void) = Etf::draw;
void (*_quitFunc)(void) = Etf::quit;
void (*_inputFunc)(void) = Etf::handleInput;
int (*_handleEventFunc)(void*) = Etf::handleEvent;
void (*GraphicsPostFBODrawDebugFunc)(void) = Etf::postDraw;
