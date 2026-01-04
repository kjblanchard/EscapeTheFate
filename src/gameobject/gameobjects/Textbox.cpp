#include <Supergoon/gameobject.h>
#include <Supergoon/log.h>
#include <Supergoon/map.h>

#include <bindings/engine.hpp>
#include <gameConfig.hpp>
#include <gameobject/gameobjects/Textbox.hpp>
#include <memory>

#include "gameobject/GameObject.hpp"
using namespace Etf;
using namespace std;

void Textbox::Create(TiledObject* objData) {
	auto textbox = new Textbox(objData);
	_gameObjects.push_back(unique_ptr<GameObject>(textbox));
	_interactables.push_back(textbox);
}

Textbox::Textbox(TiledObject* objData) : GameObject(objData->X, objData->Y) {
	InteractionRect.x = X();
	InteractionRect.y = Y();
	InteractionRect.w = objData->Width;
	InteractionRect.h = objData->Height;
}

// TODO probably switch to using shared ptrs in gameobjects, and then weak ptrs inside of the interactables.. so that they will expire when dead.
Textbox::~Textbox() {
	auto it = std::find(_interactables.begin(), _interactables.end(), this);
	if (it != _interactables.end())
		_interactables.erase(it);
}

void Textbox::Interact() {
}

void Textbox::Draw() {
	if (GameConfig::GetGameConfig().debug.interactions) Engine::DrawRectPrimitive(InteractionRect);
}
