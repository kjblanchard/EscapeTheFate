#include <Supergoon/Animation/animator.h>

#include <bindings/engine.hpp>
#include <gameobject/gameobjects/Battler.hpp>
#include <memory>
#include <ui/uiText.hpp>

#include "gameobject/GameObject.hpp"
using namespace std;
using namespace Etf;

Battler::Battler(const BattlerArgs& args) : GameObject(args.BattleData->Location.x + args.X, args.BattleData->Location.y + args.Y), _battlerData(args.BattleData) {
	_sprite = Engine::CreateSpriteFull(args.BattleData->Sprite.c_str(), internalGO(), {0, 0, args.BattleData->Location.w, args.BattleData->Location.h}, args.BattleData->Location);
	_animator = Engine::Animation::CreateAnimatorFull(args.BattleData->Sprite.c_str(), _sprite);
	StartAnimation(args.BattleData->IdleAnimation, false);
	_gameObjects.push_back(shared_ptr<GameObject>(this));
	_currentHP = _battlerData->HP;
	_battlerUI = args.BattleUI;
	_battlerUI->UpdateHP(to_string(_currentHP));
}

void Battler::TakeDamage(int damage) {
	_currentHP -= damage;
	_battlerUI->UpdateHP(to_string(_currentHP));
}

Battler::~Battler() {
	Engine::Animation::DestroyAnimatorFull(_animator);
}

void Battler::StartAnimation(const std::string& name, bool backToIdle) {
	if (backToIdle) {
		PlayAnimation(_animator, name.c_str(), 1);
		AddAnimationToAnimatorQueue(_animator, _battlerData->IdleAnimation.c_str(), -1);
	} else {
		PlayAnimation(_animator, name.c_str(), -1);
	}
}

void Battler ::Draw() {
}
