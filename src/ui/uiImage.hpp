#pragma once
#include <Supergoon/Primitives/Color.h>
#include <Supergoon/Primitives/rectangle.h>

#include <string>
#include <ui/uiObject.hpp>
struct Texture;
namespace Etf {
struct UIImageArgs {
	std::string Filename, Name;
	RectangleF Rect;
	RectangleF SourceRect;
	float Scale;
	Color DrawColor;
	bool Visible, DebugBox = false;
};
class UIImage : public UIObject {
   public:
	UIImage(UIImageArgs& args);
	~UIImage();
	void OnDraw(float offsetX, float offsetY) override final;

   private:
	Texture* _texture;
	RectangleF _sourceRect;
	float _scale;
	std::string _filename;
	Color _color;
};
}  // namespace Etf
