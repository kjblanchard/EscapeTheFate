#include <SDL3/SDL_scancode.h>
#include <Supergoon/Input/joystick.h>
#include <Supergoon/Input/keyboard.h>
#include <assert.h>

#include <bindings/Controller.hpp>

using namespace Etf;

// Handle default where we just use default
Controller::Controller() {
	ControllerConfig_.KeyboardKeyConfig[0] = ControllerConfig::DefaultKeyboardbuttons::UP;
	ControllerConfig_.KeyboardKeyConfig[1] = ControllerConfig::DefaultKeyboardbuttons::LEFT;
	ControllerConfig_.KeyboardKeyConfig[2] = ControllerConfig::DefaultKeyboardbuttons::DOWN;
	ControllerConfig_.KeyboardKeyConfig[3] = ControllerConfig::DefaultKeyboardbuttons::RIGHT;
	ControllerConfig_.KeyboardKeyConfig[4] = ControllerConfig::DefaultKeyboardbuttons::A;
	ControllerConfig_.KeyboardKeyConfig[5] = ControllerConfig::DefaultKeyboardbuttons::B;
	ControllerConfig_.KeyboardKeyConfig[6] = ControllerConfig::DefaultKeyboardbuttons::LB;
	ControllerConfig_.KeyboardKeyConfig[7] = ControllerConfig::DefaultKeyboardbuttons::RB;

	ControllerConfig_.JoystickButtonConfig[0] = ControllerConfig::DefaultJoystickButtons::UP;
	ControllerConfig_.JoystickButtonConfig[1] = ControllerConfig::DefaultJoystickButtons::LEFT;
	ControllerConfig_.JoystickButtonConfig[2] = ControllerConfig::DefaultJoystickButtons::DOWN;
	ControllerConfig_.JoystickButtonConfig[3] = ControllerConfig::DefaultJoystickButtons::RIGHT;
	ControllerConfig_.JoystickButtonConfig[4] = ControllerConfig::DefaultJoystickButtons::A;
	ControllerConfig_.JoystickButtonConfig[5] = ControllerConfig::DefaultJoystickButtons::B;
	ControllerConfig_.JoystickButtonConfig[6] = ControllerConfig::DefaultJoystickButtons::LB;
	ControllerConfig_.JoystickButtonConfig[7] = ControllerConfig::DefaultJoystickButtons::RB;
}

bool Controller::IsButtonJustPressed(GameButtons button, int playerNum) const {
	auto keyboardKey = ControllerConfig_.KeyboardKeyConfig[static_cast<int>(button)];
	auto keyboardPressed = IsKeyboardKeyJustPressed(keyboardKey);
	// First player uses keyboard and joystick
	if (playerNum == 0) {
		auto joystickPressed = false;
		if (JoystickAssigned_ != -1) {
			auto joystickButton = ControllerConfig_.JoystickButtonConfig[static_cast<int>(button)];
			joystickPressed = geGamepadButtonJustPressed(JoystickAssigned_, joystickButton);
		}
		return keyboardPressed || joystickPressed;
	}
	return keyboardPressed;
}

bool Controller::IsButtonPressed(GameButtons button, int playerNum) const {
	auto keyboardKey = ControllerConfig_.KeyboardKeyConfig[static_cast<int>(button)];
	auto keyboardPressed = IsKeyboardKeyDown(keyboardKey);
	// First player uses keyboard and joystick
	if (playerNum == 0) {
		auto joystickPressed = false;
		if (JoystickAssigned_ != -1) {
			auto joystickButton = ControllerConfig_.JoystickButtonConfig[static_cast<int>(button)];
			joystickPressed = geGamepadButtonHeldDown(JoystickAssigned_, joystickButton);
		}
		return keyboardPressed || joystickPressed;
	}
	return keyboardPressed;
}
float Controller::JoystickAxisState(JoystickAxis axis) const { return 0; }
float Controller::JoystickAxisThisFrameMovement(JoystickAxis axis) const { return 0; }

bool Controller::IsButtonJustReleased(GameButtons button, int playerNum) const {
	return false;
}

void Controller::AssignGamepadToController(int gamepadNum) {
	JoystickAssigned_ = gamepadNum;
}
