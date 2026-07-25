#include <Supergoon/filesystem.h>
#include <Supergoon/json.h>
#include <sgtools/log.h>

#include <engine.hpp>
#include <gameConfig.hpp>
#include <string>

using namespace std;
using namespace Etf;

vector<string> Etf::gameConfig::PreloadTextures = {};

gameConfig GameConfig::_config;
const vector<string> preloadTextures_ = {
	"atbBar",
	"blackBird",
	"house",
	"interaction",
	"outside",
	"player1Battler",
	"supergoonLogo",
	"terrain",
	"battleTurnMarker",
	"fingers",
	"icons_tf_16",
	"keyboard",
	"padButtonsExport",
	"player2",
	"sword1",
	"uibase",
	"bBlackBird",
	"gamecontroller",
	"inside",
	"null",
	"player1",
	"supergoongameslogo",
	"terrain",
};

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


static void loadLogosToConfig(gameConfig* config, json_object* rootObject) {
	auto logosObj = jobj(rootObject, "logos");
	if (!logosObj) return;
	auto numLogos = jGetObjectArrayLength(logosObj);
	for (int i = 0; i < numLogos; ++i) {
		auto name = jstrIndex(logosObj, i);
		if (name) config->logos.emplace_back(name);
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
	char* buf;
	size_t sz;
	Engine::Json::GetJsonBufferFromDirectory("gameConfig", &buf, &sz);
	auto root = jGetObjectFromBuffer(buf, sz);
	if (!root) {
		sgLogCritical("Error reading game config file!");
	}
	loadAudioSettingsToConfig(&_config, root);
	loadWindowSettingsToConfig(&_config, root);
	loadSceneSettingsToConfig(&_config, root);
	loadLogosToConfig(&_config, root);
	jReleaseObjectFromFile(root);
	_config.PreloadTextures = preloadTextures_;
}
