#include <Supergoon/Audio/Audio.h>
#include <Supergoon/Graphics/graphics.h>
#include <Supergoon/engine.h>
#include <Supergoon/log.h>
#include <Supergoon/state.h>
#include <Supergoon/window.h>

#include <bindings/engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <glaze/glaze.hpp>
#include <gameobject/GameObject.hpp>
#include <ui/uiObject.hpp>

namespace Etf {

void initialize() {
	GameConfig::LoadGameConfig("./assets/config/gameConfig.jsonc");
	auto& _gameConfig = GameConfig::GetGameConfig();
	SetWindowOptions(_gameConfig.window.xWin, _gameConfig.window.yWin, _gameConfig.window.title.c_str());
	SetGlobalBgmVolume(_gameConfig.audio.bgmVolume);
	SetGlobalSfxVolume(_gameConfig.audio.sfxVolume);
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
	GameObject::UpdateGameObjects();
}

void draw() {
	GameObject::DrawGameObjects();
	UIObject::DrawUI();
}
}  // namespace Etf

void (*_initializeFunc)(void) = Etf::initialize;
void (*_startFunc)(void) = Etf::start;
void (*_updateFunc)(void) = Etf::update;
void (*_drawFunc)(void) = Etf::draw;
void (*_quitFunc)(void) = nullptr;
void (*_inputFunc)(void) = nullptr;
int (*_handleEventFunc)(void*) = nullptr;
