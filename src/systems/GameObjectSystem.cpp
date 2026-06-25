#include <Supergoon/map.h>

#include <algorithm>
#include <functional>
#include <gameobject/GameObject.hpp>
#include <gameobject/gameobjects/BattleLocation.hpp>
#include <gameobject/gameobjects/LocalPlayer.hpp>
#include <gameobject/gameobjects/MapExit.hpp>
#include <gameobject/gameobjects/Textbox.hpp>
#include <interfaces/IInteractable.hpp>
#include <memory>
#include <systems/GameObjectSystem.hpp>
#include <unordered_map>
#include <vector>
using namespace Etf;
using namespace std;

std::vector<shared_ptr<GameObject>> kGameObjects;
std::vector<weak_ptr<IInteractable>> kInteractables;
std::unordered_map<int, std::function<void(TiledObject* objData)>> kLoadMap = {
	{4, LocalPlayer::Create},
	{5, Textbox::Create},
	{2, MapExit::Create},
	{6, BattleLocation::Create},
};

void GameObjectSystem::Load() {
	if (!_currentMap) return;
	for (auto& currentGo : kGameObjects) {
		if (currentGo->DoNotDestroy_) continue;
		currentGo->ShouldBeDestroyed_ = true;
	}
	// Load new gameobjects
	for (auto i = 0; i < _currentMap->NumObjects; ++i) {
		auto currentObject = &_currentMap->Objects[i];
		auto it = kLoadMap.find(currentObject->ObjectType);
		if (it == kLoadMap.end()) continue;
		it->second(currentObject);
	}
	// Remove_if basically moves the items to the end of the vector and moves around things, but does not actually erase them, which is needed to destroy the players properly
	kGameObjects.erase(
		std::remove_if(
			kGameObjects.begin(),
			kGameObjects.end(),
			[](auto& go) {
				return go->ShouldBeDestroyed_;
			}),
		kGameObjects.end());
}

void GameObjectSystem::Start() {}

void GameObjectSystem::Update() {
	for (auto& gameobject : kGameObjects) {
		gameobject->Update();
	}
}

void GameObjectSystem::AddGameObject(GameObject* gameobject) {
	kGameObjects.push_back(shared_ptr<GameObject>(gameobject));
}

void GameObjectSystem::Draw() {
	for (auto& gameobject : kGameObjects) {
		gameobject->Draw();
	}
}

void GameObjectSystem::Shutdown() {
	kGameObjects.clear();
	kInteractables.clear();
}
