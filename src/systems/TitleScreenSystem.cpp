#include <Supergoon/Input/joystick.h>

#include <engine.hpp>
#include <memory>
#include <systems/CharacterSelectSystem.hpp>
#include <systems/PlayerControllerSystem.hpp>
#include <systems/PointerInputSystem.hpp>
#include <systems/TitleScreenSystem.hpp>
#include <systems/battleSystem.hpp>
#include <types/ControllerButtons.hpp>
#include <ui/ui.hpp>
#include <ui/uiImage.hpp>
#include <ui/uiObject.hpp>

#include "gameState.hpp"
#include "sgtools/log.h"

using namespace Etf;

namespace {

const int kNumMenuItems = 4;
const int kMenuSpacing = 15;
const bool kMenuItemEnabled[kNumMenuItems] = {true, true, false, false};

UIObject* _menuItems[kNumMenuItems] = {};
UIImage* _finger = nullptr;
int _selectedIndex = 0;
bool _initialized = false;
std::unique_ptr<UIButtonGroup> _buttonGroup;

void positionFinger() {
	if (!_finger || !_menuItems[0]) return;
	auto pos = _menuItems[0]->AbsolutePosition();
	auto x = static_cast<int>(pos.x) - 15;
	auto y = static_cast<int>(pos.y) + (kMenuSpacing * _selectedIndex);
	_finger->AbsolutePosition(x, y);
}

void activateItem(int index) {
	if (!kMenuItemEnabled[index]) {
		sgLogWarn("Index %d is not enabled, %d", index, kMenuItemEnabled[index]);
		Engine::Audio::PlaySFXBuffer("error1", 0.75f);
		return;
	}
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
	if (_buttonGroup) PointerInputSystem::PopGroup(_buttonGroup.get());
	CharacterSelectSystem::Activate();
}

}  // namespace

void TitleScreenSystem::Start() {
	if (_buttonGroup) PointerInputSystem::PopGroup(_buttonGroup.get());
	_buttonGroup.reset();
	_initialized = false;
	_selectedIndex = 0;
	_finger = nullptr;
	for (auto& item : _menuItems) item = nullptr;
}

void TitleScreenSystem::Update() {
	if (Engine::CurrentSceneName() != "cloud") {
		if (_initialized && _buttonGroup) {
			PointerInputSystem::PopGroup(_buttonGroup.get());
			_buttonGroup.reset();
		}
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
		positionFinger();

		_buttonGroup = std::make_unique<UIButtonGroup>(
			[](int index) {
				_selectedIndex = index;
				positionFinger();
				Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
				if (_buttonGroup) _buttonGroup->SetFocusedIndex(index);
			},
			[](int index) {
				activateItem(index);
			});
		for (int i = 0; i < kNumMenuItems; ++i) {
			_buttonGroup->AddButton(_menuItems[i], i);
		}
		_buttonGroup->SetFocusedIndex(_selectedIndex);
		PointerInputSystem::PushGroup(_buttonGroup.get());
		return;
	}
	if (GameState::CurrentFadeState != (int)LoadingScreenFadeTypes::NotFading) return;
	if (CharacterSelectSystem::IsActive()) {
		CharacterSelectSystem::Update();
		if (!CharacterSelectSystem::IsActive()) {
			auto root = UI::GetRootUIObject();
			auto* titlePanel = root->GetChildByName("TitleNineSlice");
			auto* menuPanel = root->GetChildByName("MenuNineSlice");
			if (titlePanel) titlePanel->SetVisible(true);
			if (menuPanel) menuPanel->SetVisible(true);
			if (_buttonGroup) {
				_buttonGroup->SetFocusedIndex(_selectedIndex);
				PointerInputSystem::PushGroup(_buttonGroup.get());
			}
		}
		return;
	}
	_finger->SetVisible(true);
	auto text = static_cast<UIText*>(_menuItems[1]);
	auto color = SG_GetCurrentNumControllers() < 1 ? Color{100, 100, 100, 255} : Color{255, 255, 255, 255};
	text->UpdateDrawColor(color);
	auto& player = PlayerControllerSystem::GetPlayerByNum(0);
	if (player->IsButtonJustPressed(ControllerButtons::Up)) {
		_selectedIndex = (_selectedIndex - 1 + kNumMenuItems) % kNumMenuItems;
		Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
		positionFinger();
		if (_buttonGroup) _buttonGroup->SetFocusedIndex(_selectedIndex);
	}

	if (player->IsButtonJustPressed(ControllerButtons::Down)) {
		_selectedIndex = (_selectedIndex + 1) % kNumMenuItems;
		Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
		positionFinger();
		if (_buttonGroup) _buttonGroup->SetFocusedIndex(_selectedIndex);
	}

	if (player->IsButtonJustPressed(ControllerButtons::A)) {
		activateItem(_selectedIndex);
	}
}
