#pragma once
#include <array>
#include <interfaces/IController.hpp>
namespace Etf {
struct ControllerConfig {
};

// This is a player that the engine will assign to various systems, holds a virtual controller, which can be Keyboard or joystick
class PlayerController : public IController {
   public:
	PlayerController();
	bool IsButtonPressed(ControllerButtons button) const final;
	bool IsButtonJustPressed(ControllerButtons button) const final;
	bool IsButtonJustReleased(ControllerButtons button) const final;
	float JoystickAxisState(JoystickAxis axis) const final;
	float JoystickAxisThisFrameMovement(JoystickAxis axis) const final;
	void AssignGamepadToController(int gamepadNum);
	static void ResetPlayerNumCounter();

   private:
	ControllerConfig ControllerConfig_;
	int PlayerNum_;
	int JoystickAssigned_ = -1;
	std::array<int, static_cast<int>(ControllerButtons::NUM_GAME_BUTTONS)> KeyboardKeyConfig_;
	std::array<int, static_cast<int>(ControllerButtons::NUM_GAME_BUTTONS)> JoystickButtonConfig_;

	friend void DisplayPlayerControllerTab();
};
}  // namespace Etf
