#include <Supergoon/gameobject.h>
#include <Supergoon/map.h>

#include <gameobject/GameObject.hpp>
#include <gameobject/gameobjects/Player.hpp>
#include <gameobject/gameobjects/Textbox.hpp>
#include <interfaces/IInteractable.hpp>
using namespace Etf;
using namespace std;

std::vector<std::unique_ptr<GameObject>> GameObject::_gameObjects;
std::vector<IInteractable*> GameObject::_interactables;
std::unordered_map<int, std::function<void(TiledObject* objData)>> GameObject::_loaderMap = {
	{4, Player::Create},
	{5, Textbox::Create},
};

GameObject::GameObject(int x, int y) {
	GO = GameObjectCreate();
	GO->X = x;
	GO->Y = y;
}

float& GameObject::X() {
	return GO->X;
}

float& GameObject::Y() {
	return GO->Y;
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
	for (auto i = 0; i < _currentMap->NumObjects; ++i) {
		auto currentObject = &_currentMap->Objects[i];
		auto it = _loaderMap.find(currentObject->ObjectType);
		if (it == _loaderMap.end()) continue;
		it->second(currentObject);
	}
}
