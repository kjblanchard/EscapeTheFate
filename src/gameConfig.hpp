#pragma once
#include <string>
#include <vector>

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
		int debugLevel;
	} debug;
	struct controls {
		static const int UP = 26;
		static const int LEFT = 4;
		static const int DOWN = 22;
		static const int RIGHT = 7;
		static const int A = 44;
		static const int B = 27;
	} Controls;
};

struct GameConfig {
	static gameConfig& GetGameConfig() { return _config; }
	// Loads the game config, with the file path you send in.
	static void LoadGameConfig(const std::string& configFileName);

   private:
	static gameConfig _config;
};

}  // namespace Etf
