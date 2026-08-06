#include <Supergoon/Input/joystick.h>

#include <format>
#include <engine.hpp>
#include <systems/CharacterSelectSystem.hpp>
#include <systems/MouseInputSystem.hpp>
#include <systems/PlayerControllerSystem.hpp>
#include <systems/TitleScreenSystem.hpp>
#include <systems/battleSystem.hpp>
#include <types/ControllerButtons.hpp>
#include <ui/ui.hpp>
#include <ui/uiButton.hpp>
#include <ui/uiImage.hpp>
#include <ui/uiObject.hpp>

#include "gameState.hpp"

using namespace Etf;

namespace {

const int kNumMenuItems = 4;
const int kMenuSpacing = 15;
const bool kMenuItemEnabled[kNumMenuItems] = {true, true, false, false};

UIObject* _menuItems[kNumMenuItems] = {};
UIImage* _finger = nullptr;
UIButton* _buttons[kNumMenuItems] = {};
int _selectedIndex = 0;
bool _initialized = false;
bool _buttonsRegistered = false;

void positionFinger() {
	if (!_finger || !_menuItems[0]) return;
	auto pos = _menuItems[0]->AbsolutePosition();
	auto x = static_cast<int>(pos.x) - 15;
	auto y = static_cast<int>(pos.y) + (kMenuSpacing * _selectedIndex);
	_finger->AbsolutePosition(x, y);
}

void selectMenuItem(int index) {
	if (!kMenuItemEnabled[index]) return;
	if (index == 1) {
		if (SG_GetCurrentNumControllers() < 1) {
			Engine::Audio::PlaySFXBuffer("error1", 0.75f);
			return;
		}
		GameState::IsMultiplayer = true;
		PlayerControllerSystem::AssignControllersForMultiplayer();
	} else {
		GameState::IsMultiplayer = false;
	}
	Engine::Audio::PlaySFXBuffer("menuSelect", 0.75f);
	auto root = UI::GetRootUIObject();
	auto* titlePanel = root->GetChildByName("TitleNineSlice");
	auto* menuPanel = root->GetChildByName("MenuNineSlice");
	if (titlePanel) titlePanel->SetVisible(false);
	if (menuPanel) menuPanel->SetVisible(false);
	_finger->SetVisible(false);
	CharacterSelectSystem::Activate();
}

void unregisterButtons() {
	if (!_buttonsRegistered) return;
	for (int i = 0; i < kNumMenuItems; ++i) {
		if (_buttons[i]) MouseInputSystem::UnregisterButton(_buttons[i]);
		_buttons[i] = nullptr;
	}
	_buttonsRegistered = false;
}

void registerButtons() {
	if (_buttonsRegistered) return;
	for (int i = 0; i < kNumMenuItems; ++i) {
		auto pos = _menuItems[i]->AbsolutePosition();
		UIButtonArgs args;
		args.Rect = {0, 0, pos.w + 15, static_cast<float>(kMenuSpacing)};
		args.Name = std::format("TitleBtn{}", i);
		args.Priority = 10;
		args.Visible = true;
		auto* btn = new UIButton(args);
		int idx = i;
		btn->SetClickCallback([idx]() { selectMenuItem(idx); });
		btn->SetHoverCallback([idx]() {
			if (_selectedIndex != idx) {
				_selectedIndex = idx;
				Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
				positionFinger();
			}
		});
		_menuItems[i]->AddChild(btn);
		// Position relative to parent: offset left to cover the finger area
		btn->SetX(-15);
		btn->SetY(0);
		MouseInputSystem::RegisterButton(btn);
		_buttons[i] = btn;
	}
	_buttonsRegistered = true;
}

}  // namespace

void TitleScreenSystem::Start() {
	_initialized = false;
	_selectedIndex = 0;
	_finger = nullptr;
	for (auto& item : _menuItems) item = nullptr;
	for (auto& btn : _buttons) btn = nullptr;
	_buttonsRegistered = false;
}

void TitleScreenSystem::Update() {
	if (Engine::CurrentSceneName() != "cloud") {
		unregisterButtons();
		_initialized = false;
		return;
	}
	if (!_initialized) {
		auto root = UI::GetRootUIObject();
		if (!root->GetChildByName("CloudPanel")) return;
		_menuItems[0] = root->GetChildByName("NewGameText");
		_menuItems[1] = root->GetChildByName("MultiplayerText");
		_menuItems[2] = root->GetChildByName("LoadText");
		_menuItems[3] = root->GetChildByName("OptionsText");
		_finger = static_cast<UIImage*>(root->GetChildByName("MenuFinger"));
		if (!_menuItems[0] || !_menuItems[1] || !_menuItems[2] || !_menuItems[3] || !_finger) return;
		_initialized = true;
		registerButtons();
		positionFinger();
		return;
	}
	if (GameState::CurrentFadeState != (int)LoadingScreenFadeTypes::NotFading) return;
	if (CharacterSelectSystem::IsActive()) {
		CharacterSelectSystem::Update();
		if (!CharacterSelectSystem::IsActive()) {
			// Returned from character select via B press, restore title UI
			auto root = UI::GetRootUIObject();
			auto* titlePanel = root->GetChildByName("TitleNineSlice");
			auto* menuPanel = root->GetChildByName("MenuNineSlice");
			if (titlePanel) titlePanel->SetVisible(true);
			if (menuPanel) menuPanel->SetVisible(true);
		}
		return;
	}
	_finger->SetVisible(true);
	auto& player = PlayerControllerSystem::GetPlayerByNum(0);
	if (player->IsButtonJustPressed(ControllerButtons::Up)) {
		_selectedIndex = (_selectedIndex - 1 + kNumMenuItems) % kNumMenuItems;
		Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
		positionFinger();
	}

	if (player->IsButtonJustPressed(ControllerButtons::Down)) {
		_selectedIndex = (_selectedIndex + 1) % kNumMenuItems;
		Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
		positionFinger();
	}

	if (player->IsButtonJustPressed(ControllerButtons::A)) {
		selectMenuItem(_selectedIndex);
	}
}
