#include <Supergoon/Graphics/shader.h>
#include <Supergoon/Graphics/texture.h>
#include <Supergoon/Primitives/Color.h>
#include <Supergoon/Primitives/rectangle.h>

#include <bindings/engine.hpp>
#include <ui/uiNineSlice.hpp>
using namespace Etf;
using namespace std;

UINineSlice::UINineSlice(UINineSliceArgs& args) {
	_location = args.Rect;
	_sourceRect = args.SourceRect;
	_scale = args.Scale;
	_filename = args.Filename;
	_color = args.DrawColor;
	_texture = TextureCreate(_filename.c_str());
	TextureLoadFromBmp(_texture, _filename.c_str());
	_renderTargetTexture = TextureCreateRenderTarget(_location.w, _location.h);
	int nineSliceImageW = TextureGetWidth(_texture);
	int nineSliceImageH = TextureGetHeight(_texture);
	// This counts as the middle
	TextureClearRenderTarget(_renderTargetTexture, _color.R / (float)255, _color.G / (float)255, _color.B / (float)255, _color.A / (float)255);
	float sizeX = args.Xoffset;
	float sizeY = args.Yoffset;
	// // / Draw the corners
	// // tl
	RectangleF srcRect = {0, 0, sizeX, sizeY};
	RectangleF dstRect = {0, 0, sizeX, sizeY};
	DrawTextureToTexture(_renderTargetTexture, _texture, GetDefaultShader(), &dstRect, &srcRect, 1.0);
	// tr
	srcRect = {nineSliceImageW - sizeX, 0, sizeX, sizeY};
	dstRect = {_location.w - sizeX, 0, sizeX, sizeY};
	// dstRect = {sizeX * 2, 0, sizeX, sizeY};
	DrawTextureToTexture(_renderTargetTexture, _texture, GetDefaultShader(), &dstRect, &srcRect, 1.0);
	// // // bl
	srcRect = {0, nineSliceImageH - sizeY, sizeX, sizeY};
	dstRect = {0, _location.h - sizeY, sizeX, sizeY};
	DrawTextureToTexture(_renderTargetTexture, _texture, GetDefaultShader(), &dstRect, &srcRect, 1.0);
	// // br
	srcRect = {nineSliceImageW - sizeX, nineSliceImageH - sizeY, sizeX, sizeY};
	dstRect = {_location.w - sizeX, _location.h - sizeY, sizeX, sizeY};
	DrawTextureToTexture(_renderTargetTexture, _texture, GetDefaultShader(), &dstRect, &srcRect, 1.0);
	// // draw the bars
	int length = _location.w - (sizeX);
	int height = _location.h - (sizeY);
	// // top
	srcRect = {1 + sizeX, 0, 1, sizeY};
	for (size_t i = sizeX; i < length; i++) {
		dstRect = {(float)i, 0, 1, sizeY};
		DrawTextureToTexture(_renderTargetTexture, _texture, GetDefaultShader(), &dstRect, &srcRect, 1.0);
	}
	// // bottom
	for (size_t i = sizeX; i < length; i++) {
		dstRect = {(float)i, _location.h - sizeY + 4, 1, sizeY};
		DrawTextureToTexture(_renderTargetTexture, _texture, GetDefaultShader(), &dstRect, &srcRect, 1.0);
	}
	// // left
	srcRect = {0, sizeY + 1, sizeX, 1};
	for (size_t i = sizeY; i < height; i++) {
		dstRect = {0, (float)i, sizeX, 1};
		DrawTextureToTexture(_renderTargetTexture, _texture, GetDefaultShader(), &dstRect, &srcRect, 1.0);
	}
	// // right
	for (size_t i = sizeY; i < height; i++) {
		dstRect = {_location.w - sizeX + 3, (float)i, sizeX, 1};
		DrawTextureToTexture(_renderTargetTexture, _texture, GetDefaultShader(), &dstRect, &srcRect, 1.0);
	}
	_sourceRect = {0, 0, _location.w, _location.h};
}

UINineSlice::~UINineSlice() {
	TextureDestroy(_texture);
	TextureDestroy(_renderTargetTexture);
}

void UINineSlice::OnDraw(float offsetX, float offsetY) {
	Color color = {255, 255, 255, 255};
	DrawTexture(_renderTargetTexture, GetDefaultShader(), &_location, &_sourceRect, false, _scale, false, &color);
}
