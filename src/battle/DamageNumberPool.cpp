#include <Supergoon/text.h>

#include <battle/DamageNumberPool.hpp>
#include <cstdio>
#include <engine.hpp>

using namespace Etf;

void DamageNumberPool::Show(int amount, float x, float y, bool isHeal) {
	DamageNumberSlot* target = nullptr;
	for (int i = 0; i < kDamageNumberPoolSize; ++i) {
		if (!_slots[i].active) {
			target = &_slots[i];
			break;
		}
	}
	if (!target) target = &_slots[0];

	snprintf(target->text, sizeof(target->text), "%d", amount);
	target->startX = x;
	target->startY = y;
	target->velocityX = kDamageNumberArcWidth * _nextDirection;
	target->velocityY = -kDamageNumberArcHeight / (kDamageNumberDuration * 0.4f);
	target->timer = 0.0f;
	target->active = true;
	target->isHeal = isHeal;
	_nextDirection = -_nextDirection;
}

void DamageNumberPool::Update(float deltaSeconds) {
	for (int i = 0; i < kDamageNumberPoolSize; ++i) {
		if (!_slots[i].active) continue;
		_slots[i].timer += deltaSeconds;
		if (_slots[i].timer >= kDamageNumberDuration) {
			_slots[i].active = false;
		}
	}
}

void DamageNumberPool::Draw(const char* fontName, unsigned int fontSize) {
	for (int i = 0; i < kDamageNumberPoolSize; ++i) {
		auto& slot = _slots[i];
		if (!slot.active) continue;

		float t = slot.timer;
		float progress = t / kDamageNumberDuration;

		float xOffset = slot.velocityX * t;
		float yOffset = slot.velocityY * t + 0.5f * kDamageNumberGravity * t * t;

		uint8_t alpha = 255;
		if (progress > kDamageNumberFadeStart) {
			float fadeProgress = (progress - kDamageNumberFadeStart) / (1.0f - kDamageNumberFadeStart);
			alpha = (uint8_t)(255.0f * (1.0f - fadeProgress));
		}

		Color color;
		if (slot.isHeal) {
			color = {100, 255, 100, alpha};
		} else {
			color = {255, 255, 255, alpha};
		}

		int width = TextMeasureStringDirect(slot.text, fontName, fontSize);
		float drawX = slot.startX + xOffset - width / 2.0f;
		float drawY = slot.startY + yOffset;
		TextDrawStringDirect(slot.text, fontName, fontSize, drawX, drawY, &color, 0);
	}
}
