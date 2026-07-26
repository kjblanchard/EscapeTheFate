#pragma once
#include <Supergoon/Primitives/Color.h>
#include <Supergoon/Primitives/rectangle.h>

#include <components/SpriteAnimator.hpp>
#include <memory>
#include <string>
struct Sprite;

namespace Etf {

static constexpr int kHitAnimPoolSize = 3;

struct HitAnimSlot {
	Sprite* sprite = nullptr;
	std::unique_ptr<SpriteAnimator> animator;
	float posX = 0.0f;
	float posY = 0.0f;
	bool active = false;
	float timer = 0.0f;
};

class HitAnimPool {
   public:
	HitAnimPool(const std::string& filename, float frameW, float frameH);
	~HitAnimPool();
	void Play(const std::string& animTag, float x, float y);
	void Update(float deltaSeconds);
	void Draw();

   private:
	HitAnimSlot _slots[kHitAnimPoolSize];
	float _frameW, _frameH;
};

}  // namespace Etf
