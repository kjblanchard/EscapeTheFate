#pragma once
#include <bindings/Controller.hpp>
namespace Etf {
// This is a player that the engine will assign to various systems, holds a virtual controller, which can be Keyboard or joystick
class Player {
   public:
	// Used for input functions
	const Controller& GetController() const;

   private:
	class Controller Controller_;
	friend class PlayerSystem;
};
}  // namespace Etf
