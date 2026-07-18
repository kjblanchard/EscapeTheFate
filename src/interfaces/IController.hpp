#pragma once
#include <types/ControllerButtons.hpp>
namespace Etf {
class IController {
   public:
	virtual ~IController() = default;
	virtual bool IsButtonPressed(ControllerButtons button) const = 0;
	virtual bool IsButtonJustPressed(ControllerButtons button) const = 0;
	virtual bool IsButtonJustReleased(ControllerButtons button) const = 0;
	inline virtual float JoystickAxisState(JoystickAxis axis) const { return 0; };
	inline virtual float JoystickAxisThisFrameMovement(JoystickAxis axis) const { return 0; };
};
}  // namespace Etf
