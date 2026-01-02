#pragma once
#include <Supergoon/Primitives/Color.h>
#include <Supergoon/Primitives/rectangle.h>

#include <string>
class Sprite;

class sgGameObject;

namespace Etf {

struct Engine {
   public:
	static void LoadScene(const std::string& name = "");
	static Sprite* CreateSpriteFull(const std::string& name, sgGameObject* parent, RectangleF sourceRect, RectangleF offsetSizeRect);
	static void SetSpriteVisible(Sprite* sprite, bool visible);
	static unsigned int CreateAnimatorFull(const std::string& name, Sprite* sprite);
	static void StartAnimatorAnimation(unsigned int animator, const char*, float animSpeed = 1.0);
	static void UpdateAnimatorAnimationSpeed(unsigned int animator, float animSpeed);
	static void DrawRectPrimitive(RectangleF& rect, Color color = {255, 0, 0, 255}, bool filled = false, bool cameraOffset = true);

	// function engine.Collision.CheckForCollision(a, b)
	// return a.x < b.x + b.w and
	// a.x + a.w > b.x and
	// a.y < b.y + b.h and
	// a.y + a.h > b.y
	// end

	static inline bool CheckForRectCollision(RectangleF& lhs, RectangleF& rhs) {
		return lhs.x < rhs.x + rhs.w && lhs.x + lhs.w > rhs.x && lhs.y < rhs.y + rhs.h && lhs.y + lhs.h > rhs.y;
	}

   private:
	static void LoadAndPlayBGM(const std::string& name, float volume);
};
}  // namespace Etf
