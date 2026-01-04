#pragma once
#include <Supergoon/Primitives/Color.h>
#include <Supergoon/Primitives/rectangle.h>

#include <string>
#include <ui/uiObject.hpp>
class Texture;
namespace Etf {
struct UIImageArgs {
	std::string Filename;
	RectangleF Rect;
	RectangleF SourceRect;
	float Scale;
	Color DrawColor;
	bool Visible;
};
class UIImage : public UIObject {
   public:
	UIImage(UIImageArgs& args);
	void OnDraw(float offsetX, float offsetY) override final;

   private:
	Texture* _texture;
	RectangleF _sourceRect;
	float _scale;
	std::string _filename;
	Color _color;
};
}  // namespace Etf
