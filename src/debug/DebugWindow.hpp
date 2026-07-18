#pragma once
#include <functional>

namespace Etf {

namespace DebugWindow {
void Initialize();
void AddTabFuncToMainDebugWindow(std::function<void()> drawFunc);
void AddWindowFunc(const std::string& windowName, std::function<void()> drawFunc);
void DrawWindows();

}  // namespace DebugWindow

}  // namespace Etf
