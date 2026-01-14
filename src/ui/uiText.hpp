#pragma once
#include <Supergoon/Primitives/Color.h>
#include <Supergoon/Primitives/rectangle.h>

#include <string>
#include <ui/uiObject.hpp>
struct Text;
namespace Etf {
struct UITextArgs {
	std::string FontName;
	unsigned int FontSize;
	RectangleF Rect;
	std::string TextToDraw, Name;
	int NumCharsToDraw;
	int Priority;
	bool CenteredX, CenteredY, WordWrap, Visible, DebugBox = false;
};
class UIText : public UIObject {
   public:
	UIText(UITextArgs& args);
	~UIText();
	void OnDraw(float offsetX, float offsetY) override final;
	//If you leave numlettersTOdraw as default, it will draw the full thing
	void UpdateText(const std::string& newText, int numLettersToDraw = -1);
	void UpdateTextNumLetters(unsigned int numLetters);

   private:
	Text* _text;
	static std::string _lastUsedFontName;
	static unsigned int _lastUsedFontSize;
};
}  // namespace Etf
