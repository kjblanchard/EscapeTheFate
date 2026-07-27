#include <Supergoon/Input/keyboard.h>

#include <engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <systems/BattleSpoilsSystem.hpp>
#include <systems/battleSystem.hpp>
#include <ui/ui.hpp>
#include <ui/uiImage.hpp>
#include <ui/uiNineSlice.hpp>
#include <ui/uiObject.hpp>
#include <ui/uiProgressBar.hpp>
#include <ui/uiText.hpp>

using namespace Etf;
using namespace std;

enum class SpoilsState {
	Inactive,
	AnimatingIn,
	WaitingForInput,
	Accumulating,
	Done,
};

static const float BACKDROP_ANIM_TIME = 0.50f;
static const float INFO_ANIM_TIME = 0.35f;
static const float PORTRAIT_ANIM_TIME = 0.45f;
static const float ACCUM_DURATION = 1.5f;
static const float TICK_INTERVAL = 0.1f;

static const float BACKDROP_REST_X = 40.0f;
static const float BACKDROP_REST_Y = 15.0f;
static const float BACKDROP_START_Y = -270.0f;
static const float BACKDROP_W = 400.0f;
static const float BACKDROP_H = 240.0f;

static const float INFO_REST_X = 10.0f;
static const float INFO_REST_Y = 30.0f;
static const float INFO_START_X = 440.0f;
static const float INFO_W = 260.0f;
static const float INFO_H = 195.0f;

static const float PORTRAIT_REST_X = 280.0f;
static const float PORTRAIT_REST_Y = 30.0f;
static const float PORTRAIT_START_X = -130.0f;
static const float PORTRAIT_W = 110.0f;
static const float PORTRAIT_H = 195.0f;

static const int MAX_PLAYERS = 3;

static SpoilsState _currentState = SpoilsState::Inactive;
static bool _isDone = false;
static float _animTime = 0.0f;
static float _accumTime = 0.0f;
static float _tickTimer = 0.0f;
static int _xpGained = 0;

static UIObject* _spoilsRoot = nullptr;
static UINineSlice* _backdrop = nullptr;
static UINineSlice* _infoPanel = nullptr;
static UINineSlice* _portraitPanel = nullptr;
static UIText* _titleText = nullptr;
static UIText* _promptText = nullptr;

struct PlayerSpoilsRow {
	UIText* nameText = nullptr;
	UIText* xpText = nullptr;
	UIProgressBar* xpBar = nullptr;
	int xpToNext = 100;
	int startXP = 0;
};
static PlayerSpoilsRow _playerRows[MAX_PLAYERS];
static int _numPlayers = 0;

static void buildUI() {
	auto root = UI::GetRootUIObject();

	UIObjectArgs rootArgs;
	rootArgs.Rect = {0, 0, 480, 270};
	rootArgs.Visible = true;
	rootArgs.Name = "SpoilsRoot";
	rootArgs.Priority = 10;
	_spoilsRoot = new UIObject(rootArgs);
	root->AddChild(_spoilsRoot);

	UINineSliceArgs backdropArgs;
	backdropArgs.Filename = "uibase";
	backdropArgs.Name = "SpoilsBackdrop";
	backdropArgs.Rect = {BACKDROP_REST_X, BACKDROP_START_Y, BACKDROP_W, BACKDROP_H};
	backdropArgs.SourceRect = {0, 0, 64, 64};
	backdropArgs.Xoffset = 8;
	backdropArgs.Yoffset = 8;
	backdropArgs.Scale = 1.0f;
	backdropArgs.DrawColor = {20, 0, 40, 255};
	backdropArgs.Priority = 0;
	backdropArgs.Visible = true;
	_backdrop = new UINineSlice(backdropArgs);
	_spoilsRoot->AddChild(_backdrop);

	UITextArgs titleArgs;
	titleArgs.FontName = "PressStart2P";
	titleArgs.FontSize = 8;
	titleArgs.Rect = {10, 8, BACKDROP_W - 20, 16};
	titleArgs.TextToDraw = "BATTLE SPOILS";
	titleArgs.Name = "SpoilsTitleText";
	titleArgs.NumCharsToDraw = 100;
	titleArgs.Priority = 1;
	titleArgs.TextColor = {255, 255, 255, 255};
	titleArgs.CenteredX = true;
	titleArgs.CenteredY = false;
	titleArgs.WordWrap = false;
	titleArgs.Visible = true;
	titleArgs.DebugBox = false;
	_titleText = new UIText(titleArgs);
	_backdrop->AddChild(_titleText);

	UINineSliceArgs infoArgs;
	infoArgs.Filename = "uibase";
	infoArgs.Name = "SpoilsInfoPanel";
	infoArgs.Rect = {INFO_START_X, INFO_REST_Y, INFO_W, INFO_H};
	infoArgs.SourceRect = {0, 0, 64, 64};
	infoArgs.Xoffset = 8;
	infoArgs.Yoffset = 8;
	infoArgs.Scale = 1.0f;
	infoArgs.DrawColor = {80, 0, 120, 200};
	infoArgs.Priority = 1;
	infoArgs.Visible = true;
	_infoPanel = new UINineSlice(infoArgs);
	_backdrop->AddChild(_infoPanel);

	auto& battlers = BattleSystem::GetEnemyBattlers();
	_numPlayers = 0;
	for (size_t i = 0; i < battlers.size() && _numPlayers < MAX_PLAYERS; ++i) {
		if (!battlers[i] || !battlers[i]->IsPlayer()) continue;
		auto* battler = battlers[i];
		int row = _numPlayers;
		float rowY = 10.0f + row * 55.0f;

		UIObjectArgs playerContainerArgs;
		playerContainerArgs.Rect = {0, rowY, INFO_W - 20, 50};
		playerContainerArgs.Visible = true;
		playerContainerArgs.Name = "SpoilsPlayerContainer" + to_string(row);
		playerContainerArgs.Priority = 1;
		auto* playerContainer = new UIObject(playerContainerArgs);
		_infoPanel->AddChild(playerContainer);

		UITextArgs nameArgs;
		nameArgs.FontName = "PressStart2P";
		nameArgs.FontSize = 8;
		nameArgs.Rect = {10, 0, 200, 12};
		nameArgs.TextToDraw = battler->Name();
		nameArgs.Name = "SpoilsPlayerName" + to_string(row);
		nameArgs.NumCharsToDraw = 100;
		nameArgs.Priority = 1;
		nameArgs.TextColor = {255, 255, 255, 255};
		nameArgs.CenteredX = false;
		nameArgs.CenteredY = false;
		nameArgs.WordWrap = false;
		nameArgs.Visible = true;
		nameArgs.DebugBox = false;
		_playerRows[row].nameText = new UIText(nameArgs);
		playerContainer->AddChild(_playerRows[row].nameText);

		UITextArgs xpArgs;
		xpArgs.FontName = "PressStart2P";
		xpArgs.FontSize = 8;
		xpArgs.Rect = {10, 14, 220, 12};
		xpArgs.TextToDraw = "XP: 0 / " + to_string(battler->GetBattlerData()->XPToNextLevel);
		xpArgs.Name = "SpoilsXPText" + to_string(row);
		xpArgs.NumCharsToDraw = 100;
		xpArgs.Priority = 1;
		xpArgs.TextColor = {200, 200, 255, 255};
		xpArgs.CenteredX = false;
		xpArgs.CenteredY = false;
		xpArgs.WordWrap = false;
		xpArgs.Visible = true;
		xpArgs.DebugBox = false;
		_playerRows[row].xpText = new UIText(xpArgs);
		playerContainer->AddChild(_playerRows[row].xpText);

		UIProgressBarArgs barArgs;
		barArgs.Name = "SpoilsXPBar" + to_string(row);
		barArgs.Rect = {10, 30, 220, 8};
		barArgs.BarRect = {0, 0, 220, 8};
		barArgs.BarColor = {100, 200, 255, 255};
		barArgs.BackgroundColor = {30, 30, 60, 200};
		barArgs.Priority = 1;
		barArgs.Visible = true;
		_playerRows[row].xpBar = new UIProgressBar(barArgs);
		playerContainer->AddChild(_playerRows[row].xpBar);

		_playerRows[row].xpToNext = battler->GetBattlerData()->XPToNextLevel;
		_playerRows[row].startXP = battler->GetBattlerData()->CurrentXP;
		++_numPlayers;
	}

	UITextArgs promptArgs;
	promptArgs.FontName = "PressStart2P";
	promptArgs.FontSize = 8;
	promptArgs.Rect = {10, INFO_H - 20, INFO_W - 20, 12};
	promptArgs.TextToDraw = "Press A to collect";
	promptArgs.Name = "SpoilsPromptText";
	promptArgs.NumCharsToDraw = 100;
	promptArgs.Priority = 1;
	promptArgs.TextColor = {200, 255, 200, 255};
	promptArgs.CenteredX = true;
	promptArgs.CenteredY = false;
	promptArgs.WordWrap = false;
	promptArgs.Visible = true;
	promptArgs.DebugBox = false;
	_promptText = new UIText(promptArgs);
	_infoPanel->AddChild(_promptText);

	UINineSliceArgs portraitArgs;
	portraitArgs.Filename = "uibase";
	portraitArgs.Name = "SpoilsPortraitPanel";
	portraitArgs.Rect = {PORTRAIT_START_X, PORTRAIT_REST_Y, PORTRAIT_W, PORTRAIT_H};
	portraitArgs.SourceRect = {0, 0, 64, 64};
	portraitArgs.Xoffset = 8;
	portraitArgs.Yoffset = 8;
	portraitArgs.Scale = 1.0f;
	portraitArgs.DrawColor = {60, 0, 100, 200};
	portraitArgs.Priority = 1;
	portraitArgs.Visible = true;
	_portraitPanel = new UINineSlice(portraitArgs);
	_backdrop->AddChild(_portraitPanel);

	int portraitCount = 0;
	for (size_t i = 0; i < battlers.size() && portraitCount < MAX_PLAYERS; ++i) {
		if (!battlers[i] || !battlers[i]->IsPlayer()) continue;
		UIImageArgs imgArgs;
		imgArgs.Filename = "null";
		imgArgs.Name = "SpoilsPortrait" + to_string(portraitCount);
		imgArgs.Rect = {20, 10.0f + portraitCount * 60.0f, 64, 64};
		imgArgs.SourceRect = {0, 0, 64, 64};
		imgArgs.Scale = 1.0f;
		imgArgs.DrawColor = {255, 255, 255, 255};
		imgArgs.Priority = 1;
		imgArgs.Visible = true;
		auto* img = new UIImage(imgArgs);
		_portraitPanel->AddChild(img);
		++portraitCount;
	}
}

static void calculateXP() {
	_xpGained = 0;
	auto& battlers = BattleSystem::GetEnemyBattlers();
	for (size_t i = 0; i < battlers.size(); ++i) {
		if (!battlers[i] || battlers[i]->IsPlayer()) continue;
		if (battlers[i]->CurrentHP() == 0) {
			_xpGained += battlers[i]->GetBattlerData()->XPReward;
		}
	}
}

static void updateAnimatingIn() {
	_animTime += GameState::DeltaTimeSeconds;

	float backdropY = Engine::Tweening::GetTweenedValue(
		BACKDROP_START_Y, BACKDROP_REST_Y,
		_animTime, BACKDROP_ANIM_TIME,
		Engine::Tweening::TweenEaseTypes::QuintOut);
	_backdrop->SetY(backdropY);

	float infoX = Engine::Tweening::GetTweenedValue(
		INFO_START_X, INFO_REST_X,
		_animTime, INFO_ANIM_TIME,
		Engine::Tweening::TweenEaseTypes::QuintOut);
	_infoPanel->SetX(infoX);

	float portraitX = Engine::Tweening::GetTweenedValue(
		PORTRAIT_START_X, PORTRAIT_REST_X,
		_animTime, PORTRAIT_ANIM_TIME,
		Engine::Tweening::TweenEaseTypes::QuintOut);
	_portraitPanel->SetX(portraitX);

	float longestAnim = BACKDROP_ANIM_TIME;
	if (_animTime >= longestAnim) {
		_backdrop->SetY(BACKDROP_REST_Y);
		_infoPanel->SetX(INFO_REST_X);
		_portraitPanel->SetX(PORTRAIT_REST_X);
		_currentState = SpoilsState::WaitingForInput;
	}
}

static void updateAccumulating() {
	_accumTime += GameState::DeltaTimeSeconds;
	_tickTimer += GameState::DeltaTimeSeconds;

	if (_tickTimer >= TICK_INTERVAL) {
		_tickTimer -= TICK_INTERVAL;
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
	}

	float t = _accumTime;
	if (t > ACCUM_DURATION) t = ACCUM_DURATION;

	int displayed = (int)Engine::Tweening::GetTweenedValue(
		0.0f, (float)_xpGained,
		t, ACCUM_DURATION,
		Engine::Tweening::TweenEaseTypes::Linear);

	for (int i = 0; i < _numPlayers; ++i) {
		int totalXP = _playerRows[i].startXP + displayed;
		int xpToNext = _playerRows[i].xpToNext;
		string xpStr = "XP: " + to_string(totalXP) + " / " + to_string(xpToNext);
		_playerRows[i].xpText->UpdateText(xpStr);
		float percent = xpToNext > 0 ? (float)totalXP / (float)xpToNext * 100.0f : 0.0f;
		if (percent > 100.0f) percent = 100.0f;
		_playerRows[i].xpBar->SetBarPercent(percent);
	}

	if (_accumTime >= ACCUM_DURATION) {
		_currentState = SpoilsState::Done;
		_promptText->UpdateText("Press A to continue");
	}
}

static void skipAccumulation() {
	_accumTime = ACCUM_DURATION;
	for (int i = 0; i < _numPlayers; ++i) {
		int totalXP = _playerRows[i].startXP + _xpGained;
		int xpToNext = _playerRows[i].xpToNext;
		string xpStr = "XP: " + to_string(totalXP) + " / " + to_string(xpToNext);
		_playerRows[i].xpText->UpdateText(xpStr);
		float percent = xpToNext > 0 ? (float)totalXP / (float)xpToNext * 100.0f : 0.0f;
		if (percent > 100.0f) percent = 100.0f;
		_playerRows[i].xpBar->SetBarPercent(percent);
	}
	_currentState = SpoilsState::Done;
	_promptText->UpdateText("Press A to continue");
}

void BattleSpoilsSystem::TriggerBattleSpoils() {
	_isDone = false;
	_animTime = 0.0f;
	_accumTime = 0.0f;
	_tickTimer = 0.0f;
	_currentState = SpoilsState::AnimatingIn;
	calculateXP();
	buildUI();
}

bool BattleSpoilsSystem::IsBattleSpoilsDone() {
	return _isDone;
}

void BattleSpoilsSystem::Update() {
	if (_currentState == SpoilsState::Inactive) return;

	switch (_currentState) {
		case SpoilsState::AnimatingIn:
			updateAnimatingIn();
			break;
		case SpoilsState::WaitingForInput:
			if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.A)) {
				_currentState = SpoilsState::Accumulating;
				_promptText->UpdateText("");
			}
			break;
		case SpoilsState::Accumulating:
			if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.A)) {
				skipAccumulation();
			} else {
				updateAccumulating();
			}
			break;
		case SpoilsState::Done:
			if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.A)) {
				_isDone = true;
				_currentState = SpoilsState::Inactive;
			}
			break;
		default:
			break;
	}
}
