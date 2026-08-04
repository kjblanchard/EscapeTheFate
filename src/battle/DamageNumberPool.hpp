#pragma once
#include <Supergoon/Primitives/Color.h>

namespace Etf {

static constexpr int kDamageNumberPoolSize = 8;
static constexpr float kDamageNumberDuration = 0.8f;
static constexpr float kDamageNumberRise = 20.0f;
static constexpr float kDamageNumberFadeStart = 0.35f;

struct DamageNumberSlot {
	char text[8] = {};
	float startX = 0.0f;
	float startY = 0.0f;
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
};

}  // namespace Etf
