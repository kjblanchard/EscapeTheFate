#include <Supergoon/Audio/Audio.h>
#include <Supergoon/log.h>
#include <Supergoon/map.h>

#include <algorithm>
#include <bindings/engine.hpp>

using namespace Etf;
using namespace std;

gameConfig Engine::Config;

void Engine::LoadAndPlayBGM(const std::string& name, float volume) {
	SetBgmTrack(0);
	LoadBgm(name.c_str(), volume, -1);
	PlayBgm();
}
void Engine::LoadScene(const string& name) {
	auto newName = name;
	if (newName == "") {
		newName = Config.scene.defaultScene;
	}
	const auto it = std::find_if(Config.scene.scenes.begin(), Config.scene.scenes.end(), [&newName](Scene& scene) {
		return scene.MapName == newName;
	});
	if (it == Config.scene.scenes.end()) {
		sgLogWarn("Could not find scene with name %s, not loading", name.c_str());
		return;
	}
	auto& sceneToLoad = *it;
	// Load the Tiled map
	LoadMap(newName.c_str());
	// Load the Objects
	//
	// Load the Music
	LoadAndPlayBGM(sceneToLoad.BGMName, sceneToLoad.BGMVolume);
}
