#pragma once
#include <Supergoon/Primitives/Color.h>
#include <Supergoon/Primitives/rectangle.h>

#include <string>
#include <ui/uiObject.hpp>
class Text;
namespace Etf {
struct UITextArgs {
	std::string FontName;
	unsigned int FontSize;
	RectangleF Rect;
	std::string TextToDraw;
	int NumCharsToDraw;
	bool CenteredX, CenteredY;
	bool Visible;
};
class UIText : public UIObject {
   public:
	UIText(UITextArgs& args);
	void OnDraw(float offsetX, float offsetY) override final;

   private:
	Text* _text;
	static std::string _lastUsedFontName;
	static unsigned int _lastUsedFontSize;
};
}  // namespace Etf
