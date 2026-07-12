#include <gameobject/GameObject.hpp>
#include <systems/GameObjectSystem.hpp>
using namespace Etf;

unsigned int GameObject::_currentID = 0;

GameObject::GameObject(int x, int y) : X_(x), Y_(y), _id(_currentID++) {}
float GameObject::X() { return X_; }
float GameObject::Y() { return Y_; }
void GameObject::SetX(float x) { X_ = x; }
void GameObject::SetY(float y) { Y_ = y; }

void Etf::AddGameObjectToGameObjectSystem(GameObject* gameobject) {
	GameObjectSystem::AddGameObject(gameobject);
}

float* GameObject::GetXHandle() { return &X_; }
float* GameObject::GetYHandle() { return &Y_; };
