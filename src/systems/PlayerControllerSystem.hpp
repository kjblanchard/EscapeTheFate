#pragma once
#include <components/PlayerController.hpp>
#include <memory>
#include <ui/uiObject.hpp>
namespace Etf {

class UIAnimation;
namespace PlayerSystem {

void StartPlayerSystem();
void UpdatePlayerSystem();
void ShutdownPlayerSystem();
const std::shared_ptr<PlayerController>& GetPlayerByNum(int playerNum);
}  // namespace PlayerSystem

}  // namespace Etf
