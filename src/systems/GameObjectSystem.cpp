#include <Supergoon/map.h>

#include <functional>
#include <gameobject/GameObject.hpp>
#include <gameobject/gameobjects/BattleLocation.hpp>
#include <gameobject/gameobjects/BattleZone.hpp>
#include <gameobject/gameobjects/LocalPlayer.hpp>
#include <gameobject/gameobjects/MapExit.hpp>
#include <gameobject/gameobjects/Textbox.hpp>
#include <interfaces/IInteractable.hpp>
#include <memory>
#include <systems/GameObjectSystem.hpp>
#include <unordered_map>
#include <vector>
using namespace std;
namespace Etf {
namespace GameObjectSystem {
static vector<shared_ptr<GameObject>> gameObjects;
static vector<weak_ptr<IInteractable>> interactableGameObjects;
static unordered_map<int, function<void(TiledObject* objData)>> gameobjectLoadFunctions = {
	{1, BattleZone::Create},
	{4, LocalPlayer::Create},
	{5, Textbox::Create},
	{2, MapExit::Create},
	{6, BattleLocation::Create},
};

void Start() {}
void Update() {
	for (auto& gameobject : gameObjects) {
		gameobject->Update();
	}
}

void Load() {
	if (!_currentMap) return;
	BattleLocation::ClearAllBattleLocations();
	for (auto& currentGo : gameObjects) {
		if (currentGo->DoNotDestroy_) continue;
		currentGo->ShouldBeDestroyed_ = true;
	}
	// Load new gameobjects
	for (auto i = 0; i < _currentMap->NumObjects; ++i) {
		auto currentObject = &_currentMap->Objects[i];
		auto it = gameobjectLoadFunctions.find(currentObject->ObjectType);
		if (it == gameobjectLoadFunctions.end()) continue;
		it->second(currentObject);
	}
	gameObjects.erase(
		std::remove_if(
			gameObjects.begin(),
			gameObjects.end(),
			[](auto& go) {
				return go->ShouldBeDestroyed_;
			}),
		gameObjects.end());
}

void Draw() {
	for (auto& gameobject : gameObjects) {
		gameobject->Draw();
	}
}

void AddGameObject(GameObject* gameobject) {
	gameObjects.push_back(shared_ptr<GameObject>(gameobject));
}

void Shutdown() {
	gameObjects.clear();
	interactableGameObjects.clear();
}
}  // namespace GameObjectSystem
}  // namespace Etf
