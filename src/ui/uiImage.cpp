#include <Supergoon/Graphics/shader.h>
#include <Supergoon/Graphics/texture.h>

#include <ui/uiImage.hpp>
using namespace Etf;
using namespace std;

UIImage::UIImage(UIImageArgs& args) {
	_location = args.Rect;
	_sourceRect = args.SourceRect;
	_scale = args.Scale;
	_filename = args.Filename;
	_color = args.DrawColor;
	_texture = TextureCreate(_filename.c_str());
	TextureLoadFromBmp(_texture, _filename.c_str());
}

UIImage::~UIImage() {
	TextureDestroy(_texture);
}

void UIImage::OnDraw(float offsetX, float offsetY) {
	DrawTexture(_texture, GetDefaultShader(), &_location, &_sourceRect, false, _scale, false, &_color);
}
