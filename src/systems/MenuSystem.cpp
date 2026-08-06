#include <engine.hpp>
#include <format>
#include <gameState.hpp>
#include <systems/MenuSystem.hpp>
#include <systems/PlayerControllerSystem.hpp>
#include <systems/battleSystem.hpp>
#include <types/ControllerButtons.hpp>
#include <ui/ui.hpp>
#include <ui/uiImage.hpp>
#include <ui/uiNineSlice.hpp>
#include <ui/uiText.hpp>

using namespace Etf;

static const int kNumItems = 6;
static const char* kItemLabels[kNumItems] = {"Items", "Abilities", "Equipment", "Magic", "Stats", "Save"};
static const bool kItemEnabled[kNumItems] = {true, false, false, false, true, false};

static const Color kEnabledColor = {255, 255, 255, 255};
static const Color kDisabledColor = {150, 150, 150, 128};
static const Color kPanelColor = {50, 0, 80, 200};

static const float kPanelW = 170.0f;
static const float kPanelH = 260.0f;
static const float kPanelY = 5.0f;
static const float kPanelX[2] = {5.0f, 305.0f};
static const float kItemStartY = 42.0f;
static const float kItemSpacing = 14.0f;

static const int kNumStatLines = 7;
static const float kStatLineSpacing = 14.0f;

static const int kMaxItemLines = 8;

static UIObject* _panels[2] = {nullptr, nullptr};
static UIObject* _statsPanels[2] = {nullptr, nullptr};
static UIObject* _itemsPanels[2] = {nullptr, nullptr};
static UIImage* _fingers[2] = {nullptr, nullptr};
static UIText* _timeTexts[2] = {nullptr, nullptr};
static UIText* _statLineTexts[2][kNumStatLines] = {};
static UIText* _itemLineTexts[2][kMaxItemLines] = {};
static UIText* _portraitNameTexts[2] = {nullptr, nullptr};
static int _selectedIndex[2] = {0, 0};
static bool _statsOpen[2] = {false, false};
static bool _itemsOpen[2] = {false, false};
static bool _initialized = false;

static void positionFinger(int playerIdx) {
	if (!_fingers[playerIdx]) return;
	float y = kItemStartY + (_selectedIndex[playerIdx] * kItemSpacing);
	_fingers[playerIdx]->SetY(y);
}

static void updateTimeText() {
	int total = static_cast<int>(GameState::TotalPlaytimeSeconds);
	int minutes = total / 60;
	int seconds = total % 60;
	auto timeStr = std::format("{:02d}:{:02d}", minutes, seconds);
	for (int i = 0; i < 2; ++i) {
		if (_timeTexts[i] && GameState::Menu::MenuOpen[i]) {
			_timeTexts[i]->UpdateText(timeStr);
		}
	}
}

static void rebuildPortrait(int playerIdx) {
	auto* panel = _panels[playerIdx];
	if (!panel) return;

	// Remove old portrait if exists
	panel->DestroyChildByName(std::format("MenuPortrait{}", playerIdx));

	auto* data = BattleSystem::GetPlayerBattlerData(playerIdx);
	if (!data || data->Portrait.empty()) return;

	UIImageArgs portraitArgs;
	portraitArgs.Filename = data->Portrait;
	portraitArgs.Name = std::format("MenuPortrait{}", playerIdx);
	portraitArgs.Rect = {(kPanelW - data->PortraitRect.w) / 2.0f, kPanelH - 88.0f, data->PortraitRect.w, data->PortraitRect.h};
	portraitArgs.SourceRect = data->PortraitRect;
	portraitArgs.Scale = 1.0f;
	portraitArgs.DrawColor = {255, 255, 255, 255};
	portraitArgs.Priority = 1;
	portraitArgs.Visible = true;
	portraitArgs.DebugBox = false;
	panel->AddChild(new UIImage(portraitArgs));

	// Update the name text under the portrait
	if (_portraitNameTexts[playerIdx]) {
		_portraitNameTexts[playerIdx]->UpdateText(data->Name);
	}
}

static void openStatsPanel(int playerIdx) {
	auto* data = BattleSystem::GetPlayerBattlerData(playerIdx);
	if (!data) return;

	auto& save = GameState::Save::PlayerData[playerIdx];
	int currentHP = (save.CurrentHP < 0) ? data->HP : save.CurrentHP;
	int currentXP = (save.CurrentHP < 0) ? data->CurrentXP : save.CurrentXP;
	int xpToNext = (save.CurrentHP < 0) ? data->XPToNextLevel : save.XPToNextLevel;

	std::string lines[kNumStatLines];
	lines[0] = data->Name;
	lines[1] = std::format("HP:  {}/{}", currentHP, data->HP);
	lines[2] = std::format("STR: {}  MAG: {}", data->Str, data->Mag);
	lines[3] = std::format("DEF: {}  MDF: {}", data->Def, data->MDef);
	lines[4] = std::format("SPD: {}  POW: {}", data->Spd, data->Pow);
	lines[5] = std::format("AP:  {}", data->MaxAP);
	lines[6] = std::format("XP:  {}/{}", currentXP, xpToNext);

	for (int i = 0; i < kNumStatLines; ++i) {
		if (_statLineTexts[playerIdx][i]) {
			_statLineTexts[playerIdx][i]->UpdateText(lines[i]);
		}
	}

	if (_statsPanels[playerIdx]) _statsPanels[playerIdx]->SetVisible(true);
	_statsOpen[playerIdx] = true;
}

static void openItemsPanel(int playerIdx) {
	auto& inventory = GameState::Save::Inventory;
	for (int i = 0; i < kMaxItemLines; ++i) {
		if (!_itemLineTexts[playerIdx][i]) continue;
		if (i == 0) {
			if (inventory.empty()) {
				_itemLineTexts[playerIdx][i]->UpdateText("(empty)");
			} else {
				_itemLineTexts[playerIdx][i]->UpdateText("ITEMS");
			}
		} else {
			int itemIdx = i - 1;
			if (itemIdx < (int)inventory.size()) {
				_itemLineTexts[playerIdx][i]->UpdateText(inventory[itemIdx]);
			} else {
				_itemLineTexts[playerIdx][i]->UpdateText("");
			}
		}
	}
	if (_itemsPanels[playerIdx]) _itemsPanels[playerIdx]->SetVisible(true);
	_itemsOpen[playerIdx] = true;
}

static void buildMenuPanelForPlayer(int playerIdx) {
	UINineSliceArgs panelArgs;
	panelArgs.Name = std::format("MenuPanel{}", playerIdx);
	panelArgs.Filename = "uibase";
	panelArgs.Rect = {kPanelX[playerIdx], kPanelY, kPanelW, kPanelH};
	panelArgs.SourceRect = {0, 0, 64, 64};
	panelArgs.DrawColor = kPanelColor;
	panelArgs.Xoffset = 8;
	panelArgs.Yoffset = 8;
	panelArgs.Scale = 1.0f;
	panelArgs.Priority = 60;
	panelArgs.Visible = false;
	panelArgs.DebugBox = false;
	auto* panel = new UINineSlice(panelArgs);
	panel->SetDoNotDestroy(true);

	// Title
	UITextArgs titleArgs;
	titleArgs.FontName = "PressStart2P";
	titleArgs.FontSize = 8;
	titleArgs.Rect = {0.0f, 10.0f, kPanelW, 10.0f};
	titleArgs.TextToDraw = "- MENU -";
	titleArgs.Name = std::format("MenuTitle{}", playerIdx);
	titleArgs.NumCharsToDraw = 20;
	titleArgs.Priority = 0;
	titleArgs.TextColor = {255, 255, 200, 255};
	titleArgs.CenteredX = true;
	titleArgs.CenteredY = false;
	titleArgs.WordWrap = false;
	titleArgs.Visible = true;
	titleArgs.DebugBox = false;
	panel->AddChild(new UIText(titleArgs));

	// Player label
	UITextArgs playerLabelArgs;
	playerLabelArgs.FontName = "PressStart2P";
	playerLabelArgs.FontSize = 8;
	playerLabelArgs.Rect = {10.0f, 28.0f, 40.0f, 10.0f};
	playerLabelArgs.TextToDraw = std::format("P{}", playerIdx + 1);
	playerLabelArgs.Name = std::format("MenuPlayerLabel{}", playerIdx);
	playerLabelArgs.NumCharsToDraw = 5;
	playerLabelArgs.Priority = 0;
	playerLabelArgs.TextColor = {255, 0, 255, 255};
	playerLabelArgs.CenteredX = false;
	playerLabelArgs.CenteredY = false;
	playerLabelArgs.WordWrap = false;
	playerLabelArgs.Visible = true;
	playerLabelArgs.DebugBox = false;
	panel->AddChild(new UIText(playerLabelArgs));

	// Menu items
	for (int i = 0; i < kNumItems; ++i) {
		UITextArgs itemArgs;
		itemArgs.FontName = "PressStart2P";
		itemArgs.FontSize = 8;
		itemArgs.Rect = {20.0f, kItemStartY + (i * kItemSpacing), 140.0f, 10.0f};
		itemArgs.TextToDraw = kItemLabels[i];
		itemArgs.Name = std::format("MenuItem{}_{}", i, playerIdx);
		itemArgs.NumCharsToDraw = 20;
		itemArgs.Priority = 0;
		itemArgs.TextColor = kItemEnabled[i] ? kEnabledColor : kDisabledColor;
		itemArgs.CenteredX = false;
		itemArgs.CenteredY = false;
		itemArgs.WordWrap = false;
		itemArgs.Visible = true;
		itemArgs.DebugBox = false;
		panel->AddChild(new UIText(itemArgs));
	}

	// Finger cursor
	UIImageArgs fingerArgs;
	fingerArgs.Filename = "fingers";
	fingerArgs.Name = std::format("MenuFinger{}", playerIdx);
	fingerArgs.Rect = {7.0f, kItemStartY, 10.0f, 10.0f};
	fingerArgs.SourceRect = {0, 0, 16, 16};
	fingerArgs.Scale = 1.0f;
	fingerArgs.DrawColor = {255, 255, 255, 255};
	fingerArgs.Priority = 1;
	fingerArgs.Visible = true;
	fingerArgs.DebugBox = false;
	auto* finger = new UIImage(fingerArgs);
	panel->AddChild(finger);
	_fingers[playerIdx] = finger;

	// Portrait name text (centered, below portrait area)
	UITextArgs nameArgs;
	nameArgs.FontName = "PressStart2P";
	nameArgs.FontSize = 8;
	nameArgs.Rect = {0.0f, kPanelH - 36.0f, kPanelW, 10.0f};
	nameArgs.TextToDraw = "";
	nameArgs.Name = std::format("MenuCharName{}", playerIdx);
	nameArgs.NumCharsToDraw = 20;
	nameArgs.Priority = 0;
	nameArgs.TextColor = {255, 255, 255, 255};
	nameArgs.CenteredX = true;
	nameArgs.CenteredY = false;
	nameArgs.WordWrap = false;
	nameArgs.Visible = true;
	nameArgs.DebugBox = false;
	auto* nameText = new UIText(nameArgs);
	panel->AddChild(nameText);
	_portraitNameTexts[playerIdx] = nameText;

	// Hint text
	UITextArgs hintArgs;
	hintArgs.FontName = "PressStart2P";
	hintArgs.FontSize = 8;
	hintArgs.Rect = {0.0f, kPanelH - 22.0f, kPanelW, 10.0f};
	hintArgs.TextToDraw = "B:Back A:Select";
	hintArgs.Name = std::format("MenuHint{}", playerIdx);
	hintArgs.NumCharsToDraw = 30;
	hintArgs.Priority = 0;
	hintArgs.TextColor = {150, 150, 150, 255};
	hintArgs.CenteredX = true;
	hintArgs.CenteredY = false;
	hintArgs.WordWrap = false;
	hintArgs.Visible = true;
	hintArgs.DebugBox = false;
	panel->AddChild(new UIText(hintArgs));

	// Time text (very bottom)
	UITextArgs timeArgs;
	timeArgs.FontName = "PressStart2P";
	timeArgs.FontSize = 8;
	timeArgs.Rect = {0.0f, kPanelH - 10.0f, kPanelW, 10.0f};
	timeArgs.TextToDraw = "00:00";
	timeArgs.Name = std::format("MenuTime{}", playerIdx);
	timeArgs.NumCharsToDraw = 20;
	timeArgs.Priority = 0;
	timeArgs.TextColor = {130, 130, 130, 255};
	timeArgs.CenteredX = true;
	timeArgs.CenteredY = false;
	timeArgs.WordWrap = false;
	timeArgs.Visible = true;
	timeArgs.DebugBox = false;
	auto* timeText = new UIText(timeArgs);
	panel->AddChild(timeText);
	_timeTexts[playerIdx] = timeText;

	// Stats sub-panel (covers menu items area when viewing stats)
	UINineSliceArgs statsArgs;
	statsArgs.Name = std::format("StatsPanel{}", playerIdx);
	statsArgs.Filename = "uibase";
	statsArgs.Rect = {3.0f, kItemStartY - 4.0f, kPanelW - 6.0f, kNumStatLines * kStatLineSpacing + 16.0f};
	statsArgs.SourceRect = {0, 0, 64, 64};
	statsArgs.DrawColor = {40, 0, 60, 220};
	statsArgs.Xoffset = 8;
	statsArgs.Yoffset = 8;
	statsArgs.Scale = 1.0f;
	statsArgs.Priority = 2;
	statsArgs.Visible = false;
	statsArgs.DebugBox = false;
	auto* statsPanel = new UINineSlice(statsArgs);

	for (int i = 0; i < kNumStatLines; ++i) {
		UITextArgs lineArgs;
		lineArgs.FontName = "PressStart2P";
		lineArgs.FontSize = 8;
		lineArgs.Rect = {8.0f, 8.0f + (i * kStatLineSpacing), kPanelW - 22.0f, 10.0f};
		lineArgs.TextToDraw = " ";
		lineArgs.Name = std::format("StatLine{}_{}", i, playerIdx);
		lineArgs.NumCharsToDraw = 30;
		lineArgs.Priority = 0;
		lineArgs.TextColor = (i == 0) ? Color{255, 255, 200, 255} : kEnabledColor;
		lineArgs.CenteredX = false;
		lineArgs.CenteredY = false;
		lineArgs.WordWrap = false;
		lineArgs.Visible = true;
		lineArgs.DebugBox = false;
		auto* lineText = new UIText(lineArgs);
		statsPanel->AddChild(lineText);
		_statLineTexts[playerIdx][i] = lineText;
	}

	panel->AddChild(statsPanel);
	_statsPanels[playerIdx] = statsPanel;

	// Items sub-panel
	UINineSliceArgs itemsPanelArgs;
	itemsPanelArgs.Name = std::format("ItemsPanel{}", playerIdx);
	itemsPanelArgs.Filename = "uibase";
	itemsPanelArgs.Rect = {3.0f, kItemStartY - 4.0f, kPanelW - 6.0f, kMaxItemLines * kStatLineSpacing + 16.0f};
	itemsPanelArgs.SourceRect = {0, 0, 64, 64};
	itemsPanelArgs.DrawColor = {40, 0, 60, 220};
	itemsPanelArgs.Xoffset = 8;
	itemsPanelArgs.Yoffset = 8;
	itemsPanelArgs.Scale = 1.0f;
	itemsPanelArgs.Priority = 2;
	itemsPanelArgs.Visible = false;
	itemsPanelArgs.DebugBox = false;
	auto* itemsPanel = new UINineSlice(itemsPanelArgs);

	for (int i = 0; i < kMaxItemLines; ++i) {
		UITextArgs lineArgs;
		lineArgs.FontName = "PressStart2P";
		lineArgs.FontSize = 8;
		lineArgs.Rect = {8.0f, 8.0f + (i * kStatLineSpacing), kPanelW - 22.0f, 10.0f};
		lineArgs.TextToDraw = " ";
		lineArgs.Name = std::format("ItemLine{}_{}", i, playerIdx);
		lineArgs.NumCharsToDraw = 30;
		lineArgs.Priority = 0;
		lineArgs.TextColor = (i == 0) ? Color{255, 255, 200, 255} : kEnabledColor;
		lineArgs.CenteredX = false;
		lineArgs.CenteredY = false;
		lineArgs.WordWrap = false;
		lineArgs.Visible = true;
		lineArgs.DebugBox = false;
		auto* lineText = new UIText(lineArgs);
		itemsPanel->AddChild(lineText);
		_itemLineTexts[playerIdx][i] = lineText;
	}

	panel->AddChild(itemsPanel);
	_itemsPanels[playerIdx] = itemsPanel;

	UI::GetRootUIObject()->AddChild(panel);
	_panels[playerIdx] = panel;
}

void MenuSystem::Start() {
	_initialized = false;
	_selectedIndex[0] = 0;
	_selectedIndex[1] = 0;
	_statsOpen[0] = false;
	_statsOpen[1] = false;
	_itemsOpen[0] = false;
	_itemsOpen[1] = false;
	for (auto& p : _panels) p = nullptr;
	for (auto& p : _statsPanels) p = nullptr;
	for (auto& p : _itemsPanels) p = nullptr;
	for (auto& f : _fingers) f = nullptr;
	for (auto& t : _timeTexts) t = nullptr;
	for (auto& t : _portraitNameTexts) t = nullptr;
	for (auto& row : _statLineTexts) {
		for (auto& t : row) t = nullptr;
	}
	for (auto& row : _itemLineTexts) {
		for (auto& t : row) t = nullptr;
	}
	buildMenuPanelForPlayer(0);
	buildMenuPanelForPlayer(1);
	_initialized = true;
}

void MenuSystem::Update() {
	bool isPlaying = !GameState::Paused &&
					 !GameState::Battle::InBattle &&
					 !GameState::InDialog &&
					 GameState::CurrentFadeState == (int)LoadingScreenFadeTypes::NotFading &&
					 Engine::CurrentSceneName() != "cloud" &&
					 !Engine::CurrentSceneName().empty();
	if (isPlaying) {
		GameState::TotalPlaytimeSeconds += GameState::DeltaTimeSeconds;
	}

	if (!_initialized) return;

	for (int i = 0; i < 2; ++i) {
		auto& player = PlayerControllerSystem::GetPlayerByNum(i);
		bool menuOpen = GameState::Menu::MenuOpen[i];

		if (!menuOpen) {
			if (GameState::Paused) continue;
			if (GameState::Battle::InBattle) continue;
			if (GameState::InDialog) continue;
			if (GameState::CurrentFadeState != (int)LoadingScreenFadeTypes::NotFading) continue;
			if (Engine::CurrentSceneName() == "cloud") continue;
			if (Engine::CurrentSceneName().empty()) continue;

			if (player->IsButtonJustPressed(ControllerButtons::Y)) {
				GameState::Menu::MenuOpen[i] = true;
				_selectedIndex[i] = 0;
				_statsOpen[i] = false;
				_itemsOpen[i] = false;
				if (_statsPanels[i]) _statsPanels[i]->SetVisible(false);
				if (_itemsPanels[i]) _itemsPanels[i]->SetVisible(false);
				if (_panels[i]) _panels[i]->SetVisible(true);
				rebuildPortrait(i);
				updateTimeText();
				positionFinger(i);
				Engine::Audio::PlaySFXBuffer("menuSelect", 0.75f);
			}
			continue;
		}

		// Menu is open
		updateTimeText();

		if (player->IsButtonJustPressed(ControllerButtons::B) ||
			player->IsButtonJustPressed(ControllerButtons::Y)) {
			if (_statsOpen[i]) {
				_statsOpen[i] = false;
				if (_statsPanels[i]) _statsPanels[i]->SetVisible(false);
				Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
			} else if (_itemsOpen[i]) {
				_itemsOpen[i] = false;
				if (_itemsPanels[i]) _itemsPanels[i]->SetVisible(false);
				Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
			} else {
				GameState::Menu::MenuOpen[i] = false;
				if (_panels[i]) _panels[i]->SetVisible(false);
				Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
			}
			continue;
		}

		if (_statsOpen[i] || _itemsOpen[i]) continue;

		if (player->IsButtonJustPressed(ControllerButtons::Up)) {
			_selectedIndex[i] = (_selectedIndex[i] - 1 + kNumItems) % kNumItems;
			Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
			positionFinger(i);
		}
		if (player->IsButtonJustPressed(ControllerButtons::Down)) {
			_selectedIndex[i] = (_selectedIndex[i] + 1) % kNumItems;
			Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
			positionFinger(i);
		}

		if (player->IsButtonJustPressed(ControllerButtons::A)) {
			int idx = _selectedIndex[i];
			if (!kItemEnabled[idx]) {
				Engine::Audio::PlaySFXBuffer("error1", 0.75f);
			} else if (idx == 0) {
				Engine::Audio::PlaySFXBuffer("menuSelect", 0.75f);
				openItemsPanel(i);
			} else if (idx == 4) {
				Engine::Audio::PlaySFXBuffer("menuSelect", 0.75f);
				openStatsPanel(i);
			}
		}
	}
}
