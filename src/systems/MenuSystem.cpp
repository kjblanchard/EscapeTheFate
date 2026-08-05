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
static const bool kItemEnabled[kNumItems] = {false, false, false, false, true, false};

static const Color kEnabledColor = {255, 255, 255, 255};
static const Color kDisabledColor = {150, 150, 150, 128};
static const Color kPanelColor = {80, 0, 120, 235};

static const float kPanelW = 230.0f;
static const float kPanelH = 260.0f;
static const float kPanelY = 5.0f;
static const float kPanelX[2] = {5.0f, 245.0f};
static const float kItemStartY = 55.0f;
static const float kItemSpacing = 18.0f;

static UIObject* _panels[2] = {nullptr, nullptr};
static UIObject* _statsPanels[2] = {nullptr, nullptr};
static UIImage* _fingers[2] = {nullptr, nullptr};
static UIText* _timeTexts[2] = {nullptr, nullptr};
static UIText* _statTexts[2] = {nullptr, nullptr};
static int _selectedIndex[2] = {0, 0};
static bool _statsOpen[2] = {false, false};
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
	auto timeStr = std::format("TIME {:02d}:{:02d}", minutes, seconds);
	for (int i = 0; i < 2; ++i) {
		if (_timeTexts[i] && GameState::Menu::MenuOpen[i]) {
			_timeTexts[i]->UpdateText(timeStr);
		}
	}
}

static void openStatsPanel(int playerIdx) {
	auto* data = BattleSystem::GetPlayerBattlerData(playerIdx);
	if (!data || !_statTexts[playerIdx]) return;
	auto text = std::format(
		"{}\n\n"
		"HP:  {}\n"
		"STR: {}  MAG: {}\n"
		"DEF: {}  MDF: {}\n"
		"SPD: {}  POW: {}\n"
		"AP:  {}\n"
		"XP:  {}/{}",
		data->Name, data->HP,
		data->Str, data->Mag,
		data->Def, data->MDef,
		data->Spd, data->Pow,
		data->MaxAP,
		data->CurrentXP, data->XPToNextLevel);
	_statTexts[playerIdx]->UpdateText(text);
	if (_statsPanels[playerIdx]) _statsPanels[playerIdx]->SetVisible(true);
	_statsOpen[playerIdx] = true;
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
	titleArgs.Rect = {0.0f, 8.0f, kPanelW, 12.0f};
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
	playerLabelArgs.Rect = {10.0f, 22.0f, 40.0f, 12.0f};
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

	// Time text
	UITextArgs timeArgs;
	timeArgs.FontName = "PressStart2P";
	timeArgs.FontSize = 8;
	timeArgs.Rect = {10.0f, 38.0f, 200.0f, 12.0f};
	timeArgs.TextToDraw = "TIME 00:00";
	timeArgs.Name = std::format("MenuTime{}", playerIdx);
	timeArgs.NumCharsToDraw = 20;
	timeArgs.Priority = 0;
	timeArgs.TextColor = {200, 200, 200, 255};
	timeArgs.CenteredX = false;
	timeArgs.CenteredY = false;
	timeArgs.WordWrap = false;
	timeArgs.Visible = true;
	timeArgs.DebugBox = false;
	auto* timeText = new UIText(timeArgs);
	panel->AddChild(timeText);
	_timeTexts[playerIdx] = timeText;

	// Menu items
	for (int i = 0; i < kNumItems; ++i) {
		UITextArgs itemArgs;
		itemArgs.FontName = "PressStart2P";
		itemArgs.FontSize = 8;
		itemArgs.Rect = {22.0f, kItemStartY + (i * kItemSpacing), 180.0f, 12.0f};
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
	fingerArgs.Rect = {8.0f, kItemStartY, 12.0f, 12.0f};
	fingerArgs.SourceRect = {0, 0, 16, 16};
	fingerArgs.Scale = 1.0f;
	fingerArgs.DrawColor = {255, 255, 255, 255};
	fingerArgs.Priority = 1;
	fingerArgs.Visible = true;
	fingerArgs.DebugBox = false;
	auto* finger = new UIImage(fingerArgs);
	panel->AddChild(finger);
	_fingers[playerIdx] = finger;

	// Stats sub-panel
	UINineSliceArgs statsArgs;
	statsArgs.Name = std::format("StatsPanel{}", playerIdx);
	statsArgs.Filename = "uibase";
	statsArgs.Rect = {4.0f, kItemStartY - 2.0f, kPanelW - 8.0f, 160.0f};
	statsArgs.SourceRect = {0, 0, 64, 64};
	statsArgs.DrawColor = {60, 0, 90, 245};
	statsArgs.Xoffset = 8;
	statsArgs.Yoffset = 8;
	statsArgs.Scale = 1.0f;
	statsArgs.Priority = 2;
	statsArgs.Visible = false;
	statsArgs.DebugBox = false;
	auto* statsPanel = new UINineSlice(statsArgs);

	UITextArgs statsTextArgs;
	statsTextArgs.FontName = "PressStart2P";
	statsTextArgs.FontSize = 8;
	statsTextArgs.Rect = {8.0f, 8.0f, kPanelW - 24.0f, 140.0f};
	statsTextArgs.TextToDraw = "No data";
	statsTextArgs.Name = std::format("StatsText{}", playerIdx);
	statsTextArgs.NumCharsToDraw = 200;
	statsTextArgs.Priority = 0;
	statsTextArgs.TextColor = {255, 255, 255, 255};
	statsTextArgs.CenteredX = false;
	statsTextArgs.CenteredY = false;
	statsTextArgs.WordWrap = true;
	statsTextArgs.Visible = true;
	statsTextArgs.DebugBox = false;
	auto* statsText = new UIText(statsTextArgs);
	statsPanel->AddChild(statsText);
	_statTexts[playerIdx] = statsText;

	panel->AddChild(statsPanel);
	_statsPanels[playerIdx] = statsPanel;

	// Hint text
	UITextArgs hintArgs;
	hintArgs.FontName = "PressStart2P";
	hintArgs.FontSize = 8;
	hintArgs.Rect = {0.0f, kPanelH - 18.0f, kPanelW, 12.0f};
	hintArgs.TextToDraw = "B:Close  A:Select";
	hintArgs.Name = std::format("MenuHint{}", playerIdx);
	hintArgs.NumCharsToDraw = 30;
	hintArgs.Priority = 0;
	hintArgs.TextColor = {200, 200, 200, 255};
	hintArgs.CenteredX = true;
	hintArgs.CenteredY = false;
	hintArgs.WordWrap = false;
	hintArgs.Visible = true;
	hintArgs.DebugBox = false;
	panel->AddChild(new UIText(hintArgs));

	UI::GetRootUIObject()->AddChild(panel);
	_panels[playerIdx] = panel;
}

void MenuSystem::Start() {
	_initialized = false;
	_selectedIndex[0] = 0;
	_selectedIndex[1] = 0;
	_statsOpen[0] = false;
	_statsOpen[1] = false;
	for (auto& p : _panels) p = nullptr;
	for (auto& p : _statsPanels) p = nullptr;
	for (auto& f : _fingers) f = nullptr;
	for (auto& t : _timeTexts) t = nullptr;
	for (auto& t : _statTexts) t = nullptr;
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
				if (_statsPanels[i]) _statsPanels[i]->SetVisible(false);
				if (_panels[i]) _panels[i]->SetVisible(true);
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
			} else {
				GameState::Menu::MenuOpen[i] = false;
				if (_panels[i]) _panels[i]->SetVisible(false);
				Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
			}
			continue;
		}

		if (_statsOpen[i]) continue;

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
			} else if (idx == 4) {
				Engine::Audio::PlaySFXBuffer("menuSelect", 0.75f);
				openStatsPanel(i);
			}
		}
	}
}
