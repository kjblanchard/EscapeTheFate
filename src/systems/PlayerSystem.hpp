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
	//TODO we are statically setting this to 4, same with joystick images, prolly use vector
	static std::shared_ptr<Controller> Controllers_[4];
	static struct sUI {
		UIObject* KeyboardImage = nullptr;
		UIObject* JoystickImages[4] = {nullptr, nullptr, nullptr, nullptr};
	} PlayerUIObjectCache_;
	static void GetGameControllerImages();
	static void GetGameKeyboardImages();
	static void SetImagesToCurrentInput();
	static void SetStartupInput();

	friend void DisplayPlayersTab();
};

}  // namespace Etf
