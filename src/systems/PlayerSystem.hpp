#pragma once
#include <bindings/Player.hpp>
#include <memory>
#include <ui/uiObject.hpp>
namespace Etf {

class UIAnimation;

void StartPlayerSystem();
void UpdatePlayerSystem();
void ShutdownPlayerSystem();
const std::shared_ptr<Player>& GetPlayerByNum(int playerNum);

}  // namespace Etf
