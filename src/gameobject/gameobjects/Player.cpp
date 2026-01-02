#include <Supergoon/Input/keyboard.h>
#include <Supergoon/Primitives/rectangle.h>
#include <Supergoon/camera.h>
#include <Supergoon/log.h>
#include <Supergoon/map.h>

#include <bindings/engine.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/Player.hpp>
#include <memory>

#include "gameobject/GameObject.hpp"
using namespace std;
using namespace Etf;

static const int _moveSpeed = 100;
static const int UP = 26;
static const int LEFT = 4;
static const int DOWN = 22;
static const int RIGHT = 7;
// static const int A = 44;
// static const int B = 27;
static RectangleF _collisionOffsetAndSizeRect = {8, 8, 16, 22};

void Player::Create(TiledObject* objData) {
	for (auto i = 0; i < objData->NumProperties; ++i) {
		auto prop = objData->Properties[i];
		if (prop.Name != string("loadLocation")) continue;
		auto loadLocation = prop.Data.IntData;
		if (loadLocation != GameState::NextLoadScreen) continue;
		sgLogDebug("Making player start at pos %d!!", loadLocation);
		auto player = new Player(objData);
		SetCameraFollowTarget(&player->X(), &player->Y());
		_gameObjects.push_back(unique_ptr<GameObject>(player));
	}
}

Player::Player(TiledObject* objData) : GameObject(objData->X, objData->Y) {
	sgLogWarn("Player created at x: %f y: %f", X(), Y());
	_sprite = Engine::CreateSpriteFull("player1", internalGO(), {0, 0, 32, 32}, {0, 0, 32, 32});
	_animator = Engine::CreateAnimatorFull("player1", _sprite);
}
Player::~Player() {
	// DestroyAnimator(Animator);
	sgLogWarn("Destroying Player!");
}
void Player::Start() {}
void Player::Update() {
	handlePlayerMovement();
}
bool Player::handlePlayerMovement() {
	auto moved = false;
	auto previousDirection = _direction;
	auto velocityX = 0;
	auto velocityY = 0;
	if (IsKeyboardKeyDown(UP)) {
		moved = true;
		velocityY -= 1;
		_direction = Direction::North;
	}

	if (IsKeyboardKeyDown(DOWN)) {
		moved = true;
		velocityY += 1;
		_direction = Direction::South;
	}

	if (IsKeyboardKeyDown(LEFT)) {
		moved = true;
		velocityX -= 1;
		_direction = Direction::West;
	}

	if (IsKeyboardKeyDown(RIGHT)) {
		moved = true;
		velocityX += 1;
		_direction = Direction::East;
	}

	if (moved) {
		float desiredX = (X() + velocityX * _moveSpeed * GameState::DeltaTimeSeconds);
		float desiredY = (Y() + velocityY * _moveSpeed * GameState::DeltaTimeSeconds);
		RectangleF collisionRect = {desiredX + _collisionOffsetAndSizeRect.x, desiredY + _collisionOffsetAndSizeRect.y, _collisionOffsetAndSizeRect.w, _collisionOffsetAndSizeRect.h};
		CheckRectForCollisionWithSolids(&collisionRect);
		auto actualX = (collisionRect.x - _collisionOffsetAndSizeRect.x);
		auto actualY = (collisionRect.y - _collisionOffsetAndSizeRect.y);

		X() = roundCollisionResolve(actualX);
		Y() = roundCollisionResolve(actualY);
		if (_direction != previousDirection) {
			Engine::StartAnimatorAnimation(_animator, getAnimNameFromDirection());
		}

	} else {
		Engine::UpdateAnimatorAnimationSpeed(_animator, 0.0);
	}
	return moved;
}

constexpr const char* Player::getAnimNameFromDirection() {
	switch (_direction) {
		case Direction::North:
			return "walkU";
		case Direction::East:
			return "walkR";
		case Direction::South:
			return "walkD";
		case Direction::West:
			return "walkL";
	}
	return "walkD";	 // or assert
}
void Player::Draw() {}
