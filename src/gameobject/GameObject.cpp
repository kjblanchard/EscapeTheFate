#include <gameobject/GameObject.hpp>
#include <systems/GameObjectSystem.hpp>
using namespace Etf;

unsigned int GameObject::_currentID = 0;

GameObject::GameObject(int x, int y) : x(x), y(y), _id(_currentID++) {}
float GameObject::X() { return x; }
float GameObject::Y() { return y; }
void GameObject::SetX(float newX) { x = newX; }
void GameObject::SetY(float newY) { y = newY; }

void Etf::AddGameObjectToGameObjectSystem(GameObject* gameobject) {
	GameObjectSystem::AddGameObject(gameobject);
}

float* GameObject::GetXHandle() { return &x; }
float* GameObject::GetYHandle() { return &y; };
