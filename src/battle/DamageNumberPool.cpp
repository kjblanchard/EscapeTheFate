#include <Supergoon/text.h>

#include <battle/DamageNumberPool.hpp>
#include <cstdio>
#include <engine.hpp>

using namespace Etf;

void DamageNumberPool::Show(int amount, float x, float y, bool isHeal) {
	for (int i = 0; i < kDamageNumberPoolSize; ++i) {
		if (!_slots[i].active) {
			auto& slot = _slots[i];
			snprintf(slot.text, sizeof(slot.text), "%d", amount);
			slot.startX = x;
			slot.startY = y;
			slot.timer = 0.0f;
			slot.active = true;
			slot.isHeal = isHeal;
			return;
		}
	}
	auto& slot = _slots[0];
	snprintf(slot.text, sizeof(slot.text), "%d", amount);
	slot.startX = x;
	slot.startY = y;
	slot.timer = 0.0f;
	slot.active = true;
	slot.isHeal = isHeal;
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

		float progress = slot.timer / kDamageNumberDuration;
		float yOffset = Engine::Tweening::GetTweenedValue(0.0f, -kDamageNumberRise, slot.timer, kDamageNumberDuration, Engine::Tweening::TweenEaseTypes::QuintOut);

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
		float drawX = slot.startX - width / 2.0f;
		float drawY = slot.startY + yOffset;
		TextDrawStringDirect(slot.text, fontName, fontSize, drawX, drawY, &color, 0);
	}
}
