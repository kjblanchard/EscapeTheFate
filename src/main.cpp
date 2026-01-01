#include <Supergoon/Audio/Audio.h>
#include <Supergoon/Graphics/graphics.h>
#include <Supergoon/engine.h>
#include <Supergoon/log.h>
#include <Supergoon/window.h>

#include <glaze/glaze.hpp>
#include <string>

#include "bindings/engine.hpp"

namespace Etf {

void loadGameConfig() {
	auto& _gameConfig = Engine::Config;
	auto ec = glz::read_file_json(_gameConfig, "./assets/config/gameConfig.jsonc", std::string{});
	if (ec) {
		sgLogError("Error reading config file, please make sure it is in assets folder/gameConfig.jsonc!");
		return;
	}
}

void initialize() {
	loadGameConfig();
	auto& _gameConfig = Engine::Config;
	SetWindowOptions(_gameConfig.window.xWin, _gameConfig.window.yWin, _gameConfig.window.title.c_str());
	SetGlobalBgmVolume(_gameConfig.audio.bgmVolume);
	SetGlobalSfxVolume(_gameConfig.audio.sfxVolume);
}

void start() {
	auto& _gameConfig = Engine::Config;
	GraphicsSetLogicalWorldSize(_gameConfig.window.x, _gameConfig.window.y);
	Engine::LoadScene();
}

void update() {
}
}  // namespace Etf

void (*_initializeFunc)(void) = Etf::initialize;
void (*_startFunc)(void) = Etf::start;
void (*_updateFunc)(void) = Etf::update;
void (*_drawFunc)(void) = nullptr;
void (*_quitFunc)(void) = nullptr;
void (*_inputFunc)(void) = nullptr;
int (*_handleEventFunc)(void*) = nullptr;
