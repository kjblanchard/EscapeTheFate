#pragma once
#include <algorithm>
#include <vector>

struct TiledObject;
namespace Etf {
class GameObject;
void StartGameObjectSystem();
void LoadGameObjectSystem();
void UpdateGameObjectSystem();
void DrawGameObjectSystem();
// Add a gameobject to be managed by the gameobject system
void AddGameObject(GameObject* gameobject);
void ShutdownGameObjectSystem();

// Allows you go get a list of specific gameobjects, if you use specific interfaces on them, useful for querying
template <typename T>
std::vector<T*>& GetGameObjectsOfType() {
	static std::vector<T*> list;
	return list;
}

template <typename T>
void AddGameObjectOfType(T* gameobject) {
	auto& list = GetGameObjectsOfType<T>();
	list.push_back(gameobject);
}

// Does not destroy the gameobject, but it does remove it from the vector it's tracked in
template <typename T>
void RemoveGameObjectOfType(T* gameobject) {
	auto& list = GetGameObjectsOfType<T>();
	list.erase(std::remove(list.begin(), list.end(), gameobject), list.end());
}
}  // namespace Etf
