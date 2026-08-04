#pragma once
#include <Supergoon/Primitives/Color.h>

namespace Etf {

static constexpr int kDamageNumberPoolSize = 8;
static constexpr float kDamageNumberDuration = 0.9f;
static constexpr float kDamageNumberArcHeight = 18.0f;
static constexpr float kDamageNumberArcWidth = 14.0f;
static constexpr float kDamageNumberGravity = 80.0f;
static constexpr float kDamageNumberFadeStart = 0.4f;

struct DamageNumberSlot {
	char text[8] = {};
	float startX = 0.0f;
	float startY = 0.0f;
	float velocityX = 0.0f;
	float velocityY = 0.0f;
	float timer = 0.0f;
	bool active = false;
	bool isHeal = false;
};

class DamageNumberPool {
   public:
	void Show(int amount, float x, float y, bool isHeal);
	void Update(float deltaSeconds);
	void Draw(const char* fontName, unsigned int fontSize);

   private:
	DamageNumberSlot _slots[kDamageNumberPoolSize] = {};
	int _nextDirection = 1;
};

}  // namespace Etf
