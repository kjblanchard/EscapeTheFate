#include <Supergoon/Graphics/texture.h>
#include <Supergoon/Platform/opengl/openglTexture.h>
#include <imgui.h>
#include <string>
#include <functional>

#include <debug/DebugWindow.hpp>
#include <vector>

using namespace Etf;
using namespace std;

struct debugWindow {
	string WindowName;
	function<void()> DrawFunction;
};

static vector<function<void()>> mainWindowTabDrawFunctions_;
static vector<debugWindow> windowsToDraw_;

void DebugWindow::AddTabFuncToMainDebugWindow(std::function<void()> drawFunc) {
	mainWindowTabDrawFunctions_.push_back(drawFunc);
}

static void drawWindowInternal(debugWindow& windowToDraw) {
#ifdef imgui
	static bool p_open = true;
	static bool no_titlebar = false;
	static bool no_scrollbar = false;
	static bool no_menu = false;
	static bool no_move = false;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = false;
	static bool unsaved_document = false;
	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar) window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu) window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move) window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize) window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse) window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav) window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background) window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front) window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (unsaved_document) window_flags |= ImGuiWindowFlags_UnsavedDocument;
	if (!ImGui::Begin(windowToDraw.WindowName.c_str(), &p_open, window_flags)) {
		ImGui::End();
		return;
	}
	windowToDraw.DrawFunction();
	ImGui::End();
#else
	return;
#endif
}

static void drawMainWindow() {
#ifdef imgui
	auto textures = GetCachedTextures();
	auto numTextures = GetNumCachedTextures();
	if (ImGui::CollapsingHeader("Textures")) {
		if (ImGui::TreeNode("Cached Textures")) {
			for (int i = 0; i < numTextures; ++i) {
				auto texture = textures[i];
				if(!texture) continue;
				if (ImGui::TreeNode(texture->Name)) {
					if (ImGui::TreeNode(texture->Name)) {
						ImGui::Text("RefCount:");
						ImGui::SameLine();
						ImGui::Text("%d", texture->RefCount);
						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}
	for (auto& func : mainWindowTabDrawFunctions_) {
		func();
	}
#else
	return;
#endif
}

void DebugWindow::Initialize() {
	AddWindowFunc("Debugging Tools", drawMainWindow);
}

void DebugWindow::AddWindowFunc(const std::string& windowName, std::function<void()> drawFunc) {
	windowsToDraw_.push_back({windowName, drawFunc});
}

void DebugWindow::DrawWindows() {
	for (auto& window : windowsToDraw_) {
		drawWindowInternal(window);
	}
}
