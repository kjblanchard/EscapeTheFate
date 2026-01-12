#include <Supergoon/text.h>

#include <cstring>
#include <bindings/engine.hpp>
#include <ui/uiText.hpp>

using namespace Etf;
using namespace std;

UIText::UIText(UITextArgs& args) {
	_location = args.Rect;
	_name = args.Name;
	_text = Engine::TextBoi::CreateText(args.FontName, args.FontSize, args.Rect, args.TextToDraw, args.NumCharsToDraw, args.CenteredX, args.CenteredY);
}

UIText::~UIText() {
	TextDestroy(_text);
}

void UIText::OnDraw(float offsetX, float offsetY) {
	Engine::TextBoi::DrawText(_text, offsetY, offsetY);
}

void UIText::UpdateText(const string& newText) {
	if (!_text || newText == string(_text->Text)) return;
	if (_text->Texture) TextureDestroy(_text->Texture);
	_text->Texture = NULL;
	_text->Text = strdup(newText.c_str());
	_text->NumLettersToDraw = newText.size();
	TextOnDirty(_text);
}

void UIText::UpdateTextNumLetters(unsigned int numLetters) {
	if (!_text) return;
	_text->NumLettersToDraw = numLetters;
	TextOnDirty(_text);
}
