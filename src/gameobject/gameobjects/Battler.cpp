#include <Supergoon/Animation/animator.h>
#include <Supergoon/log.h>
#include <Supergoon/state.h>

#include <bindings/engine.hpp>
#include <gameobject/gameobjects/Battler.hpp>
#include <memory>
#include <ui/uiText.hpp>

#include "gameobject/GameObject.hpp"
using namespace std;
using namespace Etf;

Battler::Battler(const BattlerArgs& args) : GameObject(args.BattleData->Location.x + args.X, args.BattleData->Location.y + args.Y), _battlerData(args.BattleData) {
	_sprite = Engine::CreateSpriteFull(args.BattleData->Sprite.c_str(), &_x, &_y, {0, 0, args.BattleData->Location.w, args.BattleData->Location.h}, args.BattleData->Location);
	// _animator = Engine::Animation::CreateAnimatorFull(args.BattleData->Sprite.c_str(), _sprite);
	_animator = make_unique<SpriteAnimator>(args.BattleData->Sprite.c_str(), _sprite);
	StartAnimation(args.BattleData->IdleAnimation, false);
	_gameObjects.push_back(shared_ptr<GameObject>(this));
	_currentHP = _battlerData->HP;
	_currentATBCharge = 0;
	_maxATBCharge = 100;
}

void Battler::TakeDamage(int damage) {
	_currentHP -= damage;
	takeDamageImpl(damage);
}

void Battler::StartAnimation(const std::string& name, bool backToIdle) {
	if (backToIdle) {
		_animator->PlayAnimationThenLoopSecond(name.c_str(), _battlerData->IdleAnimation.c_str());
	} else {
		_animator->StartAnimation(name.c_str());
	}
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
