#include <Supergoon/Audio/Audio.h>
#include <Supergoon/log.h>
#include <Supergoon/map.h>

#include <algorithm>
#include <bindings/engine.hpp>
#include <gameConfig.hpp>
#include <gameobject/GameObject.hpp>

using namespace Etf;
using namespace std;

void Engine::LoadAndPlayBGM(const std::string& name, float volume) {
	SetBgmTrack(0);
	LoadBgm(name.c_str(), volume, -1);
	PlayBgm();
}
void Engine::LoadScene(const string& name) {
	auto newName = name;
	auto& gameSceneConfig = GameConfig::GetGameConfig().scene;
	if (newName == "") {
		newName = gameSceneConfig.defaultScene;
	}
	const auto it = std::find_if(gameSceneConfig.scenes.begin(), gameSceneConfig.scenes.end(), [&newName](Scene& scene) {
		return scene.MapName == newName;
	});
	if (it == gameSceneConfig.scenes.end()) {
		sgLogWarn("Could not find scene with name %s, not loading", name.c_str());
		return;
	}
	auto& sceneToLoad = *it;
	LoadMap(newName.c_str());
	GameObject::LoadAllGameObjects();
	LoadAndPlayBGM(sceneToLoad.BGMName, sceneToLoad.BGMVolume);
}
