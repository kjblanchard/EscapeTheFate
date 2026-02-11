#pragma once

namespace Etf {

enum class GameButtons {
	UP,
	LEFT,
	DOWN,
	RIGHT,
	A,
	B,
	// LB,
	// RB,
	// LT,
	// RT,
	// START,
	// SELECT,
	// GUIDE,
	NUM_GAME_BUTTONS,
};

struct ControllerConfig {
	struct DefaultKeyboardbuttons {
		static const int UP = 26;
		static const int LEFT = 4;
		static const int DOWN = 22;
		static const int RIGHT = 7;
		static const int A = 44;
		static const int B = 27;
	};

	struct DefaultJoystickButtons {
		static const int UP = 11;
		static const int LEFT = 13;
		static const int DOWN = 12;
		static const int RIGHT = 14;
		static const int A = 0;
		static const int B = 1;
		static const int X = 2;
		static const int Y = 3;
		static const int LB = 9;
	};

	int KeyboardKeyConfig[static_cast<int>(GameButtons::NUM_GAME_BUTTONS)];
	int JoystickButtonConfig[static_cast<int>(GameButtons::NUM_GAME_BUTTONS)];
};

class Controller {
   public:
	static void InitializeControllers();
	static bool IsButtonPressed(GameButtons button, int playerNum = 0);
	static bool IsButtonJustPressed(GameButtons button, int playerNum = 0);
	static bool IsButtonJustReleased(GameButtons button, int playerNum = 0);
	static void AssignGamepadToController(int gamepadNum);

   private:
	Controller();
	ControllerConfig _controllerConfig;
	int _joystickAssigned = 0;
};
}  // namespace Etf
   // typedef enum SDL_GamepadButton
// {
//     SDL_GAMEPAD_BUTTON_INVALID = -1,
//     SDL_GAMEPAD_BUTTON_SOUTH,           /**< Bottom face button (e.g. Xbox A button) */
//     SDL_GAMEPAD_BUTTON_EAST,            /**< Right face button (e.g. Xbox B button) */
//     SDL_GAMEPAD_BUTTON_WEST,            /**< Left face button (e.g. Xbox X button) */
//     SDL_GAMEPAD_BUTTON_NORTH,           /**< Top face button (e.g. Xbox Y button) */
//     SDL_GAMEPAD_BUTTON_BACK,
//  5   SDL_GAMEPAD_BUTTON_GUIDE,
//     SDL_GAMEPAD_BUTTON_START,
//     SDL_GAMEPAD_BUTTON_LEFT_STICK,
//     SDL_GAMEPAD_BUTTON_RIGHT_STICK,
//     SDL_GAMEPAD_BUTTON_LEFT_SHOULDER,
//  10   SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER,
//     SDL_GAMEPAD_BUTTON_DPAD_UP,
//     SDL_GAMEPAD_BUTTON_DPAD_DOWN,
//     SDL_GAMEPAD_BUTTON_DPAD_LEFT,
//     SDL_GAMEPAD_BUTTON_DPAD_RIGHT,
//     SDL_GAMEPAD_BUTTON_MISC1,           /**< Additional button (e.g. Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button, Google Stadia capture button) */
//     SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1,   /**< Upper or primary paddle, under your right hand (e.g. Xbox Elite paddle P1, DualSense Edge RB button, Right Joy-Con SR button) */
//     SDL_GAMEPAD_BUTTON_LEFT_PADDLE1,    /**< Upper or primary paddle, under your left hand (e.g. Xbox Elite paddle P3, DualSense Edge LB button, Left Joy-Con SL button) */
//     SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2,   /**< Lower or secondary paddle, under your right hand (e.g. Xbox Elite paddle P2, DualSense Edge right Fn button, Right Joy-Con SL button) */
//     SDL_GAMEPAD_BUTTON_LEFT_PADDLE2,    /**< Lower or secondary paddle, under your left hand (e.g. Xbox Elite paddle P4, DualSense Edge left Fn button, Left Joy-Con SR button) */
//     SDL_GAMEPAD_BUTTON_TOUCHPAD,        /**< PS4/PS5 touchpad button */
//     SDL_GAMEPAD_BUTTON_MISC2,           /**< Additional button */
//     SDL_GAMEPAD_BUTTON_MISC3,           /**< Additional button (e.g. Nintendo GameCube left trigger click) */
//     SDL_GAMEPAD_BUTTON_MISC4,           /**< Additional button (e.g. Nintendo GameCube right trigger click) */
//     SDL_GAMEPAD_BUTTON_MISC5,           /**< Additional button */
//     SDL_GAMEPAD_BUTTON_MISC6,           /**< Additional button */
//     SDL_GAMEPAD_BUTTON_COUNT
// } SDL_GamepadButton;
