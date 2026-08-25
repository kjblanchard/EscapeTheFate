#include <engine.hpp>
#include <gameState.hpp>
#include <systems/LanMultiplayerSystem.hpp>
#include <systems/PlayerControllerSystem.hpp>
#include <types/ControllerButtons.hpp>
#include <ui/ui.hpp>
#include <ui/uiImage.hpp>
#include <ui/uiNineSlice.hpp>
#include <ui/uiObject.hpp>
#include <ui/uiText.hpp>
#include <ui/uiTextInput.hpp>

#include "sgtools/log.h"

using namespace Etf;
using namespace std;

namespace {

enum class LanState {
	ChoosingRole,
	EnteringIP,
};

const char* kPanelName = "LanMpPanel";
const int kNumItems = 2;
const int kItemSpacing = 15;

bool _active = false;
bool _uiBuilt = false;
LanState _state = LanState::ChoosingRole;
int _selectedIndex = 0;

UIObject* _panel = nullptr;
UIText* _menuItems[kNumItems] = {};
UIImage* _finger = nullptr;
UITextInput* _ipInput = nullptr;
UIText* _hintText = nullptr;

void positionFinger() {
	if (!_finger || !_menuItems[0]) return;
	auto pos = _menuItems[0]->AbsolutePosition();
	auto x = static_cast<int>(pos.x) - 15;
	auto y = static_cast<int>(pos.y) + (kItemSpacing * _selectedIndex);
	_finger->AbsolutePosition(x, y);
}

void buildUI() {
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
	bgArgs.Name = "LanMpBG";
	bgArgs.Rect = {120, 60, 240, 150};
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
	titleArgs.Rect = {0, 10, 240, 16};
	titleArgs.TextToDraw = "~ LAN Multiplayer ~";
	titleArgs.Name = "LanMpTitle";
	titleArgs.NumCharsToDraw = 100;
	titleArgs.Priority = 2;
	titleArgs.TextColor = {255, 255, 200, 255};
	titleArgs.CenteredX = true;
	titleArgs.CenteredY = false;
	titleArgs.WordWrap = false;
	titleArgs.Visible = true;
	bg->AddChild(new UIText(titleArgs));

	UITextArgs hostArgs;
	hostArgs.FontName = "PressStart2P";
	hostArgs.FontSize = 8;
	hostArgs.Rect = {40, 40, 160, 12};
	hostArgs.TextToDraw = "Host";
	hostArgs.Name = "LanHostText";
	hostArgs.NumCharsToDraw = 100;
	hostArgs.Priority = 2;
	hostArgs.TextColor = {255, 255, 255, 255};
	hostArgs.CenteredX = true;
	hostArgs.CenteredY = true;
	hostArgs.WordWrap = false;
	hostArgs.Visible = true;
	_menuItems[0] = new UIText(hostArgs);
	bg->AddChild(_menuItems[0]);

	UITextArgs connectArgs;
	connectArgs.FontName = "PressStart2P";
	connectArgs.FontSize = 8;
	connectArgs.Rect = {40, 55, 160, 12};
	connectArgs.TextToDraw = "Connect";
	connectArgs.Name = "LanConnectText";
	connectArgs.NumCharsToDraw = 100;
	connectArgs.Priority = 2;
	connectArgs.TextColor = {255, 255, 255, 255};
	connectArgs.CenteredX = true;
	connectArgs.CenteredY = true;
	connectArgs.WordWrap = false;
	connectArgs.Visible = true;
	_menuItems[1] = new UIText(connectArgs);
	bg->AddChild(_menuItems[1]);

	UIImageArgs fingerArgs;
	fingerArgs.Filename = "fingers";
	fingerArgs.Name = "LanFinger";
	fingerArgs.Rect = {25, 40, 16, 16};
	fingerArgs.SourceRect = {16, 48, 16, 16};
	fingerArgs.Scale = 1.0f;
	fingerArgs.DrawColor = {255, 255, 255, 255};
	fingerArgs.Priority = 3;
	fingerArgs.Visible = true;
	_finger = new UIImage(fingerArgs);
	bg->AddChild(_finger);

	UITextInputArgs ipArgs;
	ipArgs.FontName = "PressStart2P";
	ipArgs.FontSize = 8;
	ipArgs.Rect = {20, 85, 200, 16};
	ipArgs.Placeholder = "Enter IP address...";
	ipArgs.Name = "LanIpInput";
	ipArgs.Priority = 2;
	ipArgs.MaxLength = 45;
	ipArgs.Visible = false;
	ipArgs.TextColor = {255, 255, 255, 255};
	_ipInput = new UITextInput(ipArgs);
	bg->AddChild(_ipInput);

	UITextArgs hintArgs;
	hintArgs.FontName = "PressStart2P";
	hintArgs.FontSize = 8;
	hintArgs.Rect = {0, 115, 240, 12};
	hintArgs.TextToDraw = "Enter:OK  Esc:Back";
	hintArgs.Name = "LanHintText";
	hintArgs.NumCharsToDraw = 100;
	hintArgs.Priority = 2;
	hintArgs.TextColor = {150, 150, 150, 255};
	hintArgs.CenteredX = true;
	hintArgs.CenteredY = false;
	hintArgs.WordWrap = false;
	hintArgs.Visible = false;
	_hintText = new UIText(hintArgs);
	bg->AddChild(_hintText);

	_uiBuilt = true;
	positionFinger();
}

void destroyUI() {
	if (!_uiBuilt) return;
	auto root = UI::GetRootUIObject();
	root->DestroyChildByName(kPanelName);
	_panel = nullptr;
	_menuItems[0] = nullptr;
	_menuItems[1] = nullptr;
	_finger = nullptr;
	_ipInput = nullptr;
	_hintText = nullptr;
	_uiBuilt = false;
}

void enterIPState() {
	_state = LanState::EnteringIP;
	_menuItems[0]->SetVisible(false);
	_menuItems[1]->SetVisible(false);
	_finger->SetVisible(false);
	_ipInput->SetVisible(true);
	_ipInput->Clear();
	_ipInput->Focus();
	_hintText->SetVisible(true);

	_ipInput->OnSubmit = [](const string& ip) {
		GameState::LanServerIP = ip;
		sgLogWarn("LAN Connect: would connect to %s", ip.c_str());
		_ipInput->Unfocus();
		LanMultiplayerSystem::Deactivate();
	};
	_ipInput->OnCancel = []() {
		_ipInput->Unfocus();
		_state = LanState::ChoosingRole;
		_menuItems[0]->SetVisible(true);
		_menuItems[1]->SetVisible(true);
		_finger->SetVisible(true);
		_ipInput->SetVisible(false);
		_hintText->SetVisible(false);
	};
}

}  // namespace

void LanMultiplayerSystem::Activate() {
	_active = true;
	_state = LanState::ChoosingRole;
	_selectedIndex = 0;
	buildUI();
}

void LanMultiplayerSystem::Deactivate() {
	if (_ipInput && _ipInput->IsFocused()) {
		_ipInput->Unfocus();
	}
	destroyUI();
	_active = false;
}

bool LanMultiplayerSystem::IsActive() {
	return _active;
}

void LanMultiplayerSystem::Update() {
	if (!_active) return;

	if (_state == LanState::EnteringIP) {
		_ipInput->Update();
		return;
	}

	auto& player = PlayerControllerSystem::GetPlayerByNum(0);

	if (player->IsButtonJustPressed(ControllerButtons::B)) {
		Deactivate();
		return;
	}

	if (player->IsButtonJustPressed(ControllerButtons::Up)) {
		_selectedIndex = (_selectedIndex - 1 + kNumItems) % kNumItems;
		Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
		positionFinger();
	}

	if (player->IsButtonJustPressed(ControllerButtons::Down)) {
		_selectedIndex = (_selectedIndex + 1) % kNumItems;
		Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
		positionFinger();
	}

	if (player->IsButtonJustPressed(ControllerButtons::A)) {
		Engine::Audio::PlaySFXBuffer("menuSelect", 0.75f);
		if (_selectedIndex == 0) {
			GameState::IsLanHost = true;
			sgLogWarn("LAN Host: not yet implemented");
		} else if (_selectedIndex == 1) {
			enterIPState();
		}
	}
}
