#pragma once
#include <Supergoon/Primitives/rectangle.h>

#include <string>
class Sprite;

class sgGameObject;

namespace Etf {

struct Engine {
   public:
	static void LoadScene(const std::string& name = "");
	static Sprite* CreateSpriteFull(const std::string& name, sgGameObject* parent, RectangleF sourceRect, RectangleF offsetSizeRect);
	static unsigned int CreateAnimatorFull(const std::string& name, Sprite* sprite);
	static void StartAnimatorAnimation(unsigned int animator, const char*, float animSpeed = 1.0);
	static void UpdateAnimatorAnimationSpeed(unsigned int animator, float animSpeed);

   private:
	static void LoadAndPlayBGM(const std::string& name, float volume);
};
}  // namespace Etf
