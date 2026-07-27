#include <Supergoon/Graphics/graphics.h>
#include <Supergoon/camera.h>

#include <engine.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/BattleLocation.hpp>
#include <systems/GameOverSystem.hpp>
#include <systems/battleSystem.hpp>
#include <ui/ui.hpp>
#include <ui/uiObject.hpp>

using namespace Etf;

namespace {
enum class GameOverState {
	NotActive,
	FadingOut,
	ShowingGameOver,
	TransitionToTitle,
};

GameOverState state_ = GameOverState::NotActive;
float timer_ = 0.0f;
constexpr float kFadeOutDuration = 1.5f;
constexpr float kShowDuration = 2.5f;
UIObject* gameOverPanel_ = nullptr;

bool allPlayersDead() {
	auto& battlers = BattleSystem::GetEnemyBattlers();
	if (battlers.empty()) return false;
	bool hasPlayer = false;
	for (auto* b : battlers) {
		if (!b || !b->IsPlayer()) continue;
		hasPlayer = true;
		if (b->CurrentHP() > 0) return false;
	}
	return hasPlayer;
}

void cacheUI() {
	if (gameOverPanel_) return;
	auto* root = UI::GetRootUIObject();
	if (!root) return;
	gameOverPanel_ = root->GetChildByName("GameOverPanel");
}

}  // namespace

void GameOverSystem::Update() {
	if (!GameState::Battle::InBattle && state_ == GameOverState::NotActive) return;

	switch (state_) {
		case GameOverState::NotActive: {
			if (!allPlayersDead()) return;
			cacheUI();
			BattleSystem::TriggerGameOver();
			// Engine::Audio::StopBGM();
			// Engine::Audio::PlayBGM("gameover");
			timer_ = 0.0f;
			state_ = GameOverState::FadingOut;
			break;
		}
		case GameOverState::FadingOut: {
			timer_ += GameState::DeltaTimeSeconds;
			float t = timer_ / kFadeOutDuration;
			if (t > 1.0f) t = 1.0f;
			unsigned char alpha = (unsigned char)(255 * (1.0f - t));
			Color c = {255, 255, 255, alpha};
			GraphicsUpdateFBOColor(&c);
			if (timer_ >= kFadeOutDuration) {
				if (gameOverPanel_) gameOverPanel_->SetVisible(true);
				Color white = {255, 255, 255, 255};
				GraphicsUpdateFBOColor(&white);
				timer_ = 0.0f;
				state_ = GameOverState::ShowingGameOver;
			}
			break;
		}
		case GameOverState::ShowingGameOver: {
			timer_ += GameState::DeltaTimeSeconds;
			if (timer_ >= kShowDuration) {
				if (gameOverPanel_) gameOverPanel_->SetVisible(false);
				gameOverPanel_ = nullptr;
				BattleLocation::ClearAllBattleLocations();
				ResetCameraFollow();
				GameState::Battle::InBattle = false;
				GameState::Battle::ExitingFromBattle = false;
				Engine::LoadScene("cloud", 0.0f, 0.5f, false);
				state_ = GameOverState::TransitionToTitle;
			}
			break;
		}
		case GameOverState::TransitionToTitle: {
			state_ = GameOverState::NotActive;
			break;
		}
	}
}
