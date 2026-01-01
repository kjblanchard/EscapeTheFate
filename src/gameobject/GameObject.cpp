#include <Supergoon/gameobject.h>
#include <Supergoon/map.h>

#include <gameobject/GameObject.hpp>
#include <gameobject/gameobjects/Player.hpp>
using namespace Etf;
using namespace std;

std::vector<std::unique_ptr<GameObject>> GameObject::_gameObjects;
std::unordered_map<int, std::function<void(TiledObject* objData)>> GameObject::_loaderMap = {
	{4, Player::Create},
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

void GameObject::LoadAllGameObjects() {
	for (auto i = 0; i < _currentMap->NumObjects; ++i) {
		auto currentObject = &_currentMap->Objects[i];
		auto it = _loaderMap.find(currentObject->ObjectType);
		if (it == _loaderMap.end()) continue;
		it->second(currentObject);
	}
}
