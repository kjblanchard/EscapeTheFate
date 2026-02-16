#include <bindings/Player.hpp>
#include <memory>
#include <systems/PlayerSystem.hpp>
using namespace Etf;
using namespace std;

static constexpr int kMaxNumLocalPlayers = 2;
static shared_ptr<Player> kPlayers[kMaxNumLocalPlayers];

void PlayerSystem::StartPlayerSystem() {
	for (auto i = 0; i < kMaxNumLocalPlayers; ++i) {
		kPlayers[i] = make_shared<Player>();
	}
}

void PlayerSystem::UpdatePlayerSystem() {
	// Listen for input to reassign to other players
}

void PlayerSystem::ShutdownPlayerSystem() {
	for (auto player : kPlayers) {
		player.reset();
	}
}
