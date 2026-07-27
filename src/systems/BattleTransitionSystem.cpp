#include <Supergoon/Graphics/graphics.h>
#include <Supergoon/Graphics/shader.h>

#include <algorithm>
#include <cstdlib>
#include <engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <string>
#include <systems/BattleTransitionSystem.hpp>

#include "systems/SceneNameSystem.hpp"

using namespace Etf;
using namespace std;

namespace {
constexpr float transitionDuration_ = 0.5f;
Shader* blurShader_ = nullptr;
Shader* activeShader_ = nullptr;
float elapsed_ = 0.0f;
bool transitioning_ = false;
string pendingScene_ = "";
float resX_ = 480.0f;
float resY_ = 270.0f;
}  // namespace

static void startBattleBGM(const string& sceneName) {
	auto& scenes = GameConfig::GetGameConfig().scene.scenes;
	auto it = find_if(scenes.begin(), scenes.end(), [&](const Scene& s) {
		return s.MapName == sceneName;
	});
	if (it != scenes.end() && !it->BGMName.empty()) {
		Engine::Audio::PlayBGM(it->BGMName, it->BGMVolume);
	}
}

void BattleTransitionSystem::Start() {
	auto& config = GameConfig::GetGameConfig();
	resX_ = (float)config.window.x;
	resY_ = (float)config.window.y;
	blurShader_ = ShaderCreate();
	ShaderCompile(blurShader_, "2dScreenVertex", "battleBlurFragment");
}

void BattleTransitionSystem::TriggerTransition(const string& battleScene) {
	if (transitioning_) return;
	pendingScene_ = battleScene;
	elapsed_ = 0.0f;
	transitioning_ = true;
	GameState::Battle::InBattle = true;
	activeShader_ = blurShader_;
	SetScreenShaderOverride(activeShader_);
	ShaderUse(activeShader_);
	ShaderSetUniformFloat(activeShader_, "time", 0.0f, false);
	ShaderSetUniformVector2f(activeShader_, "resolution", resX_, resY_, false);
	startBattleBGM(battleScene);
	SceneNameSystem::End();
}

void BattleTransitionSystem::Update() {
	if (!transitioning_) return;
	elapsed_ += GameState::DeltaTimeSeconds;
	float t = elapsed_ / transitionDuration_;
	if (t > 1.0f) t = 1.0f;
	ShaderUse(activeShader_);
	ShaderSetUniformFloat(activeShader_, "time", t, false);
	ShaderSetUniformVector2f(activeShader_, "resolution", resX_, resY_, false);
	if (elapsed_ >= transitionDuration_) {
		// Keep RGB white but zero alpha so screen is black; fade-in tweens alpha back to 255
		Color black = {255, 255, 255, 0};
		GraphicsUpdateFBOColor(&black);
		SetScreenShaderOverride(nullptr);
		transitioning_ = false;
		activeShader_ = nullptr;
		elapsed_ = 0.0f;
		Engine::LoadScene(pendingScene_, 0.0f, 2.75f, false);
		pendingScene_ = "";
	}
}

void BattleTransitionSystem::Shutdown() {
	SetScreenShaderOverride(nullptr);
	if (blurShader_) {
		ShaderDestroy(blurShader_);
		blurShader_ = nullptr;
	}
}

bool BattleTransitionSystem::IsTransitioning() {
	return transitioning_;
}
