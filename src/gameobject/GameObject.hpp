#pragma once
#include "systems/GameObjectSystem.hpp"
struct TiledObject;
namespace Etf {

class IInteractable;

class GameObject {
   public:
	float X();
	float Y();
	void SetX(float x);
	void SetY(float y);
	float* GetXHandle();
	float* GetYHandle();

   protected:
	GameObject(int x, int y);
	bool doNotDestroy = false;
	bool shouldDestroy = false;
	float x = 0;
	float y = 0;

   private:
	static unsigned int _currentID;
	unsigned int _id;

   public:
	virtual ~GameObject() = default;

   protected:
	virtual void start() {};
	virtual void update() {};
	virtual void draw() {};

	friend void GameObjectSystem::Update();
	friend void GameObjectSystem::Load();
	friend void GameObjectSystem::Draw();
};

void AddGameObjectToGameObjectSystem(GameObject* gameobject);

}  // namespace Etf
