#include <engine.hpp>
#include <gameState.hpp>
#include <systems/MouseInputSystem.hpp>
#include <systems/PauseSystem.hpp>
#include <systems/PlayerControllerSystem.hpp>
#include <types/ControllerButtons.hpp>
#include <ui/ui.hpp>
#include <ui/uiButton.hpp>
#include <ui/uiImage.hpp>
#include <ui/uiLine.hpp>
#include <ui/uiNineSlice.hpp>
#include <ui/uiText.hpp>

using namespace Etf;

static UIObject* _panel = nullptr;
static UIImage* _p1ControllerIcon = nullptr;
static UIImage* _p1KeyboardIcon = nullptr;
static UIImage* _p2ControllerIcon = nullptr;
static UIImage* _p2KeyboardIcon = nullptr;

static const Color kActiveColor = {255, 255, 255, 255};
static const Color kInactiveColor = {255, 255, 255, 80};

static void syncIcons() {
	bool p0HasGamepad = PlayerControllerSystem::GetPlayerByNum(0)->HasGamepadAssigned();
	bool p1HasGamepad = PlayerControllerSystem::GetPlayerByNum(1)->HasGamepadAssigned();
	_p1ControllerIcon->UpdateDrawColor(p0HasGamepad ? kActiveColor : kInactiveColor);
	_p1KeyboardIcon->UpdateDrawColor(kActiveColor);
	_p2ControllerIcon->UpdateDrawColor(p1HasGamepad ? kActiveColor : kInactiveColor);
	_p2KeyboardIcon->UpdateDrawColor(p1HasGamepad ? kInactiveColor : kActiveColor);
}

static void swapControllers() {
	auto& p0 = PlayerControllerSystem::GetPlayerByNum(0);
	auto& p1 = PlayerControllerSystem::GetPlayerByNum(1);
	int pad0 = p0->GetAssignedGamepad();
	int pad1 = p1->GetAssignedGamepad();
	p0->AssignGamepadToController(pad1);
	p1->AssignGamepadToController(pad0);
}

void PauseSystem::Start() {
	UINineSliceArgs panelArgs;
	panelArgs.Name = "PausePanel";
	panelArgs.Filename = "uibase";
	panelArgs.Rect = {140.0f, 75.0f, 200.0f, 120.0f};
	panelArgs.SourceRect = {0, 0, 64, 64};
	panelArgs.DrawColor = {20, 0, 40, 230};
	panelArgs.Xoffset = 8;
	panelArgs.Yoffset = 8;
	panelArgs.Scale = 1.0f;
	panelArgs.Priority = 50;
	panelArgs.Visible = false;
	panelArgs.DebugBox = false;
	auto* panel = new UINineSlice(panelArgs);
	panel->SetDoNotDestroy(true);

	UITextArgs titleArgs;
	titleArgs.FontName = "PressStart2P";
	titleArgs.FontSize = 8;
	titleArgs.Rect = {0.0f, 10.0f, 200.0f, 16.0f};
	titleArgs.TextToDraw = "PAUSED";
	titleArgs.Name = "PausedTitle";
	titleArgs.NumCharsToDraw = 10;
	titleArgs.Priority = 0;
	titleArgs.TextColor = {255, 255, 255, 255};
	titleArgs.CenteredX = true;
	titleArgs.CenteredY = false;
	titleArgs.WordWrap = false;
	titleArgs.Visible = true;
	titleArgs.DebugBox = false;
	panel->AddChild(new UIText(titleArgs));

	// P1 section
	UITextArgs p1LabelArgs;
	p1LabelArgs.FontName = "PressStart2P";
	p1LabelArgs.FontSize = 8;
	p1LabelArgs.Rect = {25.0f, 30.0f, 50.0f, 12.0f};
	p1LabelArgs.TextToDraw = "P1";
	p1LabelArgs.Name = "PauseP1Label";
	p1LabelArgs.NumCharsToDraw = 10;
	p1LabelArgs.Priority = 0;
	p1LabelArgs.TextColor = {255, 0, 255, 255};
	p1LabelArgs.CenteredX = true;
	p1LabelArgs.CenteredY = false;
	p1LabelArgs.WordWrap = false;
	p1LabelArgs.Visible = true;
	p1LabelArgs.DebugBox = false;
	panel->AddChild(new UIText(p1LabelArgs));

	UIImageArgs p1CtrlArgs;
	p1CtrlArgs.Filename = "gamecontroller";
	p1CtrlArgs.Name = "PauseP1Controller";
	p1CtrlArgs.Rect = {20.0f, 45.0f, 16.0f, 16.0f};
	p1CtrlArgs.SourceRect = {0, 0, 16, 16};
	p1CtrlArgs.Scale = 1.0f;
	p1CtrlArgs.DrawColor = kActiveColor;
	p1CtrlArgs.Priority = 0;
	p1CtrlArgs.Visible = true;
	p1CtrlArgs.DebugBox = false;
	_p1ControllerIcon = new UIImage(p1CtrlArgs);
	panel->AddChild(_p1ControllerIcon);

	UIImageArgs p1KbArgs;
	p1KbArgs.Filename = "keyboard";
	p1KbArgs.Name = "PauseP1Keyboard";
	p1KbArgs.Rect = {42.0f, 45.0f, 16.0f, 16.0f};
	p1KbArgs.SourceRect = {0, 0, 16, 16};
	p1KbArgs.Scale = 1.0f;
	p1KbArgs.DrawColor = kActiveColor;
	p1KbArgs.Priority = 0;
	p1KbArgs.Visible = true;
	p1KbArgs.DebugBox = false;
	_p1KeyboardIcon = new UIImage(p1KbArgs);
	panel->AddChild(_p1KeyboardIcon);

	// Separator
	UIObjectArgs sepObjArgs;
	sepObjArgs.Rect = {100.0f, 30.0f, 0.0f, 0.0f};
	sepObjArgs.Visible = true;
	sepObjArgs.Priority = 0;
	sepObjArgs.Name = "PauseSeparator";
	UILineArgs sepLineArgs;
	sepLineArgs.X1 = 0;
	sepLineArgs.X2 = 0;
	sepLineArgs.Y1 = 0;
	sepLineArgs.Y2 = 40;
	sepLineArgs.Thickness = 1;
	sepLineArgs.LineColor = {255, 0, 255, 255};
	panel->AddChild(new UILine(sepLineArgs, sepObjArgs));

	// P2 section
	UITextArgs p2LabelArgs;
	p2LabelArgs.FontName = "PressStart2P";
	p2LabelArgs.FontSize = 8;
	p2LabelArgs.Rect = {120.0f, 30.0f, 50.0f, 12.0f};
	p2LabelArgs.TextToDraw = "P2";
	p2LabelArgs.Name = "PauseP2Label";
	p2LabelArgs.NumCharsToDraw = 10;
	p2LabelArgs.Priority = 0;
	p2LabelArgs.TextColor = {255, 0, 255, 255};
	p2LabelArgs.CenteredX = true;
	p2LabelArgs.CenteredY = false;
	p2LabelArgs.WordWrap = false;
	p2LabelArgs.Visible = true;
	p2LabelArgs.DebugBox = false;
	panel->AddChild(new UIText(p2LabelArgs));

	UIImageArgs p2CtrlArgs;
	p2CtrlArgs.Filename = "gamecontroller";
	p2CtrlArgs.Name = "PauseP2Controller";
	p2CtrlArgs.Rect = {120.0f, 45.0f, 16.0f, 16.0f};
	p2CtrlArgs.SourceRect = {0, 0, 16, 16};
	p2CtrlArgs.Scale = 1.0f;
	p2CtrlArgs.DrawColor = kInactiveColor;
	p2CtrlArgs.Priority = 0;
	p2CtrlArgs.Visible = true;
	p2CtrlArgs.DebugBox = false;
	_p2ControllerIcon = new UIImage(p2CtrlArgs);
	panel->AddChild(_p2ControllerIcon);

	UIImageArgs p2KbArgs;
	p2KbArgs.Filename = "keyboard";
	p2KbArgs.Name = "PauseP2Keyboard";
	p2KbArgs.Rect = {142.0f, 45.0f, 16.0f, 16.0f};
	p2KbArgs.SourceRect = {0, 0, 16, 16};
	p2KbArgs.Scale = 1.0f;
	p2KbArgs.DrawColor = kInactiveColor;
	p2KbArgs.Priority = 0;
	p2KbArgs.Visible = true;
	p2KbArgs.DebugBox = false;
	_p2KeyboardIcon = new UIImage(p2KbArgs);
	panel->AddChild(_p2KeyboardIcon);

	// Hint text
	UITextArgs hintArgs;
	hintArgs.FontName = "PressStart2P";
	hintArgs.FontSize = 8;
	hintArgs.Rect = {0.0f, 80.0f, 200.0f, 30.0f};
	hintArgs.TextToDraw = "A:Swap Start:Resume";
	hintArgs.Name = "PauseHint";
	hintArgs.NumCharsToDraw = 30;
	hintArgs.Priority = 0;
	hintArgs.TextColor = {200, 200, 200, 255};
	hintArgs.CenteredX = true;
	hintArgs.CenteredY = false;
	hintArgs.WordWrap = true;
	hintArgs.Visible = true;
	hintArgs.DebugBox = false;
	panel->AddChild(new UIText(hintArgs));

	auto resumeAction = []() {
		if (!GameState::Paused) return;
		GameState::Paused = false;
		_panel->SetVisible(false);
	};

	UIButtonArgs swapBtnArgs;
	swapBtnArgs.Rect = {10.0f, 28.0f, 180.0f, 40.0f};
	swapBtnArgs.Name = "PauseSwapBtn";
	swapBtnArgs.Priority = 5;
	swapBtnArgs.Visible = true;
	swapBtnArgs.Layer = 2;
	auto* swapBtn = new UIButton(swapBtnArgs);
	swapBtn->SetClickCallback([]() {
		if (!GameState::Paused) return;
		swapControllers();
		syncIcons();
	});
	swapBtn->SetRightClickCallback(resumeAction);
	panel->AddChild(swapBtn);
	MouseInputSystem::RegisterButton(swapBtn);

	UIButtonArgs resumeBtnArgs;
	resumeBtnArgs.Rect = {10.0f, 75.0f, 180.0f, 35.0f};
	resumeBtnArgs.Name = "PauseResumeBtn";
	resumeBtnArgs.Priority = 5;
	resumeBtnArgs.Visible = true;
	resumeBtnArgs.Layer = 2;
	auto* resumeBtn = new UIButton(resumeBtnArgs);
	resumeBtn->SetClickCallback(resumeAction);
	resumeBtn->SetRightClickCallback(resumeAction);
	panel->AddChild(resumeBtn);
	MouseInputSystem::RegisterButton(resumeBtn);

	UI::GetRootUIObject()->AddChild(panel);
	_panel = panel;
}

void PauseSystem::Update() {
	auto& p0 = PlayerControllerSystem::GetPlayerByNum(0);
	auto& p1 = PlayerControllerSystem::GetPlayerByNum(1);

	if (!GameState::Paused) {
		if (Engine::CurrentSceneName() == "cloud") return;
		if (GameState::CurrentFadeState != (int)LoadingScreenFadeTypes::NotFading) return;
		if (p0->IsButtonJustPressed(ControllerButtons::Start) ||
			p1->IsButtonJustPressed(ControllerButtons::Start)) {
			GameState::Paused = true;
			_panel->SetVisible(true);
			syncIcons();
		}
		return;
	}

	if (p0->IsButtonJustPressed(ControllerButtons::Start) ||
		p1->IsButtonJustPressed(ControllerButtons::Start)) {
		GameState::Paused = false;
		_panel->SetVisible(false);
		return;
	}

	if (p0->IsButtonJustPressed(ControllerButtons::A) ||
		p1->IsButtonJustPressed(ControllerButtons::A)) {
		swapControllers();
		syncIcons();
	}
}
