#include <Supergoon/Input/joystick.h>

#include <engine.hpp>
#include <gameState.hpp>
#include <systems/CharacterSelectSystem.hpp>
#include <systems/PlayerControllerSystem.hpp>
#include <systems/TitleScreenSystem.hpp>
#include <systems/battleSystem.hpp>
#include <types/ControllerButtons.hpp>
#include <ui/ui.hpp>
#include <ui/uiImage.hpp>
#include <ui/uiObject.hpp>

using namespace Etf;
using namespace std;

namespace {

constexpr int kNumMenuItems = 4;
constexpr int kMenuSpacing = 15;
constexpr bool kMenuItemEnabled[kNumMenuItems] = {true, true, false, false};
constexpr const char* kTitlePanelName = "TitleNineSlice";
constexpr const char* kMenuPanelName = "MenuNineSlice";

UIObject* menuItems_[kNumMenuItems] = {};
UIImage* finger_ = nullptr;
int selectedIndex_ = 0;
bool initialized_ = false;

void positionFinger() {
	if (!finger_ || !menuItems_[0]) return;
	auto pos = menuItems_[0]->AbsolutePosition();
	auto x = static_cast<int>(pos.x) - 15;
	auto y = static_cast<int>(pos.y) + (kMenuSpacing * selectedIndex_);
	finger_->AbsolutePosition(x, y);
}

}  // namespace

void TitleScreenSystem::Start() {
	initialized_ = false;
	selectedIndex_ = 0;
	finger_ = nullptr;
	for (auto& item : menuItems_) item = nullptr;
}

void TitleScreenSystem::Update() {
	if (Engine::CurrentSceneName() != "cloud") {
		initialized_ = false;
		return;
	}
	if (!initialized_) {
		auto root = UI::GetRootUIObject();
		if (!root->GetChildByName("CloudPanel")) return;
		menuItems_[0] = root->GetChildByName("NewGameText");
		menuItems_[1] = root->GetChildByName("MultiplayerText");
		menuItems_[2] = root->GetChildByName("LoadText");
		menuItems_[3] = root->GetChildByName("OptionsText");
		finger_ = static_cast<UIImage*>(root->GetChildByName("MenuFinger"));
		if (!menuItems_[0] || !menuItems_[1] || !menuItems_[2] || !menuItems_[3] || !finger_) return;
		initialized_ = true;
		positionFinger();
		return;
	}
	if (GameState::CurrentFadeState != (int)LoadingScreenFadeTypes::NotFading) return;
	if (CharacterSelectSystem::IsActive()) {
		CharacterSelectSystem::Update();
		if (!CharacterSelectSystem::IsActive()) {
			auto root = UI::GetRootUIObject();
			auto* titlePanel = root->GetChildByName(kTitlePanelName);
			auto* menuPanel = root->GetChildByName(kMenuPanelName);
			if (titlePanel) titlePanel->SetVisible(true);
			if (menuPanel) menuPanel->SetVisible(true);
		}
		return;
	}
	finger_->SetVisible(true);
	auto& player = PlayerControllerSystem::GetPlayerByNum(0);
	if (player->IsButtonJustPressed(ControllerButtons::Up)) {
		selectedIndex_ = (selectedIndex_ - 1 + kNumMenuItems) % kNumMenuItems;
		Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
		positionFinger();
	}

	if (player->IsButtonJustPressed(ControllerButtons::Down)) {
		selectedIndex_ = (selectedIndex_ + 1) % kNumMenuItems;
		Engine::Audio::PlaySFXBuffer("menuMove", 0.75f);
		positionFinger();
	}

	if (player->IsButtonJustPressed(ControllerButtons::A)) {
		if (kMenuItemEnabled[selectedIndex_]) {
			if (selectedIndex_ == 1) {
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
			auto* titlePanel = root->GetChildByName(kTitlePanelName);
			auto* menuPanel = root->GetChildByName(kMenuPanelName);
			if (titlePanel) titlePanel->SetVisible(false);
			if (menuPanel) menuPanel->SetVisible(false);
			finger_->SetVisible(false);
			CharacterSelectSystem::Activate();
		}
	}
}
