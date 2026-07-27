#include <engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <systems/SceneNameSystem.hpp>
#include <ui/ui.hpp>
#include <ui/uiNineSlice.hpp>
#include <ui/uiText.hpp>

using namespace Etf;
using namespace std;

enum class SceneNameState {
	Hidden,
	SlidingIn,
	Holding,
	SlidingOut,
};

static const float Slide_Time = 0.3f;
static const float Hold_Duration = 2.0f;
static const float Panel_Rest_Y = 4.0f;
static const float Panel_Hidden_Y = -60.0f;

static SceneNameState _currentState = SceneNameState::Hidden;
static UIObject* _panel = nullptr;
static UIText* _text = nullptr;
static float _animTime = 0.0f;
static float _holdTime = 0.0f;
static string _lastSceneName;

void SceneNameSystem::Start() {
	UINineSliceArgs nsArgs;
	nsArgs.Name = "SceneNameBanner";
	nsArgs.Filename = "uibase";
	nsArgs.Rect = {155.0f, Panel_Hidden_Y, 170.0f, 40.0f};
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

	_panel = banner;
	_text = label;
	_lastSceneName = "";
}

void SceneNameSystem::Update() {
	auto& currentScene = Engine::CurrentSceneName();
	if (currentScene.empty() || currentScene == "cloud" || GameState::Battle::InBattle) return;

	if (currentScene != _lastSceneName) {
		_lastSceneName = currentScene;
		auto& scenes = GameConfig::GetGameConfig().scene.scenes;
		for (auto& s : scenes) {
			if (s.MapName == currentScene && !s.DisplayName.empty()) {
				_text->UpdateText(s.DisplayName);
				_panel->SetY(Panel_Hidden_Y);
				_animTime = 0.0f;
				_holdTime = 0.0f;
				_currentState = SceneNameState::SlidingIn;
				break;
			}
		}
		return;
	}

	switch (_currentState) {
		case SceneNameState::Hidden:
			break;

		case SceneNameState::SlidingIn: {
			_animTime += GameState::DeltaTimeSeconds;
			if (_animTime >= Slide_Time) {
				_panel->SetY(Panel_Rest_Y);
				_animTime = 0.0f;
				_currentState = SceneNameState::Holding;
				break;
			}
			auto newY = Engine::Tweening::GetTweenedValue(Panel_Hidden_Y, Panel_Rest_Y, _animTime, Slide_Time, Engine::Tweening::TweenEaseTypes::QuintOut);
			_panel->SetY(newY);
			break;
		}

		case SceneNameState::Holding: {
			_holdTime += GameState::DeltaTimeSeconds;
			if (_holdTime >= Hold_Duration) {
				_holdTime = 0.0f;
				_animTime = 0.0f;
				_currentState = SceneNameState::SlidingOut;
			}
			break;
		}

		case SceneNameState::SlidingOut: {
			_animTime += GameState::DeltaTimeSeconds;
			if (_animTime >= Slide_Time) {
				_panel->SetY(Panel_Hidden_Y);
				_currentState = SceneNameState::Hidden;
				break;
			}
			auto newY = Engine::Tweening::GetTweenedValue(Panel_Rest_Y, Panel_Hidden_Y, _animTime, Slide_Time, Engine::Tweening::TweenEaseTypes::QuintOut);
			_panel->SetY(newY);
			break;
		}
	}
}

void SceneNameSystem::End() {
	_panel->SetY(Panel_Hidden_Y);
	_currentState = SceneNameState::Hidden;
}
