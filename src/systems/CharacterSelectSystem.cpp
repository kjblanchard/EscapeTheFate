#include <Supergoon/json.h>
#include <engine.hpp>
#include <gameState.hpp>
#include <systems/CharacterSelectSystem.hpp>
#include <systems/PlayerControllerSystem.hpp>
#include <systems/battleSystem.hpp>
#include <types/ControllerButtons.hpp>
#include <ui/ui.hpp>
#include <ui/uiAnimation.hpp>
#include <ui/uiImage.hpp>
#include <ui/uiNineSlice.hpp>
#include <ui/uiObject.hpp>
#include <ui/uiText.hpp>

using namespace Etf;
using namespace std;

struct CharacterEntry {
	int Id;
	string Name;
	string Description;
	string OverworldSprite;
	string BattleSprite;
	string Portrait;
	RectangleF PortraitRect;
	string IdleAnim;
	int BattlerDBIndex;
	int OverworldFrameW;
	int OverworldFrameH;
};

namespace {

constexpr const char* kPanelName = "CharSelectPanel";

bool active_ = false;
bool uiBuilt_ = false;
vector<CharacterEntry> characters_;
int selectedIndex_ = 0;
int selectingPlayer_ = 0;
int p1SelectedIndex_ = 0;

UIObject* panel_ = nullptr;
UIAnimation* battleAnim_ = nullptr;
UIImage* portraitImage_ = nullptr;
UIText* nameText_ = nullptr;
UIText* descText_ = nullptr;

void loadCharacterData() {
	if (!characters_.empty()) return;
	auto obj = Engine::Json::GetJsonObjectFromDirectory("playerCharacters");
	if (!obj) return;
	auto count = jGetObjectArrayLength(obj);
	for (int i = 0; i < count; ++i) {
		auto entry = jGetObjectInObjectWithIndex(obj, i);
		if (!entry) continue;
		CharacterEntry c;
		c.Id = jint(entry, "id");
		c.Name = jstr(entry, "name");
		c.Description = jstr(entry, "description");
		c.OverworldSprite = jstr(entry, "overworldSprite");
		c.BattleSprite = jstr(entry, "battleSprite");
		c.Portrait = jstr(entry, "portrait");
		c.PortraitRect = Engine::Json::GetRectFromObject(entry, "portraitRect");
		auto idle = jstr(entry, "idle");
		c.IdleAnim = idle ? idle : "idle1";
		c.BattlerDBIndex = jint(entry, "battlerDBIndex");
		c.OverworldFrameW = jint(entry, "overworldFrameW");
		c.OverworldFrameH = jint(entry, "overworldFrameH");
		if (c.OverworldFrameW == 0) c.OverworldFrameW = 32;
		if (c.OverworldFrameH == 0) c.OverworldFrameH = 32;
		characters_.push_back(std::move(c));
	}
	jReleaseObjectFromFile(obj);
}

void buildUI() {
	if (characters_.empty()) return;
	auto root = UI::GetRootUIObject();

	UIObjectArgs panelArgs;
	panelArgs.Rect = {0, 0, 480, 270};
	panelArgs.Visible = true;
	panelArgs.Name = kPanelName;
	panelArgs.Priority = 5;
	panel_ = new UIObject(panelArgs);
	root->AddChild(panel_);

	UINineSliceArgs bgArgs;
	bgArgs.Filename = "uibase";
	bgArgs.Name = "CharSelectBG";
	bgArgs.Rect = {60, 30, 360, 210};
	bgArgs.SourceRect = {0, 0, 64, 64};
	bgArgs.Xoffset = 8;
	bgArgs.Yoffset = 8;
	bgArgs.Scale = 1.0f;
	bgArgs.DrawColor = {80, 0, 120, 235};
	bgArgs.Priority = 1;
	bgArgs.Visible = true;
	auto* bg = new UINineSlice(bgArgs);
	panel_->AddChild(bg);

	UITextArgs titleArgs;
	titleArgs.FontName = "PressStart2P";
	titleArgs.FontSize = 8;
	titleArgs.Rect = {0, 10, 360, 16};
	if (GameState::IsMultiplayer) {
		titleArgs.TextToDraw = selectingPlayer_ == 0 ? "~ P1: Choose Character ~" : "~ P2: Choose Character ~";
	} else {
		titleArgs.TextToDraw = "~ Choose Your Character ~";
	}
	titleArgs.Name = "CharSelectTitle";
	titleArgs.NumCharsToDraw = 100;
	titleArgs.Priority = 2;
	titleArgs.TextColor = {255, 255, 200, 255};
	titleArgs.CenteredX = true;
	titleArgs.CenteredY = false;
	titleArgs.WordWrap = false;
	titleArgs.Visible = true;
	titleArgs.DebugBox = false;
	bg->AddChild(new UIText(titleArgs));

	auto& ch = characters_[selectedIndex_];

	UIAnimationArgs animArgs;
	animArgs.Filename = ch.BattleSprite;
	animArgs.Name = "CharSelectAnim";
	animArgs.Rect = {20, 40, 144, 144};
	animArgs.SourceRect = {0, 0, 144, 144};
	animArgs.Scale = 1.0f;
	animArgs.DrawColor = {255, 255, 255, 255};
	animArgs.Priority = 2;
	animArgs.Visible = true;
	battleAnim_ = new UIAnimation(animArgs);
	battleAnim_->GetAnimator().StartAnimation(ch.IdleAnim);
	bg->AddChild(battleAnim_);

	UIImageArgs portraitImgArgs;
	portraitImgArgs.Filename = ch.Portrait;
	portraitImgArgs.Name = "CharSelectPortrait";
	portraitImgArgs.Rect = {185, 45, ch.PortraitRect.w, ch.PortraitRect.h};
	portraitImgArgs.SourceRect = ch.PortraitRect;
	portraitImgArgs.Scale = 1.0f;
	portraitImgArgs.DrawColor = {255, 255, 255, 255};
	portraitImgArgs.Priority = 2;
	portraitImgArgs.Visible = true;
	portraitImage_ = new UIImage(portraitImgArgs);
	bg->AddChild(portraitImage_);

	UITextArgs nameArgs;
	nameArgs.FontName = "PressStart2P";
	nameArgs.FontSize = 8;
	nameArgs.Rect = {185, 100, 160, 16};
	nameArgs.TextToDraw = ch.Name;
	nameArgs.Name = "CharSelectName";
	nameArgs.NumCharsToDraw = 100;
	nameArgs.Priority = 2;
	nameArgs.TextColor = {255, 255, 255, 255};
	nameArgs.CenteredX = false;
	nameArgs.CenteredY = false;
	nameArgs.WordWrap = false;
	nameArgs.Visible = true;
	nameArgs.DebugBox = false;
	nameText_ = new UIText(nameArgs);
	bg->AddChild(nameText_);

	UITextArgs descArgs;
	descArgs.FontName = "PressStart2P";
	descArgs.FontSize = 8;
	descArgs.Rect = {185, 120, 155, 60};
	descArgs.TextToDraw = ch.Description;
	descArgs.Name = "CharSelectDesc";
	descArgs.NumCharsToDraw = 200;
	descArgs.Priority = 2;
	descArgs.TextColor = {200, 200, 200, 255};
	descArgs.CenteredX = false;
	descArgs.CenteredY = false;
	descArgs.WordWrap = true;
	descArgs.Visible = true;
	descArgs.DebugBox = false;
	descText_ = new UIText(descArgs);
	bg->AddChild(descText_);

	UITextArgs promptArgs;
	promptArgs.FontName = "PressStart2P";
	promptArgs.FontSize = 8;
	promptArgs.Rect = {0, 185, 360, 12};
	promptArgs.TextToDraw = "Press A to confirm";
	promptArgs.Name = "CharSelectPrompt";
	promptArgs.NumCharsToDraw = 100;
	promptArgs.Priority = 2;
	promptArgs.TextColor = {200, 255, 200, 255};
	promptArgs.CenteredX = true;
	promptArgs.CenteredY = false;
	promptArgs.WordWrap = false;
	promptArgs.Visible = true;
	promptArgs.DebugBox = false;
	bg->AddChild(new UIText(promptArgs));

	uiBuilt_ = true;
}

void destroyUI() {
	if (!uiBuilt_) return;
	auto root = UI::GetRootUIObject();
	root->DestroyChildByName(kPanelName);
	panel_ = nullptr;
	battleAnim_ = nullptr;
	portraitImage_ = nullptr;
	nameText_ = nullptr;
	descText_ = nullptr;
	uiBuilt_ = false;
}

}  // namespace

void CharacterSelectSystem::Start() {
	active_ = false;
	uiBuilt_ = false;
	selectedIndex_ = 0;
	panel_ = nullptr;
	battleAnim_ = nullptr;
	portraitImage_ = nullptr;
	nameText_ = nullptr;
	descText_ = nullptr;
}

void CharacterSelectSystem::Activate() {
	loadCharacterData();
	active_ = true;
	selectedIndex_ = 0;
	selectingPlayer_ = 0;
	p1SelectedIndex_ = 0;
	buildUI();
}

bool CharacterSelectSystem::IsActive() {
	return active_;
}

void CharacterSelectSystem::Update() {
	if (!active_) return;
	if (GameState::CurrentFadeState != (int)LoadingScreenFadeTypes::NotFading) return;

	auto& player = PlayerControllerSystem::GetPlayerByNum(selectingPlayer_);

	if (player->IsButtonJustPressed(ControllerButtons::B)) {
		if (GameState::IsMultiplayer && selectingPlayer_ == 1) {
			selectingPlayer_ = 0;
			selectedIndex_ = p1SelectedIndex_;
			destroyUI();
			buildUI();
			return;
		}
		destroyUI();
		active_ = false;
		return;
	}

	if (characters_.size() > 1) {
		bool changed = false;
		if (player->IsButtonJustPressed(ControllerButtons::Left)) {
			selectedIndex_ = (selectedIndex_ - 1 + (int)characters_.size()) % (int)characters_.size();
			Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
			changed = true;
		}
		if (player->IsButtonJustPressed(ControllerButtons::Right)) {
			selectedIndex_ = (selectedIndex_ + 1) % (int)characters_.size();
			Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
			changed = true;
		}
		if (changed) {
			destroyUI();
			buildUI();
		}
	}

	if (player->IsButtonJustPressed(ControllerButtons::A)) {
		Engine::Audio::PlaySFXBuffer("menuSelect", 0.75f);
		auto& ch = characters_[selectedIndex_];

		if (!GameState::IsMultiplayer) {
			GameState::ResetForNewGame();
			GameState::SelectedPlayerCharacter = ch.BattlerDBIndex;
			GameState::SelectedOverworldSprite = ch.OverworldSprite;
			GameState::SelectedOverworldFrameW = ch.OverworldFrameW;
			GameState::SelectedOverworldFrameH = ch.OverworldFrameH;
			BattleSystem::ResetAfterGameOver();
			destroyUI();
			active_ = false;
			Engine::LoadScene("debugTown", 0.5f, 0.5f, false);
		} else if (selectingPlayer_ == 0) {
			p1SelectedIndex_ = selectedIndex_;
			selectingPlayer_ = 1;
			selectedIndex_ = 0;
			destroyUI();
			buildUI();
		} else {
			GameState::ResetForNewGame();
			GameState::IsMultiplayer = true;
			auto& p1ch = characters_[p1SelectedIndex_];
			GameState::SelectedPlayerCharacter = p1ch.BattlerDBIndex;
			GameState::SelectedOverworldSprite = p1ch.OverworldSprite;
			GameState::SelectedOverworldFrameW = p1ch.OverworldFrameW;
			GameState::SelectedOverworldFrameH = p1ch.OverworldFrameH;
			GameState::SelectedPlayerCharacter2 = ch.BattlerDBIndex;
			GameState::SelectedOverworldSprite2 = ch.OverworldSprite;
			GameState::SelectedOverworldFrameW2 = ch.OverworldFrameW;
			GameState::SelectedOverworldFrameH2 = ch.OverworldFrameH;
			BattleSystem::ResetAfterGameOver();
			destroyUI();
			active_ = false;
			Engine::LoadScene("debugTown", 0.5f, 0.5f, false);
		}
	}
}
