#include <Supergoon/Input/joystick.h>
#include <Supergoon/Input/keyboard.h>
#include <assert.h>

#include <bindings/Controller.hpp>

using namespace Etf;

const int MAX_NUM_CONTROLLERS = 2;

static Controller* _controllers[MAX_NUM_CONTROLLERS] = {nullptr};

void Controller::InitializeControllers() {
	for (int i = 0; i < MAX_NUM_CONTROLLERS; ++i) {
		_controllers[i] = new Controller();
	}
}

// Handle default where we just use default
Controller::Controller() {
	_controllerConfig.KeyboardKeyConfig[0] = ControllerConfig::DefaultKeyboardbuttons::UP;
	_controllerConfig.KeyboardKeyConfig[1] = ControllerConfig::DefaultKeyboardbuttons::LEFT;
	_controllerConfig.KeyboardKeyConfig[2] = ControllerConfig::DefaultKeyboardbuttons::DOWN;
	_controllerConfig.KeyboardKeyConfig[3] = ControllerConfig::DefaultKeyboardbuttons::RIGHT;
	_controllerConfig.KeyboardKeyConfig[4] = ControllerConfig::DefaultKeyboardbuttons::A;
	_controllerConfig.KeyboardKeyConfig[5] = ControllerConfig::DefaultKeyboardbuttons::B;

	_controllerConfig.JoystickButtonConfig[0] = ControllerConfig::DefaultJoystickButtons::UP;
	_controllerConfig.JoystickButtonConfig[1] = ControllerConfig::DefaultJoystickButtons::LEFT;
	_controllerConfig.JoystickButtonConfig[2] = ControllerConfig::DefaultJoystickButtons::DOWN;
	_controllerConfig.JoystickButtonConfig[3] = ControllerConfig::DefaultJoystickButtons::RIGHT;
	_controllerConfig.JoystickButtonConfig[4] = ControllerConfig::DefaultJoystickButtons::A;
	_controllerConfig.JoystickButtonConfig[5] = ControllerConfig::DefaultJoystickButtons::B;
}


bool Controller::IsButtonJustPressed(GameButtons button, int playerNum) {
	auto controller = _controllers[playerNum];
	assert(controller && "Is real");
	auto keyboardKey = controller->_controllerConfig.KeyboardKeyConfig[static_cast<int>(button)];
	auto keyboardPressed = IsKeyboardKeyJustPressed(keyboardKey);
	// First player uses keyboard and joystick
	if (playerNum == 0) {
		auto joystickPressed = false;
		if (controller->_joystickAssigned != -1) {
			auto joystickButton = controller->_controllerConfig.JoystickButtonConfig[static_cast<int>(button)];
			joystickPressed = geGamepadButtonJustPressed(controller->_joystickAssigned, joystickButton);
		}
		return keyboardPressed || joystickPressed;
	}
	return keyboardPressed;
}

bool Controller::IsButtonPressed(GameButtons button, int playerNum) {
	auto controller = _controllers[playerNum];
	assert(controller && "Is real");
	auto keyboardKey = controller->_controllerConfig.KeyboardKeyConfig[static_cast<int>(button)];
	auto keyboardPressed = IsKeyboardKeyDown(keyboardKey);
	// First player uses keyboard and joystick
	if (playerNum == 0) {
		auto joystickPressed = false;
		if (controller->_joystickAssigned != -1) {
			auto joystickButton = controller->_controllerConfig.JoystickButtonConfig[static_cast<int>(button)];
			joystickPressed = geGamepadButtonHeldDown(controller->_joystickAssigned, joystickButton);
		}
		return keyboardPressed || joystickPressed;
	}
	return keyboardPressed;
}

bool Controller::IsButtonJustReleased(GameButtons button, int playerNum) {
	return false;
}

void Controller::AssignGamepadToController(int gamepadNum) {}
