#pragma once
#include <functional>

namespace Etf {

void CreateMainWindow();
void AddTabFuncToMainWindow(std::function<void()> drawFunc);
}  // namespace Etf
