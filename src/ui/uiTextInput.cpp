#include <Supergoon/Input/keyboard.h>
#include <Supergoon/state.h>
#include <SDL3/SDL_scancode.h>

#include <engine.hpp>
#include <ui/uiText.hpp>
#include <ui/uiTextInput.hpp>

#ifdef steamed
#include <Supergoon/services.h>
#endif

using namespace Etf;
using namespace std;

static const float kCursorBlinkInterval = 0.5f;

UITextInput::UITextInput(const UITextInputArgs& args)
	: _placeholder(args.Placeholder),
	  _fontName(args.FontName),
	  _fontSize(args.FontSize),
	  _maxLength(args.MaxLength),
	  _textColor(args.TextColor) {
	_location = args.Rect;
	_name = args.Name;
	_priority = args.Priority;
	_visible = args.Visible;

	UITextArgs textArgs;
	textArgs.FontName = args.FontName;
	textArgs.FontSize = args.FontSize;
	textArgs.Rect = {0, 0, args.Rect.w, args.Rect.h};
	textArgs.TextToDraw = args.Placeholder.empty() ? "|" : args.Placeholder;
	textArgs.Name = args.Name + "_text";
	textArgs.NumCharsToDraw = 100;
	textArgs.Priority = 0;
	textArgs.TextColor = args.TextColor;
	textArgs.CenteredX = false;
	textArgs.CenteredY = true;
	textArgs.WordWrap = false;
	textArgs.Visible = true;
	_displayText = new UIText(textArgs);
	AddChild(_displayText);
}

void UITextInput::Focus() {
	_focused = true;
	_cursorBlinkTimer = 0.f;
	_cursorVisible = true;
	Engine::TextInput::Start();
	Engine::TextInput::SetCallback([this](const char* t) { onTextInput(t); });
	rebuildDisplayText();
#ifdef steamed
	auto pos = AbsolutePosition();
	ShowSteamKeyboard((int)pos.x, (int)pos.y, (int)pos.w, (int)pos.h);
#endif
}

void UITextInput::Unfocus() {
	_focused = false;
	_cursorVisible = false;
	Engine::TextInput::ClearCallback();
	Engine::TextInput::Stop();
	rebuildDisplayText();
}

void UITextInput::Update() {
	if (!_focused) return;

	_cursorBlinkTimer += DeltaTimeSeconds;
	if (_cursorBlinkTimer >= kCursorBlinkInterval) {
		_cursorBlinkTimer -= kCursorBlinkInterval;
		_cursorVisible = !_cursorVisible;
		rebuildDisplayText();
	}

	if (IsKeyboardKeyJustPressed(SDL_SCANCODE_BACKSPACE)) {
		handleBackspace();
	}
	if (IsKeyboardKeyJustPressed(SDL_SCANCODE_RETURN)) {
		if (OnSubmit) OnSubmit(_inputText);
	}
	if (IsKeyboardKeyJustPressed(SDL_SCANCODE_ESCAPE)) {
		if (OnCancel) OnCancel();
	}
}

void UITextInput::SetText(const std::string& text) {
	_inputText = text;
	if (_inputText.size() > _maxLength) {
		_inputText.resize(_maxLength);
	}
	rebuildDisplayText();
}

void UITextInput::Clear() {
	_inputText.clear();
	rebuildDisplayText();
}

void UITextInput::onTextInput(const char* text) {
	if (!text) return;
	string newText(text);
	for (char c : newText) {
		if (_inputText.size() >= _maxLength) break;
		_inputText += c;
	}
	_cursorVisible = true;
	_cursorBlinkTimer = 0.f;
	rebuildDisplayText();
}

void UITextInput::handleBackspace() {
	if (_inputText.empty()) return;
	_inputText.pop_back();
	_cursorVisible = true;
	_cursorBlinkTimer = 0.f;
	rebuildDisplayText();
}

void UITextInput::rebuildDisplayText() {
	if (!_displayText) return;
	string display;
	if (_focused) {
		display = _inputText + (_cursorVisible ? "|" : " ");
	} else if (_inputText.empty()) {
		display = _placeholder.empty() ? " " : _placeholder;
	} else {
		display = _inputText;
	}
	_displayText->UpdateText(display);
}
