#include <Supergoon/Audio/Audio.h>
#include <Supergoon/Graphics/graphics.h>
#include <Supergoon/Input/keyboard.h>
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

namespace Etf {
static const int B = 27;
static const int UP = 26;
static const int LEFT = 4;
static const int DOWN = 22;
static const int RIGHT = 7;
static const int A = 44;

void initialize() {
	GameConfig::LoadGameConfig("./assets/config/gameConfig.json");
	auto& _gameConfig = GameConfig::GetGameConfig();
	SetWindowOptions(_gameConfig.window.xWin, _gameConfig.window.yWin, _gameConfig.window.title.c_str());
	SetGlobalBgmVolume(_gameConfig.audio.bgmVolume);
}

void start() {
	auto& _gameConfig = GameConfig::GetGameConfig();
	GraphicsSetLogicalWorldSize(_gameConfig.window.x, _gameConfig.window.y);
	Engine::LoadScene();
}

void update() {
	GameState::DeltaTimeSeconds = DeltaTimeSeconds;
	GameState::DeltaTimeMilliseconds = DeltaTimeMilliseconds;
	GameState::Ticks = Ticks;
	Engine::HandleMapLoad();
	GameObject::UpdateGameObjects();
	DialogSystem::UpdateDialogSystem();
	if (GameState::Battle::InBattle) {
		BattleSystem::BattleSystemUpdate();
	}
	// if (IsKeyboardKeyDown(RIGHT)) {
	// 	// cameraPos[0] = cameraPos[0] + 0.63421325;
	// 	cameraPos[0] = cameraPos[0] + 0.63421325;
	// }
	// if (IsKeyboardKeyDown(LEFT)) {
	// 	cameraPos[0] = cameraPos[0] - 0.6421498053;
	// }
	// if (IsKeyboardKeyDown(UP)) {
	// 	cameraPos[1] = cameraPos[1] + 0.63421325;
	// }
	// if (IsKeyboardKeyDown(DOWN)) {
	// 	cameraPos[1] = cameraPos[1] - 0.6421498053;
	// }
}

void draw() {
	GameObject::DrawGameObjects();
	UI::DrawUI();
}

void handleInput() {
	if (IsKeyboardKeyJustPressed(B)) {
		Engine::LoadScene("forest1");
	}
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
int (*_handleEventFunc)(void*) = nullptr;
