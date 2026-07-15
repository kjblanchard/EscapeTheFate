#include <imgui.h>
#include <Supergoon/Graphics/graphics.h>
#include <debug/DebugGameWindow.hpp>
#include "sgtools/log.h"
#include <Supergoon/Graphics/texture.h>
using namespace Etf;
using namespace std;

extern  Texture* _imGUIScreenRenderTargetTexture;
extern int _logicalX;
extern int _logicalY;


void DebugGameWindow::Initialize() { }

void DebugGameWindow::Draw() {
#ifdef imgui
	// auto graphics = _imGUIScreenRenderTargetTexture;
	ImVec2 imguiWindowSize = ImGui::GetContentRegionAvail();
	// // Step 1: Calculate the integer scaling factor
	int scaleX = imguiWindowSize.x / _logicalX;	 // Integer division for X axis
	int scaleY = imguiWindowSize.y / _logicalY;	 // Integer division for Y axis
	// // Choose the smaller scale factor to maintain aspect ratio
	int scale = (scaleX < scaleY) ? scaleX : scaleY;
	// // Step 2: Calculate the scaled size (what SDL would render to the screen)
	int scaledWidth = _logicalX * scale;
	int scaledHeight = _logicalY * scale;
	sgLogInternal(sgLogLevelWarn, "Window size %f:%f, logicalsize %d:%d scale %d:%d scalewidth %d:%d", imguiWindowSize.x, imguiWindowSize.y, _logicalX, _logicalY, scale, scale, scaledWidth, scaledHeight);
	// // Step 3: Render the SDL_Texture in ImGui with the same scaling
	ImVec2 imguiSize(scaledWidth, scaledHeight);  // Use the scaled size
	auto tex = (ImTextureID)(intptr_t)TextureGetID(_imGUIScreenRenderTargetTexture);
	// ImGui::Image(tex, imguiSize);
	ImGui::Image( tex, imguiSize, ImVec2(0, 1), ImVec2(1, 0)
);

#else
	return;
#endif
}
