#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
struct sgGameObject;

class TiledObject;
namespace Etf {

class GameObject {
   protected:
	float& X();
	float& Y();
	static std::vector<std::unique_ptr<GameObject>> _gameObjects;
	GameObject(int x, int y);
	bool DoNotDestroy = false;

   private:
	static std::unordered_map<int, std::function<void(TiledObject* objData)>> _loaderMap;
	sgGameObject* GO;

   public:
	static void LoadAllGameObjects();
	virtual ~GameObject() = default;

   protected:
	virtual void Start() {};
	virtual void Update() {};
	virtual void Draw() {};
};

}  // namespace Etf
