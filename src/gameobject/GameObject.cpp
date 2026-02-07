#include <Supergoon/log.h>
#include <Supergoon/map.h>

#include <algorithm>
#include <gameobject/GameObject.hpp>
#include <gameobject/gameobjects/BattleLocation.hpp>
#include <gameobject/gameobjects/MapExit.hpp>
#include <gameobject/gameobjects/Player.hpp>
#include <gameobject/gameobjects/Textbox.hpp>
#include <interfaces/IInteractable.hpp>
#include <memory>
using namespace Etf;
using namespace std;

unsigned int GameObject::_currentID = 0;

std::vector<shared_ptr<GameObject>> GameObject::_gameObjects;
std::vector<weak_ptr<IInteractable>> GameObject::_interactables;
std::unordered_map<int, std::function<void(TiledObject* objData)>> GameObject::_loaderMap = {
	{4, Player::Create},
	{5, Textbox::Create},
	{2, MapExit::Create},
	{6, BattleLocation::Create},
};

GameObject::GameObject(int x, int y) : _x(x), _y(y), _id(_currentID++) {
}

GameObject::~GameObject() {}

float& GameObject::X() {
	return _x;
}

float& GameObject::Y() {
	return _y;
}

void GameObject::UpdateGameObjects() {
	for (auto& gameobject : _gameObjects) {
		gameobject->Update();
	}
}

void GameObject::DrawGameObjects() {
	for (auto& gameobject : _gameObjects) {
		gameobject->Draw();
	}
}

void GameObject::LoadAllGameObjects() {
	if (!_currentMap) return;
	for (auto& currentGo : _gameObjects) {
		if (currentGo->DoNotDestroy) continue;
		currentGo->ShouldBeDestroyed = true;
	}
	// Load new gameobjects
	for (auto i = 0; i < _currentMap->NumObjects; ++i) {
		auto currentObject = &_currentMap->Objects[i];
		auto it = _loaderMap.find(currentObject->ObjectType);
		if (it == _loaderMap.end()) continue;
		it->second(currentObject);
	}
	// Remove_if basically moves the items to the end of the vector and moves around things, but does not actually erase them, which is needed to destroy the players properly
	_gameObjects.erase(
		std::remove_if(
			_gameObjects.begin(),
			_gameObjects.end(),
			[](auto& go) {
				return go->ShouldBeDestroyed;
			}),
		_gameObjects.end());
}

void GameObject::DestroyAllGameObjects() {
	_gameObjects.clear();
	_interactables.clear();
}

void GameObject::AddGameObject(GameObject* gameobject) {
	_gameObjects.push_back(shared_ptr<GameObject>(gameobject));
}
