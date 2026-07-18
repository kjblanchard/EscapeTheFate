#pragma once
#include <components/PlayerController.hpp>
#include <memory>
#include <ui/uiObject.hpp>
namespace Etf {

class UIAnimation;
namespace PlayerControllerSystem {

void Start();
void Update();
void Shutdown();
const std::shared_ptr<PlayerController>& GetPlayerByNum(int playerNum);
}  // namespace PlayerSystem

}  // namespace Etf
