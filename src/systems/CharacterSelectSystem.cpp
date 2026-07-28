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
};

namespace {

const char* kPanelName = "CharSelectPanel";

bool _active = false;
bool _uiBuilt = false;
vector<CharacterEntry> _characters;
int _selectedIndex = 0;

UIObject* _panel = nullptr;
UIAnimation* _battleAnim = nullptr;
UIImage* _portraitImage = nullptr;
UIText* _nameText = nullptr;
UIText* _descText = nullptr;

void loadCharacterData() {
	if (!_characters.empty()) return;
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
		_characters.push_back(move(c));
	}
	jReleaseObjectFromFile(obj);
}

void buildUI() {
	if (_characters.empty()) return;
	auto root = UI::GetRootUIObject();

	UIObjectArgs panelArgs;
	panelArgs.Rect = {0, 0, 480, 270};
	panelArgs.Visible = true;
	panelArgs.Name = kPanelName;
	panelArgs.Priority = 5;
	_panel = new UIObject(panelArgs);
	root->AddChild(_panel);

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
	_panel->AddChild(bg);

	UITextArgs titleArgs;
	titleArgs.FontName = "PressStart2P";
	titleArgs.FontSize = 8;
	titleArgs.Rect = {0, 10, 360, 16};
	titleArgs.TextToDraw = "~ Choose Your Character ~";
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

	auto& ch = _characters[_selectedIndex];

	UIAnimationArgs animArgs;
	animArgs.Filename = ch.BattleSprite;
	animArgs.Name = "CharSelectAnim";
	animArgs.Rect = {20, 40, 144, 144};
	animArgs.SourceRect = {0, 0, 144, 144};
	animArgs.Scale = 1.0f;
	animArgs.DrawColor = {255, 255, 255, 255};
	animArgs.Priority = 2;
	animArgs.Visible = true;
	_battleAnim = new UIAnimation(animArgs);
	_battleAnim->GetAnimator().StartAnimation(ch.IdleAnim);
	bg->AddChild(_battleAnim);

	UIImageArgs portraitImgArgs;
	portraitImgArgs.Filename = ch.Portrait;
	portraitImgArgs.Name = "CharSelectPortrait";
	portraitImgArgs.Rect = {185, 45, ch.PortraitRect.w, ch.PortraitRect.h};
	portraitImgArgs.SourceRect = ch.PortraitRect;
	portraitImgArgs.Scale = 1.0f;
	portraitImgArgs.DrawColor = {255, 255, 255, 255};
	portraitImgArgs.Priority = 2;
	portraitImgArgs.Visible = true;
	_portraitImage = new UIImage(portraitImgArgs);
	bg->AddChild(_portraitImage);

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
	_nameText = new UIText(nameArgs);
	bg->AddChild(_nameText);

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
	_descText = new UIText(descArgs);
	bg->AddChild(_descText);

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

	_uiBuilt = true;
}

void destroyUI() {
	if (!_uiBuilt) return;
	auto root = UI::GetRootUIObject();
	root->DestroyChildByName(kPanelName);
	_panel = nullptr;
	_battleAnim = nullptr;
	_portraitImage = nullptr;
	_nameText = nullptr;
	_descText = nullptr;
	_uiBuilt = false;
}

}  // namespace

void CharacterSelectSystem::Start() {
	_active = false;
	_uiBuilt = false;
	_selectedIndex = 0;
	_panel = nullptr;
	_battleAnim = nullptr;
	_portraitImage = nullptr;
	_nameText = nullptr;
	_descText = nullptr;
}

void CharacterSelectSystem::Activate() {
	loadCharacterData();
	_active = true;
	_selectedIndex = 0;
	buildUI();
}

bool CharacterSelectSystem::IsActive() {
	return _active;
}

void CharacterSelectSystem::Update() {
	if (!_active) return;
	if (GameState::CurrentFadeState != (int)LoadingScreenFadeTypes::NotFading) return;

	auto& player = PlayerControllerSystem::GetPlayerByNum(0);

	if (player->IsButtonJustPressed(ControllerButtons::B)) {
		destroyUI();
		_active = false;
		return;
	}

	if (_characters.size() > 1) {
		bool changed = false;
		if (player->IsButtonJustPressed(ControllerButtons::Left)) {
			_selectedIndex = (_selectedIndex - 1 + (int)_characters.size()) % (int)_characters.size();
			Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
			changed = true;
		}
		if (player->IsButtonJustPressed(ControllerButtons::Right)) {
			_selectedIndex = (_selectedIndex + 1) % (int)_characters.size();
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
		auto selectedBattlerIndex = _characters[_selectedIndex].BattlerDBIndex;
		GameState::ResetForNewGame();
		GameState::SelectedPlayerCharacter = selectedBattlerIndex;
		BattleSystem::ResetAfterGameOver();
		destroyUI();
		_active = false;
		Engine::LoadScene("debugTown", 0.5f, 0.5f, false);
	}
}
