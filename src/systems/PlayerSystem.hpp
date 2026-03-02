#pragma once
#include <bindings/Player.hpp>
#include <memory>
#include <ui/uiObject.hpp>
namespace Etf {

// Functions to be ran by the main for this system.
class PlayerSystem {
   public:
	// Initializes the max num players for the game.
	static void StartPlayerSystem();
	// Handles if someone is trying to update the virtual controller assigned to this player to another player
	static void UpdatePlayerSystem();
	// Cleanup
	static void ShutdownPlayerSystem();

	static const std::shared_ptr<Player>& GetPlayerByNum(int playerNum);

   private:
	static constexpr int MaxNumLocalPlayers_ = 2;
	static std::shared_ptr<Player> Players_[MaxNumLocalPlayers_];
	// TODO we are statically setting this to 4, same with joystick images, prolly use vector
	static std::shared_ptr<Controller> Controllers_[4];
	static struct sUI {
		struct Player1 {
			UIObject* KeyboardImage = nullptr;
			UIObject* JoystickImage = nullptr;
		} Player1;
		struct Player2 {
			UIObject* JoystickImage = nullptr;
			UIObject* AButtonImage = nullptr;
		} Player2;
	} PlayerUIObjectCache_;
	static void GetPlayer1UI();
	static void GetPlayer2UI();
	static void SetStartupInput();

	friend void DisplayPlayersTab();
};

}  // namespace Etf
