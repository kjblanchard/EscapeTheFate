#include <Supergoon/text.h>

#include <bindings/engine.hpp>
#include <ui/uiText.hpp>

using namespace Etf;
using namespace std;

UIText::UIText(UITextArgs& args) {
	_location = args.Rect;
	_text = Engine::TextBoi::CreateText(args.FontName, args.FontSize, args.Rect, args.TextToDraw, args.NumCharsToDraw, args.CenteredX, args.CenteredY);
}

UIText::~UIText() {
	TextDestroy(_text);
}

void UIText::OnDraw(float offsetX, float offsetY) {
	Engine::TextBoi::DrawText(_text, offsetY, offsetY);
}
