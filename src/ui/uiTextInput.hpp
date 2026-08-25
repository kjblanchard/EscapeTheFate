#pragma once
#include <Supergoon/Primitives/Color.h>
#include <Supergoon/Primitives/rectangle.h>

#include <functional>
#include <string>
#include <ui/uiObject.hpp>

namespace Etf {

class UIText;

struct UITextInputArgs {
	std::string FontName;
	unsigned int FontSize;
	RectangleF Rect;
	std::string Placeholder;
	std::string Name;
	int Priority = 0;
	unsigned int MaxLength = 45;
	bool Visible = true;
	Color TextColor = {255, 255, 255, 255};
};

class UITextInput : public UIObject {
   public:
	UITextInput(const UITextInputArgs& args);
	~UITextInput() = default;

	void Update();
	void Focus();
	void Unfocus();
	bool IsFocused() const { return _focused; }

	const std::string& GetText() const { return _inputText; }
	void SetText(const std::string& text);
	void Clear();

	std::function<void(const std::string&)> OnSubmit;
	std::function<void()> OnCancel;

   private:
	void onTextInput(const char* text);
	void handleBackspace();
	void rebuildDisplayText();

	std::string _inputText;
	std::string _placeholder;
	std::string _fontName;
	unsigned int _fontSize;
	unsigned int _maxLength;
	Color _textColor;

	bool _focused = false;
	float _cursorBlinkTimer = 0.f;
	bool _cursorVisible = true;

	UIText* _displayText = nullptr;
};

}  // namespace Etf
