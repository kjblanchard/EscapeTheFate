#include <cstdlib>
#include <engine.hpp>
#include <gameState.hpp>
#include <systems/BattleSpoilsSystem.hpp>
#include <systems/PlayerControllerSystem.hpp>
#include <systems/battleSystem.hpp>
#include <types/ControllerButtons.hpp>
#include <ui/ui.hpp>
#include <ui/uiImage.hpp>
#include <ui/uiNineSlice.hpp>
#include <ui/uiObject.hpp>
#include <ui/uiProgressBar.hpp>
#include <ui/uiText.hpp>
#include <string>
#include <vector>

using namespace Etf;
using namespace std;

enum class SpoilsState {
	Inactive,
	AnimatingIn,
	WaitingForInput,
	Accumulating,
	Done,
};
using enum SpoilsState;

namespace {

constexpr float kBackdropAnimTime = 0.50f;
constexpr float kInfoAnimTime = 0.35f;
constexpr float kPortraitAnimTime = 0.45f;
constexpr float kAccumDuration = 1.5f;
constexpr float kTickInterval = 0.1f;

constexpr float kBackdropRestX = 40.0f;
constexpr float kBackdropRestY = 15.0f;
constexpr float kBackdropStartX = -420.0f;
constexpr float kBackdropW = 400.0f;
constexpr float kBackdropH = 240.0f;

constexpr float kInfoRestX = 10.0f;
constexpr float kInfoRestY = 30.0f;
constexpr float kInfoStartX = 440.0f;
constexpr float kInfoW = 260.0f;
constexpr float kInfoH = 195.0f;

constexpr float kPortraitRestX = 280.0f;
constexpr float kPortraitRestY = 30.0f;
constexpr float kPortraitStartX = -130.0f;
constexpr float kPortraitW = 110.0f;
constexpr float kPortraitH = 195.0f;

constexpr int kMaxPlayers = 3;

SpoilsState currentState_ = Inactive;
bool isDone_ = false;
float animTime_ = 0.0f;
float accumTime_ = 0.0f;
float tickTimer_ = 0.0f;
int xpGained_ = 0;

UIObject* spoilsRoot_ = nullptr;
UIProgressBar* solidBacking_ = nullptr;
UINineSlice* backdrop_ = nullptr;
UINineSlice* infoPanel_ = nullptr;
UINineSlice* portraitPanel_ = nullptr;
UIText* titleText_ = nullptr;
UIText* xpGainedText_ = nullptr;
UIText* promptText_ = nullptr;

struct PlayerSpoilsRow {
	UIText* nameText = nullptr;
	UIText* xpText = nullptr;
	UIProgressBar* xpBar = nullptr;
	int xpToNext = 100;
	int startXP = 0;
};
PlayerSpoilsRow playerRows_[kMaxPlayers];
int numPlayers_ = 0;
vector<string> wonItems_;

void buildUI() {
	auto root = UI::GetRootUIObject();

	UIObjectArgs rootArgs;
	rootArgs.Rect = {0, 0, 480, 270};
	rootArgs.Visible = true;
	rootArgs.Name = "SpoilsRoot";
	rootArgs.Priority = 10;
	spoilsRoot_ = new UIObject(rootArgs);
	root->AddChild(spoilsRoot_);

	UIProgressBarArgs backingArgs;
	backingArgs.Name = "SpoilsSolidBacking";
	backingArgs.Rect = {kBackdropStartX, kBackdropRestY, kBackdropW, kBackdropH};
	backingArgs.BarRect = {0, 0, kBackdropW, kBackdropH};
	backingArgs.BarColor = {40, 10, 60, 255};
	backingArgs.BackgroundColor = {40, 10, 60, 255};
	backingArgs.Priority = 0;
	backingArgs.Visible = true;
	solidBacking_ = new UIProgressBar(backingArgs);
	solidBacking_->SetBarPercent(100.0f);
	spoilsRoot_->AddChild(solidBacking_);

	UINineSliceArgs backdropArgs;
	backdropArgs.Filename = "uibase";
	backdropArgs.Name = "SpoilsBackdrop";
	backdropArgs.Rect = {kBackdropStartX, kBackdropRestY, kBackdropW, kBackdropH};
	backdropArgs.SourceRect = {0, 0, 64, 64};
	backdropArgs.Xoffset = 8;
	backdropArgs.Yoffset = 8;
	backdropArgs.Scale = 1.0f;
	backdropArgs.DrawColor = {40, 10, 60, 255};
	backdropArgs.Priority = 1;
	backdropArgs.Visible = true;
	backdrop_ = new UINineSlice(backdropArgs);
	spoilsRoot_->AddChild(backdrop_);

	UITextArgs titleArgs;
	titleArgs.FontName = "PressStart2P";
	titleArgs.FontSize = 8;
	titleArgs.Rect = {10, 8, kBackdropW - 20, 16};
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
	titleText_ = new UIText(titleArgs);
	backdrop_->AddChild(titleText_);

	UINineSliceArgs infoArgs;
	infoArgs.Filename = "uibase";
	infoArgs.Name = "SpoilsInfoPanel";
	infoArgs.Rect = {kInfoStartX, kInfoRestY, kInfoW, kInfoH};
	infoArgs.SourceRect = {0, 0, 64, 64};
	infoArgs.Xoffset = 8;
	infoArgs.Yoffset = 8;
	infoArgs.Scale = 1.0f;
	infoArgs.DrawColor = {80, 0, 120, 255};
	infoArgs.Priority = 1;
	infoArgs.Visible = true;
	infoPanel_ = new UINineSlice(infoArgs);
	backdrop_->AddChild(infoPanel_);

	UITextArgs xpGainedArgs;
	xpGainedArgs.FontName = "PressStart2P";
	xpGainedArgs.FontSize = 8;
	xpGainedArgs.Rect = {10, 10, kInfoW - 20, 12};
	xpGainedArgs.TextToDraw = "EXP Gained: " + to_string(xpGained_);
	xpGainedArgs.Name = "SpoilsXPGainedText";
	xpGainedArgs.NumCharsToDraw = 100;
	xpGainedArgs.Priority = 1;
	xpGainedArgs.TextColor = {255, 220, 100, 255};
	xpGainedArgs.CenteredX = false;
	xpGainedArgs.CenteredY = false;
	xpGainedArgs.WordWrap = false;
	xpGainedArgs.Visible = true;
	xpGainedArgs.DebugBox = false;
	xpGainedText_ = new UIText(xpGainedArgs);
	infoPanel_->AddChild(xpGainedText_);

	float itemsOffsetY = 0.0f;
	if (!wonItems_.empty()) {
		string itemStr = "Got: " + wonItems_[0];
		for (size_t idx = 1; idx < wonItems_.size(); ++idx) {
			itemStr += ", " + wonItems_[idx];
		}
		UITextArgs itemArgs;
		itemArgs.FontName = "PressStart2P";
		itemArgs.FontSize = 8;
		itemArgs.Rect = {10, 24.0f, kInfoW - 20, 12};
		itemArgs.TextToDraw = itemStr;
		itemArgs.Name = "SpoilsItemLine";
		itemArgs.NumCharsToDraw = 100;
		itemArgs.Priority = 1;
		itemArgs.TextColor = {150, 255, 150, 255};
		itemArgs.CenteredX = false;
		itemArgs.CenteredY = false;
		itemArgs.WordWrap = false;
		itemArgs.Visible = true;
		itemArgs.DebugBox = false;
		auto* itemText = new UIText(itemArgs);
		infoPanel_->AddChild(itemText);
		itemsOffsetY = 16.0f;
	}

	auto& battlers = BattleSystem::GetEnemyBattlers();
	numPlayers_ = 0;
	for (size_t i = 0; i < battlers.size() && numPlayers_ < kMaxPlayers; ++i) {
		if (!battlers[i] || !battlers[i]->IsPlayer()) continue;
		auto* battler = battlers[i];
		int row = numPlayers_;
		float rowY = 26.0f + itemsOffsetY + row * 38.0f;

		UIObjectArgs playerContainerArgs;
		playerContainerArgs.Rect = {0, rowY, kInfoW - 20, 36};
		playerContainerArgs.Visible = true;
		playerContainerArgs.Name = "SpoilsPlayerContainer" + to_string(row);
		playerContainerArgs.Priority = 1;
		auto* playerContainer = new UIObject(playerContainerArgs);
		infoPanel_->AddChild(playerContainer);

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
		playerRows_[row].nameText = new UIText(nameArgs);
		playerContainer->AddChild(playerRows_[row].nameText);

		UITextArgs xpArgs;
		xpArgs.FontName = "PressStart2P";
		xpArgs.FontSize = 8;
		xpArgs.Rect = {10, 13, 220, 12};
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
		playerRows_[row].xpText = new UIText(xpArgs);
		playerContainer->AddChild(playerRows_[row].xpText);

		UIProgressBarArgs barArgs;
		barArgs.Name = "SpoilsXPBar" + to_string(row);
		barArgs.Rect = {10, 27, 220, 6};
		barArgs.BarRect = {0, 0, 220, 6};
		barArgs.BarColor = {100, 200, 255, 255};
		barArgs.BackgroundColor = {30, 30, 60, 255};
		barArgs.Priority = 1;
		barArgs.Visible = true;
		playerRows_[row].xpBar = new UIProgressBar(barArgs);
		playerContainer->AddChild(playerRows_[row].xpBar);

		playerRows_[row].xpToNext = battler->GetBattlerData()->XPToNextLevel;
		playerRows_[row].startXP = battler->GetBattlerData()->CurrentXP;
		++numPlayers_;
	}

	UITextArgs promptArgs;
	promptArgs.FontName = "PressStart2P";
	promptArgs.FontSize = 8;
	promptArgs.Rect = {10, kInfoH - 16, kInfoW - 20, 12};
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
	promptText_ = new UIText(promptArgs);
	infoPanel_->AddChild(promptText_);

	UINineSliceArgs portraitArgs;
	portraitArgs.Filename = "uibase";
	portraitArgs.Name = "SpoilsPortraitPanel";
	portraitArgs.Rect = {kPortraitStartX, kPortraitRestY, kPortraitW, kPortraitH};
	portraitArgs.SourceRect = {0, 0, 64, 64};
	portraitArgs.Xoffset = 8;
	portraitArgs.Yoffset = 8;
	portraitArgs.Scale = 1.0f;
	portraitArgs.DrawColor = {60, 0, 100, 255};
	portraitArgs.Priority = 1;
	portraitArgs.Visible = true;
	portraitPanel_ = new UINineSlice(portraitArgs);
	backdrop_->AddChild(portraitPanel_);

	int portraitCount = 0;
	for (size_t i = 0; i < battlers.size() && portraitCount < kMaxPlayers; ++i) {
		if (!battlers[i] || !battlers[i]->IsPlayer()) continue;
		auto* data = battlers[i]->GetBattlerData();
		string portraitFile = data->Portrait;
		if (portraitFile.empty()) portraitFile = "null";
		auto& pr = data->PortraitRect;
		UIImageArgs imgArgs;
		imgArgs.Filename = portraitFile;
		imgArgs.Name = "SpoilsPortrait" + to_string(portraitCount);
		imgArgs.Rect = {20, 10.0f + portraitCount * 60.0f, pr.w, pr.h};
		imgArgs.SourceRect = {pr.x, pr.y, pr.w, pr.h};
		imgArgs.Scale = 1.0f;
		imgArgs.DrawColor = {255, 255, 255, 255};
		imgArgs.Priority = 1;
		imgArgs.Visible = true;
		auto* img = new UIImage(imgArgs);
		portraitPanel_->AddChild(img);
		++portraitCount;
	}
}

void calculateXP() {
	xpGained_ = 0;
	auto& battlers = BattleSystem::GetEnemyBattlers();
	for (size_t i = 0; i < battlers.size(); ++i) {
		if (!battlers[i] || battlers[i]->IsPlayer()) continue;
		if (battlers[i]->CurrentHP() == 0) {
			xpGained_ += battlers[i]->GetBattlerData()->XPReward;
		}
	}
}

void calculateDrops() {
	wonItems_.clear();
	auto& battlers = BattleSystem::GetEnemyBattlers();
	for (size_t i = 0; i < battlers.size(); ++i) {
		if (!battlers[i] || battlers[i]->IsPlayer()) continue;
		if (battlers[i]->CurrentHP() != 0) continue;
		for (auto& drop : battlers[i]->GetBattlerData()->ItemDrops) {
			int roll = rand() % 100;
			if (roll < drop.DropPercent) {
				wonItems_.push_back(drop.Name);
			}
		}
	}
}

void updateAnimatingIn() {
	animTime_ += GameState::DeltaTimeSeconds;

	float backdropX = Engine::Tweening::GetTweenedValue(
		kBackdropStartX, kBackdropRestX,
		animTime_, kBackdropAnimTime,
		Engine::Tweening::TweenEaseTypes::QuintOut);
	backdrop_->SetX(backdropX);
	solidBacking_->SetX(backdropX);

	float infoX = Engine::Tweening::GetTweenedValue(
		kInfoStartX, kInfoRestX,
		animTime_, kInfoAnimTime,
		Engine::Tweening::TweenEaseTypes::QuintOut);
	infoPanel_->SetX(infoX);

	float portraitX = Engine::Tweening::GetTweenedValue(
		kPortraitStartX, kPortraitRestX,
		animTime_, kPortraitAnimTime,
		Engine::Tweening::TweenEaseTypes::QuintOut);
	portraitPanel_->SetX(portraitX);

	if (animTime_ >= kBackdropAnimTime) {
		backdrop_->SetX(kBackdropRestX);
		solidBacking_->SetX(kBackdropRestX);
		infoPanel_->SetX(kInfoRestX);
		portraitPanel_->SetX(kPortraitRestX);
		currentState_ = WaitingForInput;
	}
}

void updateAccumulating() {
	accumTime_ += GameState::DeltaTimeSeconds;
	tickTimer_ += GameState::DeltaTimeSeconds;

	if (tickTimer_ >= kTickInterval) {
		tickTimer_ -= kTickInterval;
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
	}

	float t = accumTime_;
	if (t > kAccumDuration) t = kAccumDuration;

	int displayed = (int)Engine::Tweening::GetTweenedValue(
		0.0f, (float)xpGained_,
		t, kAccumDuration,
		Engine::Tweening::TweenEaseTypes::Linear);

	int remaining = xpGained_ - displayed;
	if (remaining > 0) {
		xpGainedText_->UpdateText("EXP Gained: " + to_string(remaining));
	} else {
		xpGainedText_->SetVisible(false);
	}

	for (int i = 0; i < numPlayers_; ++i) {
		int totalXP = playerRows_[i].startXP + displayed;
		int xpToNext = playerRows_[i].xpToNext;
		string xpStr = "XP: " + to_string(totalXP) + " / " + to_string(xpToNext);
		playerRows_[i].xpText->UpdateText(xpStr);
		float percent = xpToNext > 0 ? (float)totalXP / (float)xpToNext * 100.0f : 0.0f;
		if (percent > 100.0f) percent = 100.0f;
		playerRows_[i].xpBar->SetBarPercent(percent);
	}

	if (accumTime_ >= kAccumDuration) {
		currentState_ = Done;
		xpGainedText_->SetVisible(false);
		promptText_->UpdateText("Press A to continue");
	}
}

void skipAccumulation() {
	accumTime_ = kAccumDuration;
	xpGainedText_->SetVisible(false);
	for (int i = 0; i < numPlayers_; ++i) {
		int totalXP = playerRows_[i].startXP + xpGained_;
		int xpToNext = playerRows_[i].xpToNext;
		string xpStr = "XP: " + to_string(totalXP) + " / " + to_string(xpToNext);
		playerRows_[i].xpText->UpdateText(xpStr);
		float percent = xpToNext > 0 ? (float)totalXP / (float)xpToNext * 100.0f : 0.0f;
		if (percent > 100.0f) percent = 100.0f;
		playerRows_[i].xpBar->SetBarPercent(percent);
	}
	currentState_ = Done;
	promptText_->UpdateText("Press A to continue");
}

bool anyPlayerPressedA() {
	auto& p1 = PlayerControllerSystem::GetPlayerByNum(0);
	if (p1->IsButtonJustPressed(ControllerButtons::A)) return true;
	if (GameState::IsMultiplayer) {
		auto& p2 = PlayerControllerSystem::GetPlayerByNum(1);
		if (p2->IsButtonJustPressed(ControllerButtons::A)) return true;
	}
	return false;
}

}  // namespace

void BattleSpoilsSystem::TriggerBattleSpoils() {
	isDone_ = false;
	animTime_ = 0.0f;
	accumTime_ = 0.0f;
	tickTimer_ = 0.0f;
	currentState_ = AnimatingIn;
	calculateXP();
	calculateDrops();
	buildUI();
}

bool BattleSpoilsSystem::IsBattleSpoilsDone() {
	return isDone_;
}

void BattleSpoilsSystem::Update() {
	if (currentState_ == Inactive) return;

	switch (currentState_) {
		case AnimatingIn:
			updateAnimatingIn();
			break;
		case WaitingForInput:
			if (anyPlayerPressedA()) {
				currentState_ = Accumulating;
				promptText_->UpdateText("");
			}
			break;
		case Accumulating:
			if (anyPlayerPressedA()) {
				skipAccumulation();
			} else {
				updateAccumulating();
			}
			break;
		case Done:
			if (anyPlayerPressedA()) {
				isDone_ = true;
				currentState_ = Inactive;
			}
			break;
		default:
			break;
	}
}
