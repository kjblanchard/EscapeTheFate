#pragma once
#include <algorithm>
#include <vector>

struct TiledObject;
namespace Etf {
class GameObject;
class GameObjectSystem {
   public:
	static void Start();
	static void Update();
	static void Load();
	static void Draw();
	static void Shutdown();
	// Add a gameobject to be managed by the gameobject system
	static void AddGameObject(GameObject* gameobject);

	// Allows you go get a list of specific gameobjects, if you use specific interfaces on them, useful for querying
	template <typename T>
	static std::vector<T*>& GetGameObjectsOfType() {
		static std::vector<T*> list;
		return list;
	}

	template <typename T>
	static void AddGameObjectOfType(T* gameobject) {
		auto& list = GetGameObjectsOfType<T>();
		list.push_back(gameobject);
	}

	// Does not destroy the gameobject, but it does remove it from the vector it's tracked in
	template <typename T>
	static void RemoveGameObjectOfType(T* gameobject) {
		auto& list = GetGameObjectsOfType<T>();
		list.erase(std::remove(list.begin(), list.end(), gameobject), list.end());
	}
};
}  // namespace Etf
