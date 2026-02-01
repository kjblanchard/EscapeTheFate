#pragma once
#include <Supergoon/Primitives/rectangle.h>
namespace Etf {
class IInteractable {
   public:
	virtual ~IInteractable() = default;
	RectangleF InteractionRect = {0, 0, 0, 0};
	virtual void Interact() = 0;
};
}  // namespace Etf
