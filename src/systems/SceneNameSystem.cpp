#include <engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <systems/SceneNameSystem.hpp>
#include <ui/ui.hpp>
#include <ui/uiNineSlice.hpp>
#include <ui/uiText.hpp>
#include <string>

using namespace Etf;
using namespace std;

enum class SceneNameState {
	Hidden,
	SlidingIn,
	Holding,
	SlidingOut,
};
using enum SceneNameState;

namespace {
constexpr float kSlideTime = 0.3f;
constexpr float kHoldDuration = 2.0f;
constexpr float kPanelRestY = 4.0f;
constexpr float kPanelHiddenY = -60.0f;

SceneNameState currentState_ = Hidden;
UIObject* panel_ = nullptr;
UIText* text_ = nullptr;
float animTime_ = 0.0f;
float holdTime_ = 0.0f;
string lastSceneName_;
}  // namespace

void SceneNameSystem::Start() {
	UINineSliceArgs nsArgs;
	nsArgs.Name = "SceneNameBanner";
	nsArgs.Filename = "uibase";
	nsArgs.Rect = {155.0f, kPanelHiddenY, 170.0f, 40.0f};
	nsArgs.SourceRect = {0, 0, 64, 64};
	nsArgs.DrawColor = {80, 0, 120, 235};
	nsArgs.Xoffset = 8;
	nsArgs.Yoffset = 8;
	nsArgs.Scale = 1.0f;
	nsArgs.Priority = 20;
	nsArgs.Visible = true;
	nsArgs.DebugBox = false;
	auto* banner = new UINineSlice(nsArgs);
	banner->SetDoNotDestroy(true);

	UITextArgs textArgs;
	textArgs.FontName = "PressStart2P";
	textArgs.FontSize = 8;
	textArgs.Rect = {6.0f, 6.0f, 156.0f, 28.0f};
	textArgs.TextToDraw = "";
	textArgs.Name = "SceneNameText";
	textArgs.NumCharsToDraw = 64;
	textArgs.Priority = 0;
	textArgs.TextColor = {255, 255, 255, 255};
	textArgs.CenteredX = true;
	textArgs.CenteredY = true;
	textArgs.WordWrap = false;
	textArgs.Visible = true;
	textArgs.DebugBox = false;

	auto* label = new UIText(textArgs);
	banner->AddChild(label);

	UI::GetRootUIObject()->AddChild(banner);

	panel_ = banner;
	text_ = label;
	lastSceneName_ = "";
}

void SceneNameSystem::Update() {
	auto& currentScene = Engine::CurrentSceneName();
	if (currentScene.empty() || currentScene == "cloud" || GameState::Battle::InBattle) return;

	if (currentScene != lastSceneName_) {
		lastSceneName_ = currentScene;
		auto& scenes = GameConfig::GetGameConfig().scene.scenes;
		for (auto& s : scenes) {
			if (s.MapName == currentScene && !s.DisplayName.empty()) {
				text_->UpdateText(s.DisplayName);
				panel_->SetY(kPanelHiddenY);
				animTime_ = 0.0f;
				holdTime_ = 0.0f;
				currentState_ = SlidingIn;
				break;
			}
		}
		return;
	}

	switch (currentState_) {
		case Hidden:
			break;

		case SlidingIn: {
			animTime_ += GameState::DeltaTimeSeconds;
			if (animTime_ >= kSlideTime) {
				panel_->SetY(kPanelRestY);
				animTime_ = 0.0f;
				currentState_ = Holding;
				break;
			}
			auto newY = Engine::Tweening::GetTweenedValue(kPanelHiddenY, kPanelRestY, animTime_, kSlideTime, Engine::Tweening::TweenEaseTypes::QuintOut);
			panel_->SetY(newY);
			break;
		}

		case Holding: {
			holdTime_ += GameState::DeltaTimeSeconds;
			if (holdTime_ >= kHoldDuration) {
				holdTime_ = 0.0f;
				animTime_ = 0.0f;
				currentState_ = SlidingOut;
			}
			break;
		}

		case SlidingOut: {
			animTime_ += GameState::DeltaTimeSeconds;
			if (animTime_ >= kSlideTime) {
				panel_->SetY(kPanelHiddenY);
				currentState_ = Hidden;
				break;
			}
			auto newY = Engine::Tweening::GetTweenedValue(kPanelRestY, kPanelHiddenY, animTime_, kSlideTime, Engine::Tweening::TweenEaseTypes::QuintOut);
			panel_->SetY(newY);
			break;
		}
	}
}

void SceneNameSystem::End() {
	panel_->SetY(kPanelHiddenY);
	currentState_ = Hidden;
}
