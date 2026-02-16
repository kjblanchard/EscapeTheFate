#pragma once
namespace Etf {

// Functions to be ran by the main for this system.
class PlayerSystem {
   public:
	// Initializes the max num players for the game.
	void StartPlayerSystem();
	// Handles if someone is trying to update the virtual controller assigned to this player to another player
	void UpdatePlayerSystem();
	// Cleanup
	void ShutdownPlayerSystem();
};

}  // namespace Etf
