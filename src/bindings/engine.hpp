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
};
struct Engine {
   public:
	static void LoadScene(const std::string& name = "");
	static gameConfig Config;

   private:
	static void LoadAndPlayBGM(const std::string& name, float volume);
};
}  // namespace Etf
