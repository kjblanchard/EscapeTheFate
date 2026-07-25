#pragma once
#include <string>
#include <vector>

namespace Etf {
struct Scene {
	std::string MapName, UIName, BGMName;
	float BGMVolume;
};

struct gameConfig {
	static std::vector<std::string> PreloadTextures;
	std::vector<std::string> logos;
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
		bool battleZones;
		int debugLevel;
	} debug;
	struct controls {
		struct keyboard {
			static const int UP = 26;
			static const int LEFT = 4;
			static const int DOWN = 22;
			static const int RIGHT = 7;
			static const int A = 44;
			static const int B = 27;
			static const int LB = 18;
			static const int RB = 19;
		} Keyboard;
		struct joystick {
			static const int UP = 11;
			static const int LEFT = 13;
			static const int DOWN = 12;
			static const int RIGHT = 14;
			static const int A = 0;
			static const int B = 1;
			static const int X = 2;
			static const int Y = 3;
			static const int LB = 9;
			static const int RB = 10;
		} Joystick;
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
