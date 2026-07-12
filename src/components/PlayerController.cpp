#include <components/PlayerController.hpp>
#include <interfaces/IController.hpp>
#include <Supergoon/Input/joystick.h>
#include <Supergoon/Input/keyboard.h>
#include <gameConfig.hpp>
using namespace Etf;

static int sCurrentPlayerNum_ = 0;

PlayerController::PlayerController() : PlayerNum_(sCurrentPlayerNum_++) {
	auto& gameconfig = GameConfig::GetGameConfig();
	KeyboardKeyConfig_[0] = gameconfig.Controls.Keyboard.UP;
	KeyboardKeyConfig_[1] = gameconfig.Controls.Keyboard.LEFT;
	KeyboardKeyConfig_[2] = gameconfig.Controls.Keyboard.DOWN;
	KeyboardKeyConfig_[3] = gameconfig.Controls.Keyboard.RIGHT;
	KeyboardKeyConfig_[4] = gameconfig.Controls.Keyboard.A;
	KeyboardKeyConfig_[5] = gameconfig.Controls.Keyboard.B;
	KeyboardKeyConfig_[6] = gameconfig.Controls.Keyboard.LB;
	KeyboardKeyConfig_[7] = gameconfig.Controls.Keyboard.RB;

	JoystickButtonConfig_[0] = gameconfig.Controls.Keyboard.UP;
	JoystickButtonConfig_[1] = gameconfig.Controls.Keyboard.LEFT;
	JoystickButtonConfig_[2] = gameconfig.Controls.Keyboard.DOWN;
	JoystickButtonConfig_[3] = gameconfig.Controls.Keyboard.RIGHT;
	JoystickButtonConfig_[4] = gameconfig.Controls.Keyboard.A;
	JoystickButtonConfig_[5] = gameconfig.Controls.Keyboard.B;
	JoystickButtonConfig_[6] = gameconfig.Controls.Keyboard.LB;
	JoystickButtonConfig_[7] = gameconfig.Controls.Keyboard.RB;
}

bool PlayerController::IsButtonJustPressed(ControllerButtons button) const {
	auto keyboardKey = KeyboardKeyConfig_[static_cast<int>(button)];
	auto keyboardPressed = IsKeyboardKeyJustPressed(keyboardKey);
	// First player uses keyboard and joystick
	if (PlayerNum_ == 0) {
		auto joystickPressed = false;
		if (JoystickAssigned_ != -1) {
			auto joystickButton = JoystickButtonConfig_[static_cast<int>(button)];
			joystickPressed = geGamepadButtonJustPressed(JoystickAssigned_, joystickButton);
		}
		return keyboardPressed || joystickPressed;
	}
	return keyboardPressed;
}

bool PlayerController::IsButtonPressed(ControllerButtons button) const {
	auto keyboardKey = KeyboardKeyConfig_[static_cast<int>(button)];
	auto keyboardPressed = IsKeyboardKeyDown(keyboardKey);
	// First player uses keyboard and joystick
	if (PlayerNum_ == 0) {
		auto joystickPressed = false;
		if (JoystickAssigned_ != -1) {
			auto joystickButton = JoystickButtonConfig_[static_cast<int>(button)];
			joystickPressed = geGamepadButtonHeldDown(JoystickAssigned_, joystickButton);
		}
		return keyboardPressed || joystickPressed;
	}
	return keyboardPressed;
}
float PlayerController::JoystickAxisState(JoystickAxis axis) const { return 0; }
float PlayerController::JoystickAxisThisFrameMovement(JoystickAxis axis) const { return 0; }

bool PlayerController::IsButtonJustReleased(ControllerButtons button) const {
	return false;
}

void PlayerController::AssignGamepadToController(int gamepadNum) {
	JoystickAssigned_ = gamepadNum;
}
