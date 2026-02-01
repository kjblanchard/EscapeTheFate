#include <Supergoon/Graphics/shader.h>
#include <Supergoon/Graphics/texture.h>

#include <ui/uiImage.hpp>
using namespace Etf;
using namespace std;

UIImage::UIImage(UIImageArgs& args) {
	_location = args.Rect;
	_sourceRect = args.SourceRect;
	_scale = args.Scale;
	_name = args.Name;
	_filename = args.Filename; 
	_visible = args.Visible;
	_priority = args.Priority;
	_color = args.DrawColor;
	_debugBox = args.DebugBox;
	_texture = TextureCreate(_filename.c_str());
	TextureLoadFromPng(_texture, _filename.c_str());
}

UIImage::~UIImage() {
	TextureDestroy(_texture);
}

void UIImage::OnDraw(float offsetX, float offsetY) {
	RectangleF dst = {_location.x + offsetX, _location.y + offsetY, _location.w, _location.h};
	DrawTexture(_texture, GetDefaultShader(), &dst, &_sourceRect, false, _scale, false, &_color);
}
