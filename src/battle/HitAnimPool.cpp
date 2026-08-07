#include <Supergoon/sprite.h>

#include <battle/HitAnimPool.hpp>
#include <engine.hpp>
#include <format>

using namespace Etf;
using namespace std;

namespace {
constexpr Color kWhite = {255, 255, 255, 255};
}  // namespace

HitAnimPool::HitAnimPool(const string& filename, float frameW, float frameH)
	: _frameW(frameW), _frameH(frameH) {
	auto pngName = format("{}.png", filename);
	for (int i = 0; i < kHitAnimPoolSize; ++i) {
		auto& slot = _slots[i];
		RectangleF sourceRect = {0, 0, _frameW, _frameH};
		RectangleF offsetSize = {0, 0, _frameW, _frameH};
		slot.sprite = Engine::Sprites::CreateManualSpriteFull(pngName, &slot.posX, &slot.posY, sourceRect, offsetSize);
		Engine::Sprites::SetSpriteVisible(slot.sprite, false);
		slot.animator = make_unique<SpriteAnimator>(filename, slot.sprite);
	}
}

HitAnimPool::~HitAnimPool() {
	for (int i = 0; i < kHitAnimPoolSize; ++i) {
		_slots[i].animator.reset();
		if (_slots[i].sprite) {
			DestroySpriteManual(_slots[i].sprite);
			_slots[i].sprite = nullptr;
		}
	}
}

void HitAnimPool::Play(const string& animTag, float x, float y) {
	for (int i = 0; i < kHitAnimPoolSize; ++i) {
		auto& slot = _slots[i];
		if (!slot.active) {
			slot.posX = x;
			slot.posY = y;
			slot.active = true;
			float duration = slot.animator->GetAnimationDuration(animTag);
			slot.timer = duration > 0 ? duration + 0.05f : 0.5f;
			slot.animator->StartAnimation(animTag, 1);
			Engine::Sprites::SetSpriteVisible(slot.sprite, true);
			return;
		}
	}
	// All slots busy - restart the oldest (slot 0)
	auto& slot = _slots[0];
	slot.posX = x;
	slot.posY = y;
	slot.active = true;
	float duration = slot.animator->GetAnimationDuration(animTag);
	slot.timer = duration > 0 ? duration + 0.05f : 0.5f;
	slot.animator->StartAnimation(animTag, 1);
	Engine::Sprites::SetSpriteVisible(slot.sprite, true);
}

void HitAnimPool::Update(float deltaSeconds) {
	for (int i = 0; i < kHitAnimPoolSize; ++i) {
		auto& slot = _slots[i];
		if (!slot.active) continue;
		slot.timer -= deltaSeconds;
		if (slot.timer <= 0) {
			slot.active = false;
			Engine::Sprites::SetSpriteVisible(slot.sprite, false);
		}
	}
}

void HitAnimPool::Draw() {
	for (int i = 0; i < kHitAnimPoolSize; ++i) {
		auto& slot = _slots[i];
		if (!slot.active) continue;
		RectangleF dst = {0, 0, _frameW, _frameH};
		Color color = kWhite;
		DrawSpriteManual(slot.sprite, &dst, &color, false);
	}
}
