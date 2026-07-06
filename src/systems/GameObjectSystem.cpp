#include <Supergoon/map.h>

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
using namespace std;
namespace Etf {
static vector<shared_ptr<GameObject>> gameObjects;
static vector<weak_ptr<IInteractable>> interactableGameObjects;
static unordered_map<int, function<void(TiledObject* objData)>> gameobjectLoadFunctions = {
	{4, LocalPlayer::Create},
	{5, Textbox::Create},
	{2, MapExit::Create},
	{6, BattleLocation::Create},
};

void StartGameObjectSystem() {}
void UpdateGameObjectSystem() {
	for (auto& gameobject : gameObjects) {
		gameobject->Update();
	}
}

void LoadGameObjectSystem() {
	if (!_currentMap) return;
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

void DrawGameObjectSystem() {
	for (auto& gameobject : gameObjects) {
		gameobject->Draw();
	}
}

void AddGameObject(GameObject* gameobject) {
	gameObjects.push_back(shared_ptr<GameObject>(gameobject));
}


void ShutdownGameObjectSystem() {
	gameObjects.clear();
	interactableGameObjects.clear();
}
}  // namespace Etf
