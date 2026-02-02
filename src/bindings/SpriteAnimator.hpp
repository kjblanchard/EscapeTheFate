#pragma once
#include <Supergoon/Animation/animationdata.h>
#include <Supergoon/Animation/animator.h>

#include <memory>
#include <string>

typedef struct Sprite Sprite;

namespace Etf {

class SpriteAnimator {
	// Filename is for the animation data AND the json

   public:
	SpriteAnimator(const std::string& filename, Sprite* sprite);
	~SpriteAnimator();
	// Plays the first animation once, then loops on the second
	void PlayAnimationThenLoopSecond(const std::string& first, const std::string& second) const;
	//-1 loops is forever, 1.0 speed is regular.
	void StartAnimation(const std::string& animName, int loops = -1, float speed = 1.0f) const;
	// 1.0 is regular
	void UpdateAnimatorSpeed(float speed);
	// Adds to the playing queue
	void AddAnimationToQueue();

   private:
	std::shared_ptr<AnimationData> _animationData;
	Animator* _animator;
};

}  // namespace Etf
