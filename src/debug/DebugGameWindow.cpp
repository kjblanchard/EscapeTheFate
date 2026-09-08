#include <Supergoon/Graphics/graphics.h>
#include <Supergoon/Graphics/texture.h>
#include <Supergoon/Platform/opengl/openglTexture.h>
#include <imgui.h>

#include <debug/DebugGameWindow.hpp>

using namespace Etf;
using namespace std;

extern "C" {
extern Texture* _imGUIScreenRenderTargetTexture;
extern int _logicalX;
extern int _logicalY;
}

// float _debugGameImageX = 0.0f;
// float _debugGameImageY = 0.0f;
// float _debugGameImageWidth = 0.0f;
// float _debugGameImageHeight = 0.0f;



void DebugGameWindow::Initialize() {}

// void DebugGameWindow::Draw() {
// #ifdef imgui
// 	ImVec2 imguiWindowSize = ImGui::GetContentRegionAvail();

// 	int scaleX = imguiWindowSize.x / _logicalX;
// 	int scaleY = imguiWindowSize.y / _logicalY;
// 	int scale = (scaleX < scaleY) ? scaleX : scaleY;
// 	if (scale < 1) scale = 1;

// 	int scaledWidth = _logicalX * scale;
// 	int scaledHeight = _logicalY * scale;

// 	auto tex = (void*)TextureGetID(_imGUIScreenRenderTargetTexture);
// 	if (!tex) return;

// 	ImVec2 imagePos = ImGui::GetCursorScreenPos();

// 	_debugGameImageX = imagePos.x;
// 	_debugGameImageY = imagePos.y;
// 	_debugGameImageWidth = (float)scaledWidth;
// 	_debugGameImageHeight = (float)scaledHeight;

// 	ImGui::Image(
// 		tex,
// 		ImVec2((float)scaledWidth, (float)scaledHeight),
// 		ImVec2(0, 1),
// 		ImVec2(1, 0)
// 	);
// #else
// 	return;
// #endif
// }


// Shared state
float _debugGameImageX = 0.0f;
float _debugGameImageY = 0.0f;
float _debugGameImageScale = 1.0f;

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
	if (!tex) return;

	ImVec2 imagePos = ImGui::GetCursorScreenPos();

	_debugGameImageX = imagePos.x;
	_debugGameImageY = imagePos.y;
	_debugGameImageScale = (float)scale;

	ImGui::Image(
		tex,
		ImVec2((float)scaledWidth, (float)scaledHeight),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);
#else
	return;
#endif
}

// void DebugGameWindow::Draw() {
// #ifdef imgui
// 	ImVec2 imguiWindowPos = ImGui::GetCursorScreenPos();
// 	ImVec2 imguiWindowSize = ImGui::GetContentRegionAvail();
// 	int scaleX = imguiWindowSize.x / _logicalX;
// 	int scaleY = imguiWindowSize.y / _logicalY;
// 	int scale = (scaleX < scaleY) ? scaleX : scaleY;
// 	if (scale < 1) scale = 1;
// 	int scaledWidth = _logicalX * scale;
// 	int scaledHeight = _logicalY * scale;
// 	auto tex = (void*)TextureGetID(_imGUIScreenRenderTargetTexture);
// 	if (!tex) return;
// 	_gameMouseOriginX = imguiWindowPos.x;
// 	_gameMouseOriginY = imguiWindowPos.y;
// 	_gameMouseScale = (float)scale;
// 	ImGui::Image(tex, ImVec2(scaledWidth, scaledHeight), ImVec2(0, 1), ImVec2(1, 0));

// #else
// 	return;
// #endif
// }
