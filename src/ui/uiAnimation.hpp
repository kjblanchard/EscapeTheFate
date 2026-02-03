#pragma once
#include <Supergoon/Primitives/Color.h>
#include <Supergoon/Primitives/rectangle.h>

#include <bindings/SpriteAnimator.hpp>
#include <memory>
#include <string>
#include <ui/uiObject.hpp>
struct Sprite;
namespace Etf {
struct UIAnimationArgs {
	std::string Filename, Name;
	RectangleF Rect;
	RectangleF SourceRect;
	float Scale;
	Color DrawColor;
	int Priority;

	bool Visible, DebugBox = false;
};
class UIAnimation : public UIObject {
   public:
	UIAnimation(UIAnimationArgs& args);
	~UIAnimation();
	inline const SpriteAnimator& GetAnimator() { return *_animator.get(); }
	void OnDraw(float offsetX, float offsetY) override final;

   private:
	float locWithOffsetX = 0;
	float locWithOffsetY = 0;
	Sprite* _sprite;
	RectangleF _sourceRect;
	float _scale;
	std::string _filename;
	Color _color;
	std::unique_ptr<SpriteAnimator> _animator;
};
}  // namespace Etf
