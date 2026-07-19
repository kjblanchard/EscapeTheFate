#include <Supergoon/Graphics/shader.h>

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
Shader* activeShader_ = nullptr;
float elapsed_ = 0.0f;
bool transitioning_ = false;
string pendingScene_ = "";
float resX_ = 480.0f;
float resY_ = 270.0f;
}  // namespace

void BattleTransitionSystem::Start() {
	auto& config = GameConfig::GetGameConfig();
	resX_ = (float)config.window.x;
	resY_ = (float)config.window.y;

	swirlShader_ = ShaderCreate();
	ShaderCompile(swirlShader_, "2dScreenVertex", "battleSwirlFragment");
	blurShader_ = ShaderCreate();
	ShaderCompile(blurShader_, "2dScreenVertex", "battleBlurFragment");
}

void BattleTransitionSystem::TriggerTransition(const string& battleScene) {
	if (transitioning_) return;
	pendingScene_ = battleScene;
	elapsed_ = 0.0f;
	transitioning_ = true;
	activeShader_ = (rand() % 2 == 0) ? swirlShader_ : blurShader_;
	SetScreenShaderOverride(activeShader_);
	ShaderUse(activeShader_);
	ShaderSetUniformFloat(activeShader_, "time", 0.0f, false);
	ShaderSetUniformVector2f(activeShader_, "resolution", resX_, resY_, false);
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
		SetScreenShaderOverride(nullptr);
		transitioning_ = false;
		activeShader_ = nullptr;
		elapsed_ = 0.0f;
		GameState::Battle::InBattle = true;
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
}

bool BattleTransitionSystem::IsTransitioning() {
	return transitioning_;
}
