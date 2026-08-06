#include <Supergoon/Graphics/graphics.h>
#include <Supergoon/Graphics/texture.h>
#include <Supergoon/Platform/opengl/openglTexture.h>
#include <imgui.h>

#include <debug/DebugGameWindow.hpp>

#include "sgtools/log.h"
using namespace Etf;
using namespace std;

extern Texture* _imGUIScreenRenderTargetTexture;
extern int _logicalX;
extern int _logicalY;

static DebugGameWindow::ViewportInfo sViewport;

void DebugGameWindow::Initialize() {}

const DebugGameWindow::ViewportInfo& DebugGameWindow::GetViewportInfo() {
	return sViewport;
}

void DebugGameWindow::Draw() {
#ifdef imgui
	ImVec2 imguiWindowSize = ImGui::GetContentRegionAvail();
	int scaleX = imguiWindowSize.x / _logicalX;
	int scaleY = imguiWindowSize.y / _logicalY;
	int scale = (scaleX < scaleY) ? scaleX : scaleY;
	if (scale < 1) scale = 1;
	int scaledWidth = _logicalX * scale;
	int scaledHeight = _logicalY * scale;
	auto tex = (void*)TextureGetID(_imGUIScreenRenderTargetTexture);
	if(!tex) return;
	ImVec2 cursorPos = ImGui::GetCursorScreenPos();
	sViewport.topLeftX = cursorPos.x;
	sViewport.topLeftY = cursorPos.y;
	sViewport.scale = static_cast<float>(scale);
	sViewport.width = static_cast<float>(scaledWidth);
	sViewport.height = static_cast<float>(scaledHeight);
	sViewport.valid = true;
	ImGui::Image(tex, ImVec2(scaledWidth, scaledHeight), ImVec2(0, 1), ImVec2(1, 0));

#else
	return;
#endif
}
