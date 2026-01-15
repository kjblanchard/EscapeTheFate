#include <Supergoon/Animation/animator.h>

#include <bindings/engine.hpp>
#include <gameobject/gameobjects/Battler.hpp>
#include <memory>

#include "gameobject/GameObject.hpp"
using namespace std;
using namespace Etf;

Battler::Battler(BattlerData* battleData, int x, int y) : GameObject(battleData->Location.x + x, battleData->Location.y + y), _battlerData(battleData) {
	_sprite = Engine::CreateSpriteFull(battleData->Sprite.c_str(), internalGO(), {0, 0, battleData->Location.w, battleData->Location.h}, battleData->Location);
	_animator = Engine::Animation::CreateAnimatorFull(battleData->Sprite.c_str(), _sprite);
	StartAnimation(battleData->IdleAnimation);
	_gameObjects.push_back(shared_ptr<GameObject>(this));
}

Battler::~Battler() {
	Engine::Animation::DestroyAnimatorFull(_animator);
}

void Battler::StartAnimation(const std::string& name) {
	PlayAnimation(_animator, name.c_str(), -1);
}

void Battler ::Draw() {
}
