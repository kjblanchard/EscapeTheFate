#pragma once
#include <functional>

namespace Etf {

void AddTabFuncToMainDebugWindow(std::function<void()> drawFunc);
void DrawMainDebugWindow();
}  // namespace Etf
