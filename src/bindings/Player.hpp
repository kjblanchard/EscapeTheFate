#pragma once
#include <bindings/Controller.hpp>
#include <memory>
namespace Etf {
// This is a player that the engine will assign to various systems, holds a virtual controller, which can be Keyboard or joystick
class Player {
   public:
	// Used for input functions
	const Controller& GetController() const;
	Player();

   private:
	std::shared_ptr<Controller> Controller_;
	int PlayerNum_;
	//PlayerSystem manages the players
	friend class PlayerSystem;
	//ImGUI debugging
	friend void DisplayPlayersTab();
};
}  // namespace Etf
