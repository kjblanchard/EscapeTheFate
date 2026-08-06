#pragma once

namespace Etf {
class UIButton;
namespace MouseInputSystem {
void Update();
void RegisterButton(UIButton* button);
void UnregisterButton(UIButton* button);
void ClearButtons();
void GetMouseGamePos(float& outX, float& outY);
}  // namespace MouseInputSystem
}  // namespace Etf
