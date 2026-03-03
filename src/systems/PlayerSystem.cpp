#include <Supergoon/Input/joystick.h>
#include <Supergoon/log.h>
#include <assert.h>

#include <systems/PlayerSystem.hpp>
#include <ui/ui.hpp>

#include "gameState.hpp"
#include "ui/uiAnimation.hpp"

using namespace Etf;
using namespace std;

static constexpr int sMaxNumLocalPlayers_ = 2;
static std::shared_ptr<Player> sPlayers[sMaxNumLocalPlayers_];
// TODO we are statically setting this to 4, same with joystick images, prolly use vector
static std::shared_ptr<Controller> sControllers[4];
static struct sUI {
	struct Player1 {
		UIObject* KeyboardImage = nullptr;
		UIObject* JoystickImage = nullptr;
	} Player1;
	struct Player2 {
		UIObject* JoystickImage = nullptr;
		UIAnimation* AButtonAnimation = nullptr;
	} Player2;
} sPlayerUIObjectCache;

static void getPlayer1UI() {
	auto playersGuide = UI::GetRootUIObject()->GetChildByName("PlayersGuide");
	assert(playersGuide && "No players guide");
	auto player1 = playersGuide->GetChildByName("Player1");
	assert(player1 && "No player1 UI");
	sPlayerUIObjectCache.Player1.JoystickImage = player1->GetChildByName("ControllerImage");
	sPlayerUIObjectCache.Player1.KeyboardImage = player1->GetChildByName("KeyboardImage");
	sPlayerUIObjectCache.Player1.JoystickImage->SetVisible(false);
	sPlayerUIObjectCache.Player1.KeyboardImage->SetVisible(true);
}

static void getPlayer2UI() {
	auto playersGuide = UI::GetRootUIObject()->GetChildByName("PlayersGuide");
	auto& player2 = sPlayerUIObjectCache.Player2;
	assert(playersGuide && "No players guide");
	auto player = playersGuide->GetChildByName("Player2");
	assert(player && "No player2 UI");
	player2.JoystickImage = player->GetChildByName("ControllerImage");
	player2.AButtonAnimation = static_cast<UIAnimation*>(player->GetChildByName("SpawnButtonImage"));
	player2.JoystickImage->SetVisible(false);
	player2.AButtonAnimation->SetVisible(false);
	assert(player2.JoystickImage && player2.AButtonAnimation && "Could not load player2 stuff");
}

static void setStartupInput() {
	auto nJoysticks = SG_GetCurrentNumControllers();
	auto pNum = 0;
	for (auto i = 0; i < nJoysticks; ++i) {
		sControllers[i]->AssignGamepadToController(i);
		if (pNum > 1) continue;
		if (pNum == 0) {
			sPlayerUIObjectCache.Player1.JoystickImage->SetVisible(true);
		} else {
			sPlayerUIObjectCache.Player2.JoystickImage->SetVisible(true);
		}
		++pNum;
	}
}

static void handlePlayer2ButtonPress(bool isVisible, UIAnimation& player2Anim) {
	if (isVisible) {
		player2Anim.SetVisible(true);
		player2Anim.GetAnimator().StartAnimation("pressed");
	} else {
		player2Anim.SetVisible(false);
	}
}

void Etf::StartPlayerSystem() {
	for (auto i = 0; i < geGamepadMaxPads(); ++i) {
		sControllers[i] = make_shared<Controller>();
	}
	for (auto i = 0; i < sMaxNumLocalPlayers_; ++i) {
		sPlayers[i] = make_shared<Player>();
		sPlayers[i]->Controller_ = sControllers[i];
	}
	getPlayer1UI();
	getPlayer2UI();
	setStartupInput();
}

void Etf::UpdatePlayerSystem() {
	// Listen for input to reassign to other players
	bool switched = false;
	for (auto i = 0; i < sMaxNumLocalPlayers_; ++i) {
		auto& player = sPlayers[i];
		auto otherPlayer = i == 0 ? i + 1 : i - 1;
		auto& otherPlayerThing = sPlayers[otherPlayer];
		auto& controller = player->Controller_;
		if (controller->IsButtonJustPressed(GameButtons::RB)) {
			sgLogDebug("Trying to reassign controller to next player, if there is a input attached");
			if (controller->DoesGamepadHaveJoystickAssigned()) {
				auto nController = controller->DoesGamepadHaveJoystickAssigned();
				sgLogDebug("Reassigning controller %d", nController);
				controller->AssignGamepadToController(-1);
				otherPlayerThing->Controller_->AssignGamepadToController(nController);
				switched = true;
				break;
			}
		}
	}
	if (!switched) return;
	for (auto i = 0; i < sMaxNumLocalPlayers_; ++i) {
		auto isP1 = i == 0;
		auto joyImage = isP1 ? sPlayerUIObjectCache.Player1.JoystickImage : sPlayerUIObjectCache.Player2.JoystickImage;
		auto visible = sPlayers[i]->Controller_->DoesGamepadHaveJoystickAssigned();
		joyImage->SetVisible(visible);
		auto updateP2Button = !isP1 && !GameState::Players::Player2Spawned;
		if (!updateP2Button) continue;
		handlePlayer2ButtonPress(visible, *sPlayerUIObjectCache.Player2.AButtonAnimation);
	}
}

void Etf::ShutdownPlayerSystem() {
	for (auto player : sPlayers) {
		player.reset();
	}
}

const shared_ptr<Player>& Etf::GetPlayerByNum(int playerNum) {
	auto playerNumToReturn = playerNum > sMaxNumLocalPlayers_ ? 0 : playerNum;
	return sPlayers[playerNumToReturn];
}
