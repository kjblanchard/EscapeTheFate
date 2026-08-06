#pragma once

namespace Etf {
class UIButton;
namespace MouseInputSystem {
void Update();
void RegisterButton(UIButton* button);
void UnregisterButton(UIButton* button);
void ClearButtons();
}  // namespace MouseInputSystem
}  // namespace Etf
