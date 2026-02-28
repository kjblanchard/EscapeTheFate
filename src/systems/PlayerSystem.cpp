#include <Supergoon/Input/joystick.h>
#include <Supergoon/log.h>

#include <format>
#include <systems/PlayerSystem.hpp>
#include <ui/ui.hpp>

using namespace Etf;
using namespace std;

std::shared_ptr<Player> PlayerSystem::Players_[MaxNumLocalPlayers_] = {nullptr, nullptr};
PlayerSystem::sUI PlayerSystem::PlayerUIObjectCache_;
std::shared_ptr<Controller> PlayerSystem::Controllers_[4];

void PlayerSystem::GetGameControllerImages() {
	auto gameControllerImages = UI::GetRootUIObject()->GetChildByName("ControllerImages");
	if (!gameControllerImages) {
		sgLogCritical("Could Not find controller images from UI, exiting");
	}
	for (auto i = 0; i < geGamepadMaxPads(); ++i) {
		auto imageNum = i + 1;
		auto controllerImageObject = gameControllerImages->GetChildByName(format("ControllerImage{}", to_string(imageNum)));
		if (!controllerImageObject) {
			sgLogCritical("Could Not find controller image from UI, for num %d exiting", imageNum);
		}
		PlayerUIObjectCache_.JoystickImages[i] = controllerImageObject;
		controllerImageObject->SetVisible(false);
	}
}

void PlayerSystem::GetGameKeyboardImages() {
	auto keyboardObject = UI::GetRootUIObject()->GetChildByName("KeyboardImage");
	if (!keyboardObject) {
		sgLogCritical("Could Not find keyboard images from UI, exiting");
	}
	keyboardObject->SetVisible(false);
	PlayerUIObjectCache_.KeyboardImage = keyboardObject;
}

void PlayerSystem::SetImagesToCurrentInput() {
	PlayerUIObjectCache_.KeyboardImage->SetVisible(true);
	auto numJoysticks = SG_GetCurrentNumControllers();
	for (auto i = 0; i < numJoysticks; ++i) {
		PlayerUIObjectCache_.JoystickImages[i]->SetVisible(true);
	}
}

void PlayerSystem::SetStartupInput() {
	auto numJoysticks = SG_GetCurrentNumControllers();
	for (auto i = 0; i < numJoysticks; ++i) {
		PlayerUIObjectCache_.JoystickImages[i]->SetVisible(true);
		Controllers_[i]->AssignGamepadToController(i);
		Players_[0]->Controller_->AssignGamepadToController(i);
	}
}

void PlayerSystem::StartPlayerSystem() {
	for (auto i = 0; i < geGamepadMaxPads(); ++i) {
		Controllers_[i] = make_shared<Controller>();
	}

	for (auto i = 0; i < MaxNumLocalPlayers_; ++i) {
		Players_[i] = make_shared<Player>();
		Players_[i]->Controller_ = Controllers_[i];
	}
	GetGameControllerImages();
	GetGameKeyboardImages();
	SetImagesToCurrentInput();
	SetStartupInput();
}
void PlayerSystem::UpdatePlayerSystem() {
	// Listen for input to reassign to other players
	for (auto i = 0; i < MaxNumLocalPlayers_; ++i) {
		auto& player = Players_[i];
		auto otherPlayer = i == 0 ? i + 1 : i - 1;
		auto& otherPlayerThing = Players_[otherPlayer];
		auto& controller = player->Controller_;
		if (controller->IsButtonJustPressed(GameButtons::RB)) {
			sgLogDebug("Trying to reassign controller to next player, if there is a input attached");
			if (controller->DoesGamepadHaveJoystickAssigned()) {
				sgLogDebug("Reassigning");
				auto controllerToReassign = controller->JoystickAssigned_;
				controller->AssignGamepadToController(-1);
				otherPlayerThing->Controller_->AssignGamepadToController(controllerToReassign);
				//Do not do the other player, cause we just switched.
				return;
			}
		}
	}
}

void PlayerSystem::ShutdownPlayerSystem() {
	for (auto player : Players_) {
		player.reset();
	}
}

const shared_ptr<Player>& PlayerSystem::GetPlayerByNum(int playerNum) {
	auto playerNumToReturn = playerNum > MaxNumLocalPlayers_ ? 0 : playerNum;
	return Players_[playerNumToReturn];
}
