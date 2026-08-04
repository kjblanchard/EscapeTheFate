#include <Supergoon/Animation/animator.h>
#include <Supergoon/sprite.h>
#include <Supergoon/state.h>
#include <sgtools/log.h>

#include <battle/abilityData.hpp>
#include <engine.hpp>
#include <format>
#include <gameobject/GameObject.hpp>
#include <gameobject/gameobjects/Battler.hpp>
#include <memory>
#include <ui/uiText.hpp>
using namespace std;
using namespace Etf;

Battler::~Battler() {
	DestroySprite(_sprite);
}

Battler::Battler(const BattlerArgs& args) : GameObject(args.BattleData->Location.x + args.X, args.BattleData->Location.y + args.Y), _battlerData(args.BattleData) {
	auto spriteName = format("{}.png", args.BattleData->Sprite.c_str());
	_sprite = Engine::Sprites::CreateSpriteFull(spriteName.c_str(), &X_, &Y_, {0, 0, args.BattleData->Location.w, args.BattleData->Location.h}, args.BattleData->Location);
	_animator = make_unique<SpriteAnimator>(args.BattleData->Sprite.c_str(), _sprite);
	_animator->StartAnimation(args.BattleData->IdleAnimation);
	_currentHP = _battlerData->HP;
	_currentATBCharge = 0;
	_maxATBCharge = 100;
	AddGameObjectToGameObjectSystem(this);
}

float Battler::SpriteWidth() {
	return _sprite->OffsetAndSizeRectF.w;
}

float Battler::SpriteHeight() {
	return _sprite->OffsetAndSizeRectF.h;
}

void Battler::TakeDamage(int damage) {
	_currentHP -= damage;
	_damageNumberPool.Show(damage, SpriteX() + SpriteWidth() / 2.0f, SpriteY() - 4.0f, false);
	takeDamageImpl(damage);
}

void Battler::Heal(int amount) {
	_currentHP += amount;
	if (_currentHP > _battlerData->HP) _currentHP = _battlerData->HP;
	_damageNumberPool.Show(amount, SpriteX() + SpriteWidth() / 2.0f, SpriteY() - 4.0f, true);
	healImpl(amount);
}

void Battler::PlayHitAnimation(const AbilityData& ability) {
	float x = SpriteX() + _battlerData->AnimOffsetX;
	float y = SpriteY() + _battlerData->AnimOffsetY;
	auto it = _hitAnimPools.find(ability.AnimationFile);
	if (it == _hitAnimPools.end()) {
		_hitAnimPools[ability.AnimationFile] = make_unique<HitAnimPool>(ability.AnimationFile, 64.0f, 64.0f);
		it = _hitAnimPools.find(ability.AnimationFile);
	}
	it->second->Play(ability.AnimationTag, x, y);
	if (!ability.SFXName.empty()) {
		Engine::Audio::PlaySFXBuffer(ability.SFXName, 1.0f);
	}
}

void Battler::updateHitAnims() {
	for (auto& [key, pool] : _hitAnimPools) {
		pool->Update(DeltaTimeSeconds);
	}
}

void Battler::updateDamageNumbers() {
	_damageNumberPool.Update(DeltaTimeSeconds);
}

void Battler::updateATBGauge() {
	if (_currentATBCharge >= _maxATBCharge) return;
	auto delta = DeltaTimeSeconds * 20;
	auto gaguePower = delta * _battlerData->Spd;
	_currentATBCharge += gaguePower;
	_currentATBCharge = _currentATBCharge > _maxATBCharge ? _maxATBCharge : _currentATBCharge;
}

void Battler::Draw() {
	for (auto& [key, pool] : _hitAnimPools) {
		pool->Draw();
	}
	_damageNumberPool.Draw(kDamageFont, kDamageFontSize);
}
