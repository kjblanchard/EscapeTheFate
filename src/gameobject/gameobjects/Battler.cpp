#include <Supergoon/Animation/animator.h>
#include <Supergoon/sprite.h>
#include <Supergoon/state.h>
#include <sgtools/log.h>

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
	// auto spriteJson = format("{}.png", args.BattleData->Sprite.c_str());
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
	takeDamageImpl(damage);
}

void Battler::updateATBGauge() {
	if (_currentATBCharge >= _maxATBCharge) return;
	auto delta = DeltaTimeSeconds * 20;
	auto gaguePower = delta * _battlerData->Spd;
	_currentATBCharge += gaguePower;
	_currentATBCharge = _currentATBCharge > _maxATBCharge ? _maxATBCharge : _currentATBCharge;
}

// Right now this is taken care of by the sprite and animation system from the engine
void Battler ::Draw() {}
