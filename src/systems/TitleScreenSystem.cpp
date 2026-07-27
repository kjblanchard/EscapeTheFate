#include <engine.hpp>
#include <systems/PlayerControllerSystem.hpp>
#include <systems/TitleScreenSystem.hpp>
#include <types/ControllerButtons.hpp>
#include <ui/ui.hpp>
#include <ui/uiImage.hpp>
#include <ui/uiObject.hpp>

using namespace Etf;

namespace {

const int kNumMenuItems = 3;
const int kMenuSpacing = 15;
const bool kMenuItemEnabled[kNumMenuItems] = {true, false, false};

UIObject* _menuItems[kNumMenuItems] = {};
UIImage* _finger = nullptr;
int _selectedIndex = 0;
bool _initialized = false;

void positionFinger() {
	if (!_finger || !_menuItems[0]) return;
	auto pos = _menuItems[0]->AbsolutePosition();
	auto x = pos.x - 15;
	auto y = pos.y + (kMenuSpacing * _selectedIndex);
	_finger->AbsolutePosition(x, y);
}

}  // namespace

void TitleScreenSystem::Start() {
	_initialized = false;
	_selectedIndex = 0;
	_finger = nullptr;
	for (auto& item : _menuItems) item = nullptr;
}

void TitleScreenSystem::Update() {
	if (Engine::CurrentSceneName() != "cloud") return;

	if (!_initialized) {
		auto root = UI::GetRootUIObject();
		if (!root->GetChildByName("CloudPanel")) return;

		_menuItems[0] = root->GetChildByName("NewGameText");
		_menuItems[1] = root->GetChildByName("LoadText");
		_menuItems[2] = root->GetChildByName("OptionsText");
		_finger = static_cast<UIImage*>(root->GetChildByName("MenuFinger"));

		if (!_menuItems[0] || !_menuItems[1] || !_menuItems[2] || !_finger) return;

		_initialized = true;
		positionFinger();
		return;
	}

	auto& player = PlayerControllerSystem::GetPlayerByNum(0);

	if (player->IsButtonJustPressed(ControllerButtons::Up)) {
		int next = _selectedIndex;
		do {
			next = (next - 1 + kNumMenuItems) % kNumMenuItems;
		} while (!kMenuItemEnabled[next] && next != _selectedIndex);
		if (next != _selectedIndex) {
			_selectedIndex = next;
			positionFinger();
		}
	}

	if (player->IsButtonJustPressed(ControllerButtons::Down)) {
		int next = _selectedIndex;
		do {
			next = (next + 1) % kNumMenuItems;
		} while (!kMenuItemEnabled[next] && next != _selectedIndex);
		if (next != _selectedIndex) {
			_selectedIndex = next;
			positionFinger();
		}
	}

	if (player->IsButtonJustPressed(ControllerButtons::A)) {
		if (_selectedIndex == 0) {
			Engine::LoadScene("debugTown");
		}
	}
}
