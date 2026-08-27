#include <Supergoon/Animation/animator.h>
#include <Supergoon/sprite.h>
#include <Supergoon/state.h>
#include <sgtools/log.h>

#include <battle/abilityData.hpp>
#include <engine.hpp>
#include <gameobject/GameObject.hpp>
#include <gameobject/gameobjects/Battler.hpp>
#include <memory>
#include <ui/uiText.hpp>
using namespace std;
using namespace Etf;

Battler::~Battler() {
	DestroySprite(sprite);
}

Battler::Battler(const BattlerArgs& args) : GameObject(args.BattleData->Location.x + args.X, args.BattleData->Location.y + args.Y), battlerData(args.BattleData) {
	auto spriteName = format("{}.png", args.BattleData->Sprite.c_str());
	sprite = Engine::Sprites::CreateSpriteFull(spriteName.c_str(), &x, &y, {0, 0, args.BattleData->Location.w, args.BattleData->Location.h}, args.BattleData->Location);
	animator = make_unique<SpriteAnimator>(args.BattleData->Sprite.c_str(), sprite);
	animator->StartAnimation(args.BattleData->IdleAnimation);
	currentHP = battlerData->HP;
	currentATBCharge = 0;
	maxATBCharge = 100;
	currentAP = 0;
	maxAP = battlerData->MaxAP;
	AddGameObjectToGameObjectSystem(this);
}

float Battler::SpriteWidth() {
	return sprite->OffsetAndSizeRectF.w;
}

float Battler::SpriteHeight() {
	return sprite->OffsetAndSizeRectF.h;
}

void Battler::handleTurnStartStatus() {
	for (auto& s : statusEffects) {
		HandleStatusEffect(s, StatusEffectTriggers::TurnStart, this, nullptr);
	}
}
void Battler::handleTurnEndStatus() {
	for (auto& s : statusEffects) {
		HandleStatusEffect(s, StatusEffectTriggers::TurnEnd, this, nullptr);
	}
}
void Battler::handleShouldApplyStatus(Battler* b) {
	for (auto& s : statusEffects) {
		HandleStatusEffect(s, StatusEffectTriggers::DamageTaken, this, b);
	}
}

int Battler::absorbDamage(int rawDamage) {
	for (auto& s : statusEffects) {
		if (s.StatusType != StatusEffects::RelicShield) continue;
		if (s.Duration <= 0) continue;
		int absorbed = std::min(s.Duration, rawDamage);
		s.Duration -= absorbed;
		rawDamage -= absorbed;
		if (rawDamage <= 0) return 0;
	}
	return rawDamage;
}

int Battler::GetOutgoingDamageBonus() const {
	int bonus = 0;
	for (const auto& s : statusEffects) {
		if (s.StatusType == StatusEffects::RelicDamageBonus && s.Duration != 0) {
			bonus += 1;
		}
	}
	return bonus;
}

void Battler::AddSpdBonus(int amount) {
	spdBonus += amount;
}

void Battler::TakeDamage(int damage) {
	damage = absorbDamage(damage);
	if (damage <= 0 || currentHP <= 0) return;
	currentHP -= damage;
	damageNumberPool.Show(damage, (SpriteX() + SpriteWidth() / 2.0f) + battlerData->DamageOffsetX, (SpriteY() - 4.0f) + battlerData->DamageOffsetY, false);
	takeDamageImpl(damage);
}

void Battler::Heal(int amount) {
	currentHP += amount;
	if (currentHP > battlerData->HP) currentHP = battlerData->HP;
	damageNumberPool.Show(amount, SpriteX() + SpriteWidth() / 2.0f, SpriteY() - 4.0f, true);
	healImpl(amount);
}

void Battler::PlayHitAnimation(const AbilityData& ability) {
	float x = SpriteX() + battlerData->AnimOffsetX;
	float y = SpriteY() + battlerData->AnimOffsetY;
	auto it = hitAnimPools.find(ability.AnimationFile);
	if (it == hitAnimPools.end()) {
		hitAnimPools[ability.AnimationFile] = make_unique<HitAnimPool>(ability.AnimationFile, 64.0f, 64.0f);
		it = hitAnimPools.find(ability.AnimationFile);
	}
	it->second->Play(ability.AnimationTag, x, y);
	if (!ability.SFXName.empty()) {
		Engine::Audio::PlaySFXBuffer(ability.SFXName, 1.0f);
	}
}

void Battler::updateHitAnims() {
	for (auto& [key, pool] : hitAnimPools) {
		pool->Update(DeltaTimeSeconds);
	}
}

void Battler::updateDamageNumbers() {
	damageNumberPool.Update(DeltaTimeSeconds);
}

void Battler::updateATBGauge() {
	if (currentAP >= maxAP) return;
	if (currentATBCharge >= maxATBCharge) {
		currentAP++;
		if (currentAP > maxAP) currentAP = maxAP;
		currentATBCharge = (currentAP >= maxAP) ? (float)maxATBCharge : 0;
		onAPGained();
		return;
	}
	auto delta = DeltaTimeSeconds * 20;
	auto gaguePower = delta * (battlerData->Spd + spdBonus);
	currentATBCharge += gaguePower;
	currentATBCharge = currentATBCharge > maxATBCharge ? maxATBCharge : currentATBCharge;
}

bool Battler::spendAP(int cost) {
	if (currentAP < cost) return false;
	currentAP -= cost;
	return true;
}

void Battler::draw() {
	for (auto& [key, pool] : hitAnimPools) {
		pool->Draw();
	}
	damageNumberPool.Draw(kDamageFont, kDamageFontSize);
}
