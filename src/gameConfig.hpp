#pragma once
#include <vector>
#include <string>

namespace Etf {
struct Scene {
	std::string MapName, UIName, BGMName;
	float BGMVolume;
};

struct gameConfig {
	struct audio {
		float bgmVolume, sfxVolume;
	} audio;
	struct window {
		int x, y, xWin, yWin;
		std::string title;
	} window;
	struct scene {
		std::string defaultScene;
		std::vector<Scene> scenes;
	} scene;
	struct debug {
		bool interactions;
		bool mapExits;
	} debug;
};

struct GameConfig {
	static gameConfig& GetGameConfig() { return _config; }
	// Loads the game config, with the file path you send in.
	static void LoadGameConfig(const std::string& configFileName);

   private:
	static gameConfig _config;
};

}  // namespace Etf
