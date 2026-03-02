#include <Supergoon/Input/joystick.h>
#include <Supergoon/log.h>
#include <assert.h>

#include <systems/PlayerSystem.hpp>
#include <ui/ui.hpp>

#include "ui/uiAnimation.hpp"

using namespace Etf;
using namespace std;

std::shared_ptr<Player> PlayerSystem::Players_[MaxNumLocalPlayers_] = {nullptr, nullptr};
PlayerSystem::sUI PlayerSystem::PlayerUIObjectCache_;
std::shared_ptr<Controller> PlayerSystem::Controllers_[4];

void PlayerSystem::GetPlayer1UI() {
	auto playersGuide = UI::GetRootUIObject()->GetChildByName("PlayersGuide");
	assert(playersGuide && "No players guide");
	auto player1 = playersGuide->GetChildByName("Player1");
	assert(player1 && "No player1 UI");
	PlayerUIObjectCache_.Player1.JoystickImage = player1->GetChildByName("ControllerImage");
	PlayerUIObjectCache_.Player1.KeyboardImage = player1->GetChildByName("KeyboardImage");
	PlayerUIObjectCache_.Player1.JoystickImage->SetVisible(false);
	PlayerUIObjectCache_.Player1.KeyboardImage->SetVisible(true);
}

void PlayerSystem::GetPlayer2UI() {
	auto playersGuide = UI::GetRootUIObject()->GetChildByName("PlayersGuide");
	assert(playersGuide && "No players guide");
	auto player = playersGuide->GetChildByName("Player2");
	assert(player && "No player2 UI");
	PlayerUIObjectCache_.Player2.JoystickImage = player->GetChildByName("ControllerImage");
	PlayerUIObjectCache_.Player2.AButtonImage = player->GetChildByName("SpawnButtonImage");
	PlayerUIObjectCache_.Player2.JoystickImage->SetVisible(false);
	auto animation = static_cast<UIAnimation*>(PlayerUIObjectCache_.Player2.AButtonImage);
	assert(animation && "No animation");
	animation->GetAnimator().StartAnimation("pressed");
	PlayerUIObjectCache_.Player2.AButtonImage->SetVisible(true);
}

void PlayerSystem::SetStartupInput() {
	auto numJoysticks = SG_GetCurrentNumControllers();
	auto p = 0;
	for (auto i = 0; i < numJoysticks; ++i) {
		Controllers_[i]->AssignGamepadToController(i);
		// Assign controller to player initially
		if (p > 1) continue;
		Players_[p]->Controller_->AssignGamepadToController(i);
		if (p == 0) {
			PlayerUIObjectCache_.Player1.JoystickImage->SetVisible(true);
		} else {
			PlayerUIObjectCache_.Player2.JoystickImage->SetVisible(true);
		}
		++p;
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
	// GetGameControllerImages();
	GetPlayer1UI();
	GetPlayer2UI();
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
				// Do not do the other player, cause we just switched.
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
