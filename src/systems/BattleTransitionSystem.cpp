#include <Supergoon/Graphics/graphics.h>
#include <Supergoon/Graphics/shader.h>

#include <algorithm>
#include <cstdlib>
#include <engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <string>
#include <systems/BattleTransitionSystem.hpp>

using namespace Etf;
using namespace std;

static constexpr float TRANSITION_DURATION = 1.5f;

namespace {
Shader* swirlShader_ = nullptr;
Shader* blurShader_ = nullptr;
Shader* shatterShader_ = nullptr;
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

	swirlShader_ = ShaderCreate();
	ShaderCompile(swirlShader_, "2dScreenVertex", "battleSwirlFragment");
	blurShader_ = ShaderCreate();
	ShaderCompile(blurShader_, "2dScreenVertex", "battleBlurFragment");
	shatterShader_ = ShaderCreate();
	ShaderCompile(shatterShader_, "2dScreenVertex", "battleShatterFragment");
}

static void pickRandomTint(float& r, float& g, float& b) {
	// Palette of vivid battle tints — randomly pick one each transition
	static const float tints[][3] = {
		{2.0f, 0.4f, 0.3f},  // hot red
		{2.0f, 1.0f, 0.2f},  // fiery orange/yellow
		{0.3f, 0.5f, 2.0f},  // electric blue
		{1.8f, 0.2f, 1.5f},  // magenta/purple
		{0.2f, 2.0f, 0.8f},  // toxic green
		{1.5f, 0.3f, 2.0f},  // violet
		{2.0f, 0.6f, 1.0f},  // hot pink
		{0.4f, 1.8f, 2.0f},  // cyan
	};
	int idx = rand() % 8;
	r = tints[idx][0];
	g = tints[idx][1];
	b = tints[idx][2];
}

void BattleTransitionSystem::TriggerTransition(const string& battleScene) {
	if (transitioning_) return;
	pendingScene_ = battleScene;
	elapsed_ = 0.0f;
	transitioning_ = true;
	GameState::Battle::InBattle = true;
	int pick = rand() % 3;
	activeShader_ = pick == 0 ? swirlShader_ : (pick == 1 ? blurShader_ : shatterShader_);
	SetScreenShaderOverride(activeShader_);
	ShaderUse(activeShader_);
	ShaderSetUniformFloat(activeShader_, "time", 0.0f, false);
	ShaderSetUniformVector2f(activeShader_, "resolution", resX_, resY_, false);
	float tr, tg, tb;
	pickRandomTint(tr, tg, tb);
	ShaderSetUniformVector3f(activeShader_, "tintColor", tr, tg, tb, false);
	startBattleBGM(battleScene);
}

void BattleTransitionSystem::Update() {
	if (!transitioning_) return;
	elapsed_ += GameState::DeltaTimeSeconds;
	float t = elapsed_ / TRANSITION_DURATION;
	if (t > 1.0f) t = 1.0f;

	ShaderUse(activeShader_);
	ShaderSetUniformFloat(activeShader_, "time", t, false);
	ShaderSetUniformVector2f(activeShader_, "resolution", resX_, resY_, false);

	if (elapsed_ >= TRANSITION_DURATION) {
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
	if (swirlShader_) {
		ShaderDestroy(swirlShader_);
		swirlShader_ = nullptr;
	}
	if (blurShader_) {
		ShaderDestroy(blurShader_);
		blurShader_ = nullptr;
	}
	if (shatterShader_) {
		ShaderDestroy(shatterShader_);
		shatterShader_ = nullptr;
	}
}

bool BattleTransitionSystem::IsTransitioning() {
	return transitioning_;
}
