#pragma once
#include <Supergoon/Primitives/Color.h>
#include <Supergoon/Primitives/rectangle.h>

#include <string>
#include <ui/uiObject.hpp>
class Texture;
namespace Etf {
struct UINineSliceArgs {
	std::string Filename;
	RectangleF Rect;
	RectangleF SourceRect;
	// Offset to use when making the nine slice, else it will be considered the center.
float Xoffset, Yoffset;
	float Scale;
	Color DrawColor;
	bool Visible, DebugBox = false;
};
class UINineSlice : public UIObject {
   public:
	UINineSlice(UINineSliceArgs& args);
	~UINineSlice();
	void OnDraw(float offsetX, float offsetY) override final;

   private:
	Texture* _texture;
	Texture* _renderTargetTexture;
	RectangleF _sourceRect;
	float _scale;
	std::string _filename;
	Color _color;
};
}  // namespace Etf
