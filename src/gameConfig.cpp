#include <Supergoon/filesystem.h>
#include <Supergoon/json.h>
#include <Supergoon/log.h>

#include <bindings/engine.hpp>
#include <format>
#include <gameConfig.hpp>
#include <string>

using namespace std;
using namespace Etf;

gameConfig GameConfig::_config;

static void loadAudioSettingsToConfig(gameConfig* config, json_object* rootObject) {
	auto audioObject = jobj(rootObject, "audio");
	if (!audioObject) {
		sgLogWarn("Could not load audio settings, setting defaults");
		return;
	}
	config->audio.bgmVolume = jfloat(audioObject, "bgmVolume");
	config->audio.sfxVolume = jfloat(audioObject, "sfxVolume");
}

static void loadWindowSettingsToConfig(gameConfig* config, json_object* rootObject) {
	auto windowObject = jobj(rootObject, "window");
	if (!windowObject) {
		sgLogWarn("Could not load window settings, setting defaults");
		return;
	}
	config->window.x = jint(windowObject, "x");
	config->window.y = jint(windowObject, "y");
	config->window.xWin = jint(windowObject, "xWin");
	config->window.yWin = jint(windowObject, "yWin");
	config->window.title = jstr(windowObject, "title");
}

static void loadDebugSettingsToConfig(gameConfig* config, json_object* rootObject) {
	auto debugObj = jobj(rootObject, "debug");
	if (!debugObj) {
		sgLogWarn("Could not load debug settings, setting defaults");
		return;
	}
	config->debug.interactions = jbool(debugObj, "interactions");
	config->debug.mapExits = jbool(debugObj, "mapExits");
	auto debugLogLevelChar = jstr(debugObj, "logLevel");
	if (!debugLogLevelChar)
		config->debug.debugLevel = Log_LError;
	else {
		auto debugLevelString = string_view(debugLogLevelChar);
		if (debugLevelString.starts_with('w'))
			config->debug.debugLevel = Log_LWarn;
		else if (debugLevelString.starts_with('d'))
			config->debug.debugLevel = Log_LDebug;
		else if (debugLevelString.starts_with('i'))
			config->debug.debugLevel = Log_LInfo;
	}
}

static void loadSceneSettingsToConfig(gameConfig* config, json_object* rootObject) {
	auto sceneObj = jobj(rootObject, "scene");
	if (!sceneObj) {
		sgLogCritical("Could not load scenes, this is required to run");
		return;
	}
	config->scene.defaultScene = jstr(sceneObj, "defaultScene");
	auto scenesObj = jobj(sceneObj, "scenes");
	if (!scenesObj) {
		sgLogCritical("Could not load scenes, this is required to run");
		return;
	}
	auto numScenes = jGetObjectArrayLength(scenesObj);
	for (auto i = 0; i < numScenes; ++i) {
		auto currentSceneObj = jGetObjectInObjectWithIndex(scenesObj, i);
		if (!currentSceneObj) {
			sgLogWarn("Could not get array obj in scene??");
			continue;
		}
		config->scene.scenes.emplace_back();
		config->scene.scenes.back().BGMName = jstr(currentSceneObj, "BGMName");
		config->scene.scenes.back().MapName = jstr(currentSceneObj, "MapName");
		config->scene.scenes.back().UIName = jstr(currentSceneObj, "UIName");
		config->scene.scenes.back().BGMVolume = jfloat(currentSceneObj, "BGMVolume");
	}
}

void GameConfig::LoadGameConfig(const std::string& configFileName) {
	auto loadString = format("{}{}", GetBasePath(), configFileName);
	auto root = jGetObjectFromFile(loadString.c_str());
	if (!root) {
		sgLogCritical("Error reading config file, please make sure it is in assets folder/gameConfig.jsonc!");
	}
	loadAudioSettingsToConfig(&_config, root);
	loadWindowSettingsToConfig(&_config, root);
	loadDebugSettingsToConfig(&_config, root);
	loadSceneSettingsToConfig(&_config, root);
	jReleaseObjectFromFile(root);
}
