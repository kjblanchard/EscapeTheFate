#pragma once
#include <string>

namespace Etf {
namespace BattleTransitionSystem {

void TriggerTransition(const std::string& battleScene);
void Start();
void Update();
void Shutdown();
bool IsTransitioning();

}  // namespace BattleTransitionSystem
}  // namespace Etf
