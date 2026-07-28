#include <Supergoon/Input/joystick.h>

#include <systems/PlayerControllerSystem.hpp>

using namespace Etf;
using namespace PlayerControllerSystem;
using namespace std;

static constexpr int sMaxNumLocalPlayers_ = 2;
static shared_ptr<PlayerController> sPlayers[sMaxNumLocalPlayers_];

void Etf::PlayerControllerSystem::Start() {
	PlayerController::ResetPlayerNumCounter();
	for (auto i = 0; i < sMaxNumLocalPlayers_; ++i) {
		sPlayers[i] = make_shared<PlayerController>();
	}
	auto nConnected = SG_GetCurrentNumControllers();
	for (auto i = 0; i < nConnected && i < sMaxNumLocalPlayers_; ++i) {
		sPlayers[i]->AssignGamepadToController(i);
	}
}

void Etf::PlayerControllerSystem::Update() {
}

void Etf::PlayerControllerSystem::Shutdown() {
	for (auto player : sPlayers) {
		player.reset();
	}
}

const shared_ptr<PlayerController>& Etf::PlayerControllerSystem::GetPlayerByNum(int playerNum) {
	auto playerNumToReturn = playerNum > sMaxNumLocalPlayers_ ? 0 : playerNum;
	return sPlayers[playerNumToReturn];
}
