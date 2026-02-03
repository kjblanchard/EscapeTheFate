#include <Supergoon/Graphics/shader.h>
#include <Supergoon/Graphics/texture.h>
#include <Supergoon/log.h>
#include <Supergoon/sprite.h>

#include <bindings/engine.hpp>
#include <ui/uiAnimation.hpp>
using namespace Etf;
using namespace std;

UIAnimation::UIAnimation(UIAnimationArgs& args) {
	_location = args.Rect;
	_sourceRect = args.SourceRect;
	_scale = args.Scale;
	_name = args.Name;
	_filename = args.Filename;
	_visible = args.Visible;
	_priority = args.Priority;
	_color = args.DrawColor;
	_debugBox = args.DebugBox;
	_sprite = Engine::CreateManualSpriteFull(args.Filename.c_str(), &locWithOffsetX, &locWithOffsetY, {0, 0, 32, 32}, {0, 0, 32, 32});
	_animator = make_unique<SpriteAnimator>(args.Filename, _sprite);
}

UIAnimation::~UIAnimation() {
	DestroySpriteManual(_sprite);
}

void UIAnimation::OnDraw(float offsetX, float offsetY) {
	locWithOffsetX = _location.x + offsetX;
	locWithOffsetY = _location.y + offsetY;
	RectangleF dst = {0, 0, _location.w, _location.h};
	// RectangleF dst = {0,0, _location.w, _location.h};
	Color color = {255, 255, 255, 255};
	DrawSpriteManual(_sprite, &dst, &color);
}
