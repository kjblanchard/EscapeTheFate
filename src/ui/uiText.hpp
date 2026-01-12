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
	bool CenteredX, CenteredY, WordWrap, Visible, DebugBox = false;
};
class UIText : public UIObject {
   public:
	UIText(UITextArgs& args);
	~UIText();
	void OnDraw(float offsetX, float offsetY) override final;
	void UpdateText(const std::string& newText);
	void UpdateTextNumLetters(unsigned int numLetters);

   private:
	Text* _text;
	static std::string _lastUsedFontName;
	static unsigned int _lastUsedFontSize;
};
}  // namespace Etf
