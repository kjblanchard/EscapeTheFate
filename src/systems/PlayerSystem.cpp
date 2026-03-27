#include <Supergoon/Input/joystick.h>
#include <sgtools/log.h>

#include <cassert>
#include <gameState.hpp>
#include <systems/PlayerSystem.hpp>
#include <ui/ui.hpp>
#include <ui/uiAnimation.hpp>

using namespace Etf;
using namespace std;

static constexpr int sMaxNumLocalPlayers_ = 2;	// only support 2 in local mode.
static constexpr int sMaxNumControllers = 4;	// match engine max controllers
static shared_ptr<Player> sPlayers[sMaxNumLocalPlayers_];
static shared_ptr<Controller> sControllers[sMaxNumControllers];
static struct PlayersUI {
	struct {
		UIObject* KeyboardImage = nullptr;
		UIObject* JoystickImage = nullptr;
	} Player1;
	struct {
		UIObject* JoystickImage = nullptr;
		UIAnimation* AButtonAnimation = nullptr;
	} Player2;
} sPlayersUI;

static void getPlayer1UI() {
	auto playersGuide = UI::GetRootUIObject()->GetChildByName("PlayersGuide");
	assert(playersGuide && "No players guide");
	auto player1 = playersGuide->GetChildByName("Player1");
	assert(player1 && "No player1 UI");
	sPlayersUI.Player1.JoystickImage = player1->GetChildByName("ControllerImage");
	sPlayersUI.Player1.KeyboardImage = player1->GetChildByName("KeyboardImage");
	sPlayersUI.Player1.JoystickImage->SetVisible(false);
	sPlayersUI.Player1.KeyboardImage->SetVisible(true);
}

static void getPlayer2UI() {
	auto playersGuide = UI::GetRootUIObject()->GetChildByName("PlayersGuide");
	auto& player2 = sPlayersUI.Player2;
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
			sPlayersUI.Player1.JoystickImage->SetVisible(true);
		} else {
			sPlayersUI.Player2.JoystickImage->SetVisible(true);
		}
		++pNum;
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

static void handlePlayer2ButtonPress(bool isVisible, UIAnimation& player2Anim) {
	if (isVisible) {
		player2Anim.SetVisible(true);
		player2Anim.GetAnimator().StartAnimation("pressed");
	} else {
		player2Anim.SetVisible(false);
	}
}

static void handleControllerSwitch(int playerNum, shared_ptr<Controller>& controller, shared_ptr<Controller>& otherController) {
	sgLogDebug("Trying to reassign controller to next player");
	if (controller->DoesGamepadHaveJoystickAssigned()) {
		auto joystickID = controller->Joystick();
		sgLogDebug("Reassigning joystick %d from player %d", joystickID, playerNum);
		controller->AssignGamepadToController(-1);
		otherController->AssignGamepadToController(joystickID);
	}
}

static void updateUI() {
	for (auto i = 0; i < sMaxNumLocalPlayers_; ++i) {
		auto isP1 = i == 0;
		auto joyImage = isP1 ? sPlayersUI.Player1.JoystickImage : sPlayersUI.Player2.JoystickImage;
		auto visible = sPlayers[i]->GetController().DoesGamepadHaveJoystickAssigned();
		joyImage->SetVisible(visible);
		auto updateP2Button = !isP1 && !GameState::Players::Player2Spawned;
		if (!updateP2Button) continue;
		handlePlayer2ButtonPress(visible, *sPlayersUI.Player2.AButtonAnimation);
	}
}

void Etf::UpdatePlayerSystem() {
	for (auto i = 0; i < sMaxNumLocalPlayers_; ++i) {
		auto& player = sPlayers[i];
		auto& controller = player->Controller_;
		if (controller->IsButtonJustPressed(GameButtons::RB)) {
			auto otherPNum = i == 0 ? i + 1 : i - 1;
			auto& otherPController = sPlayers[otherPNum]->Controller_;
			handleControllerSwitch(i, controller, otherPController);
			updateUI();
			break;
		}
	}
	auto& p2Controller = sPlayers[1]->GetController();
	if (p2Controller.IsButtonJustPressed(GameButtons::A) && !GameState::Players::Player2Spawned) {
		sgLogDebug("Should spawn the player 2!");
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
