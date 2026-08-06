#include <Supergoon/Input/mouse.h>

#include <algorithm>
#include <systems/MouseInputSystem.hpp>
#include <ui/uiButton.hpp>
#include <vector>

#ifdef imgui
#include <debug/DebugGameWindow.hpp>
#include <imgui.h>
#endif

using namespace Etf;

static std::vector<UIButton*> sButtons;

static void getGameMousePosition(float& outX, float& outY) {
#ifdef imgui
	auto& vp = DebugGameWindow::GetViewportInfo();
	if (!vp.valid || vp.scale < 1) {
		outX = -1;
		outY = -1;
		return;
	}
	ImVec2 mp = ImGui::GetIO().MousePos;
	float relX = mp.x - vp.topLeftX;
	float relY = mp.y - vp.topLeftY;
	if (relX < 0 || relY < 0 || relX >= vp.width || relY >= vp.height) {
		outX = -1;
		outY = -1;
		return;
	}
	outX = relX / vp.scale;
	outY = relY / vp.scale;
#else
	GetGameMousePos(&outX, &outY);
#endif
}

void MouseInputSystem::Update() {
	float gameX, gameY;
	getGameMousePosition(gameX, gameY);
	if (gameX < 0 || gameY < 0) return;

	for (auto* btn : sButtons) {
		if (btn->HitTest(gameX, gameY)) {
			btn->Hover();
		}
	}

	if (!IsMouseButtonJustPressed(MouseButtonsLeftClick)) return;

	for (auto* btn : sButtons) {
		if (btn->HitTest(gameX, gameY)) {
			btn->Fire();
			break;
		}
	}
}

void MouseInputSystem::RegisterButton(UIButton* button) {
	sButtons.push_back(button);
}

void MouseInputSystem::UnregisterButton(UIButton* button) {
	sButtons.erase(std::remove(sButtons.begin(), sButtons.end(), button), sButtons.end());
}

void MouseInputSystem::ClearButtons() {
	sButtons.clear();
}
