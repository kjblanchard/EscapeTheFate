#pragma once
namespace Etf {
namespace DebugGameWindow {
void Initialize();
void Draw();

struct ViewportInfo {
	float topLeftX = 0;
	float topLeftY = 0;
	float scale = 1;
	float width = 0;
	float height = 0;
	bool valid = false;
};
const ViewportInfo& GetViewportInfo();

}  // namespace DebugGameWindow
}  // namespace Etf
