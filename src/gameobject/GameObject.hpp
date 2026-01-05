#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
struct sgGameObject;
struct TiledObject;
namespace Etf {

class IInteractable;

class GameObject {
   protected:
	float& X();
	float& Y();
	inline sgGameObject* internalGO() { return GO; }
	static std::vector<std::shared_ptr<GameObject>> _gameObjects;
	static std::vector<std::weak_ptr<IInteractable>> _interactables;
	GameObject(int x, int y);
	bool DoNotDestroy = false;
	bool ShouldBeDestroyed = false;

   private:
	static std::unordered_map<int, std::function<void(TiledObject* objData)>> _loaderMap;
	sgGameObject* GO;

   public:
	// Loads all gameobjects in the current map, then destroys all the old unless they have donotdestroy
	static void LoadAllGameObjects();
	static void UpdateGameObjects();
	static void DrawGameObjects();
	// Destroys all gameobjects and interactables
	static void DestroyAllGameObjects();
	virtual ~GameObject();

   protected:
	virtual void Start() {};
	virtual void Update() {};
	virtual void Draw() {};
};

}  // namespace Etf
