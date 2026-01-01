#include <Supergoon/log.h>
#include <Supergoon/map.h>

#include <gameState.hpp>
#include <gameobject/gameobjects/Player.hpp>
#include <memory>

#include "gameobject/GameObject.hpp"
using namespace std;
using namespace Etf;

void Player::Create(TiledObject* objData) {
	for (auto i = 0; i < objData->NumProperties; ++i) {
		auto prop = objData->Properties[i];
		if (prop.Name != string("loadLocation")) continue;
		auto loadLocation = prop.Data.IntData;
		if (loadLocation != GameState::NextLoadScreen) continue;
		sgLogDebug("Making player start at pos %d!!", loadLocation);
		auto player = new Player(objData);
		_gameObjects.push_back(unique_ptr<GameObject>(player));
	}
}

Player::Player(TiledObject* objData) : GameObject(objData->X, objData->Y) {
	sgLogWarn("Player created at x: %f y: %f", X(), Y());
}
Player::~Player() {
	sgLogWarn("Destroying Player!");
}
void Player::Start() {}
void Player::Update() {}
void Player::Draw() {}

void Player::UpdatePlayers() {}
void Player::DrawPlayers() {}
