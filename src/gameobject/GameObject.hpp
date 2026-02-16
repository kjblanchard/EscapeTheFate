#pragma once
struct TiledObject;
namespace Etf {

class IInteractable;

class GameObject {
   public:
	float& X();
	float& Y();

   protected:
	GameObject(int x, int y);
	bool DoNotDestroy_ = false;
	bool ShouldBeDestroyed_ = false;
	float X_ = 0;
	float Y_ = 0;

   private:
	static unsigned int _currentID;
	unsigned int _id;

   public:
	virtual ~GameObject() = default;

   protected:
	virtual void Start() {};
	virtual void Update() {};
	virtual void Draw() {};

	friend class GameObjectSystem;
};

void AddGameObjectToGameObjectSystem(GameObject* gameobject);

}  // namespace Etf
