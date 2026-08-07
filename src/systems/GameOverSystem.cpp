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
using namespace std;

namespace {

enum class GameOverState {
	NotActive,
	FadingOut,
	ShowingGameOver,
	WaitingForSceneLoad,
};
using enum GameOverState;

constexpr float kFadeOutDuration = 1.75f;
constexpr float kShowDuration = 5.0f;
constexpr const char* kTitlePanelName = "TitleNineSlice";
constexpr const char* kMenuPanelName = "MenuNineSlice";

GameOverState state_ = NotActive;
float timer_ = 0.0f;
UIObject* gameOverPanel_ = nullptr;

bool allPlayersDead() {
	auto& battlers = BattleSystem::GetEnemyBattlers();
	if (battlers.empty()) return false;
	bool hasPlayer = false;
	for (auto* battler : battlers) {
		if (!battler || !battler->IsPlayer()) continue;
		hasPlayer = true;
		if (battler->CurrentHP() > 0) return false;
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
	if (!GameState::Battle::InBattle && state_ == NotActive) return;
	switch (state_) {
		case NotActive: {
			if (!allPlayersDead()) return;
			cacheUI();
			BattleSystem::TriggerGameOver();
			timer_ = 0.0f;
			state_ = FadingOut;
			break;
		}
		case FadingOut: {
			timer_ += GameState::DeltaTimeSeconds;
			float progress = timer_ / kFadeOutDuration;
			if (progress > 1.0f) progress = 1.0f;
			unsigned char alpha = (unsigned char)(255 * (1.0f - progress));
			Color c = {255, 255, 255, alpha};
			GraphicsUpdateFBOColor(&c);
			if (timer_ >= kFadeOutDuration) {
				if (gameOverPanel_) gameOverPanel_->SetVisible(true);
				Color white = {255, 255, 255, 255};
				GraphicsUpdateFBOColor(&white);
				timer_ = 0.0f;
				Engine::Audio::PlayBGM("gameover", 1.0f, 0);
				state_ = ShowingGameOver;
			}
			break;
		}
		case ShowingGameOver: {
			timer_ += GameState::DeltaTimeSeconds;
			if (timer_ >= kShowDuration) {
				if (gameOverPanel_) gameOverPanel_->SetVisible(false);
				Color black = {255, 255, 255, 0};
				GraphicsUpdateFBOColor(&black);
				BattleLocation::ClearAllBattleLocations();
				ResetCameraFollow();
				GameState::Battle::InBattle = false;
				GameState::Battle::ExitingFromBattle = false;
				Engine::LoadScene("cloud", 0.0f, 0.75f, false);
				state_ = WaitingForSceneLoad;
			}
			break;
		}
		case WaitingForSceneLoad: {
			if (Engine::CurrentSceneName() == "cloud") {
				auto* titlePanel = UI::GetRootUIObject()->GetChildByName(kTitlePanelName);
				auto* menuPanel = UI::GetRootUIObject()->GetChildByName(kMenuPanelName);
				if (titlePanel) titlePanel->SetVisible(true);
				if (menuPanel) menuPanel->SetVisible(true);
				BattleSystem::ResetAfterGameOver();
				if (gameOverPanel_) gameOverPanel_->SetVisible(false);
				gameOverPanel_ = nullptr;
				state_ = NotActive;
			}
			break;
		}
	}
}
