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

	JoystickButtonConfig_[0] = gameconfig.Controls.Joystick.UP;
	JoystickButtonConfig_[1] = gameconfig.Controls.Joystick.LEFT;
	JoystickButtonConfig_[2] = gameconfig.Controls.Joystick.DOWN;
	JoystickButtonConfig_[3] = gameconfig.Controls.Joystick.RIGHT;
	JoystickButtonConfig_[4] = gameconfig.Controls.Joystick.A;
	JoystickButtonConfig_[5] = gameconfig.Controls.Joystick.B;
	JoystickButtonConfig_[6] = gameconfig.Controls.Joystick.LB;
	JoystickButtonConfig_[7] = gameconfig.Controls.Joystick.RB;

	KeyboardKeyConfig_[static_cast<int>(ControllerButtons::Start)] = gameconfig.Controls.Keyboard.START;
	JoystickButtonConfig_[static_cast<int>(ControllerButtons::Start)] = gameconfig.Controls.Joystick.START;
}

bool PlayerController::IsButtonJustPressed(ControllerButtons button) const {
	auto idx = static_cast<int>(button);
	auto stickPressed = isStickDirectionJustPressed(button);
	if (PlayerNum_ == 0) {
		auto keyboardPressed = IsKeyboardKeyJustPressed(KeyboardKeyConfig_[idx]);
		auto joystickPressed = false;
		if (JoystickAssigned_ != -1) {
			joystickPressed = geGamepadButtonJustPressed(JoystickAssigned_, JoystickButtonConfig_[idx]);
		}
		return keyboardPressed || joystickPressed || stickPressed;
	}
	if (JoystickAssigned_ != -1) {
		return geGamepadButtonJustPressed(JoystickAssigned_, JoystickButtonConfig_[idx]) || stickPressed;
	}
	return stickPressed;
}

bool PlayerController::IsButtonPressed(ControllerButtons button) const {
	auto idx = static_cast<int>(button);
	if (PlayerNum_ == 0) {
		auto keyboardPressed = IsKeyboardKeyDown(KeyboardKeyConfig_[idx]);
		auto joystickPressed = false;
		if (JoystickAssigned_ != -1) {
			joystickPressed = geGamepadButtonHeldDown(JoystickAssigned_, JoystickButtonConfig_[idx]);
		}
		return keyboardPressed || joystickPressed;
	}
	if (JoystickAssigned_ != -1) {
		return geGamepadButtonHeldDown(JoystickAssigned_, JoystickButtonConfig_[idx]);
	}
	return false;
}
float PlayerController::JoystickAxisState(JoystickAxis axis) const {
	if (JoystickAssigned_ == -1) return 0.0f;
	switch (axis) {
		case JoystickAxis::LeftThumbstickX:
			return geGamepadLeftAxisXFloat(JoystickAssigned_);
		case JoystickAxis::LeftThumbstickY:
			return geGamepadLeftAxisYFloat(JoystickAssigned_);
		default:
			return 0.0f;
	}
}

float PlayerController::JoystickAxisThisFrameMovement(JoystickAxis axis) const {
	if (JoystickAssigned_ == -1) return 0.0f;
	switch (axis) {
		case JoystickAxis::LeftThumbstickX:
			return static_cast<float>(geGamepadLeftAxisXThisFrameMovement(JoystickAssigned_));
		case JoystickAxis::LeftThumbstickY:
			return static_cast<float>(geGamepadLeftAxisYThisFrameMovement(JoystickAssigned_));
		default:
			return 0.0f;
	}
}

bool PlayerController::IsButtonJustReleased(ControllerButtons button) const {
	auto idx = static_cast<int>(button);
	if (PlayerNum_ == 0) {
		auto keyboardReleased = IsKeyboardKeyJustReleased(KeyboardKeyConfig_[idx]);
		auto joystickReleased = false;
		if (JoystickAssigned_ != -1) {
			joystickReleased = geGamepadButtonJustReleased(JoystickAssigned_, JoystickButtonConfig_[idx]);
		}
		return keyboardReleased || joystickReleased;
	}
	if (JoystickAssigned_ != -1) {
		return geGamepadButtonJustReleased(JoystickAssigned_, JoystickButtonConfig_[idx]);
	}
	return false;
}

void PlayerController::Update() {
	StickWasUp_ = StickIsUp_;
	StickWasDown_ = StickIsDown_;
	StickWasLeft_ = StickIsLeft_;
	StickWasRight_ = StickIsRight_;
	if (JoystickAssigned_ == -1) {
		StickIsUp_ = StickIsDown_ = StickIsLeft_ = StickIsRight_ = false;
		return;
	}
	constexpr float kThreshold = 0.5f;
	auto x = geGamepadLeftAxisXFloat(JoystickAssigned_);
	auto y = geGamepadLeftAxisYFloat(JoystickAssigned_);
	StickIsLeft_ = x < -kThreshold;
	StickIsRight_ = x > kThreshold;
	StickIsUp_ = y < -kThreshold;
	StickIsDown_ = y > kThreshold;
}

bool PlayerController::isStickDirectionJustPressed(ControllerButtons button) const {
	switch (button) {
		case ControllerButtons::Up:
			return StickIsUp_ && !StickWasUp_;
		case ControllerButtons::Down:
			return StickIsDown_ && !StickWasDown_;
		case ControllerButtons::Left:
			return StickIsLeft_ && !StickWasLeft_;
		case ControllerButtons::Right:
			return StickIsRight_ && !StickWasRight_;
		default:
			return false;
	}
}

void PlayerController::AssignGamepadToController(int gamepadNum) {
	JoystickAssigned_ = gamepadNum;
}

void PlayerController::ResetPlayerNumCounter() {
	sCurrentPlayerNum_ = 0;
}
