#include <Supergoon/Input/keyboard.h>
#include <Supergoon/Primitives/Point.h>
#include <Supergoon/camera.h>
#include <Supergoon/log.h>
#include <Supergoon/map.h>

#include <bindings/engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/MapExit.hpp>
#include <gameobject/gameobjects/Player.hpp>
#include <memory>

#include "gameobject/GameObject.hpp"
#include "interfaces/IInteractable.hpp"
using namespace std;
using namespace Etf;

static const int _moveSpeed = 100;
static const int UP = 26;
static const int LEFT = 4;
static const int DOWN = 22;
static const int RIGHT = 7;
// static const int A = 44;
// static const int B = 27;
static const RectangleF _collisionOffsetAndSizeRect = {8, 8, 16, 22};
static const Point _interactionEastWestWidthHeight = {26, 8};
static const Point _interactionNorthSouthWidthHeight = {8, 26};

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
	_sprite = Engine::CreateSpriteFull("player1", internalGO(), {0, 0, 32, 32}, {0, 0, 32, 32});
	_InteractionSprite = Engine::CreateSpriteFull("interaction", internalGO(), {0, 0, 16, 16}, {20, -5, 16, 16});
	Engine::SetSpriteVisible(_InteractionSprite, false);
	_animator = Engine::Animation::CreateAnimatorFull("player1", _sprite);
}
Player::~Player() {
	Engine::Animation::DestroyAnimatorFull(_animator);
}
void Player::Start() {}
void Player::Update() {
	handlePlayerMovement();
	handleInteractions();
	if (handleMapExits()) {
		return;
	}
}

void Player::updateInteractionRect() {
	switch (_direction) {
		case Direction::East:
			_interactionRect.x = _collisionRect.x + _collisionRect.w;
			_interactionRect.y = _collisionRect.y + (_collisionRect.h / 2.0f) - (_interactionEastWestWidthHeight.Y / 2.0f);
			_interactionRect.w = _interactionEastWestWidthHeight.X;
			_interactionRect.h = _interactionEastWestWidthHeight.Y;
			break;
		case Direction::West:
			_interactionRect.x = _collisionRect.x - _interactionEastWestWidthHeight.X;
			_interactionRect.y = _collisionRect.y + (_collisionRect.h / 2.0f) - (_interactionEastWestWidthHeight.Y / 2.0f);
			_interactionRect.w = _interactionEastWestWidthHeight.X;
			_interactionRect.h = _interactionEastWestWidthHeight.Y;
			break;
		case Direction::North:
			_interactionRect.x = _collisionRect.x + (_collisionRect.w / 2.0f) - (_interactionNorthSouthWidthHeight.X / 2.0f);
			_interactionRect.y = _collisionRect.y - _interactionNorthSouthWidthHeight.Y;
			_interactionRect.w = _interactionNorthSouthWidthHeight.X;
			_interactionRect.h = _interactionNorthSouthWidthHeight.Y;
			break;
		case Direction::South:
			_interactionRect.x = _collisionRect.x + (_collisionRect.w / 2.0f) - (_interactionNorthSouthWidthHeight.X / 2.0f);
			_interactionRect.y = _collisionRect.y + _collisionRect.h;
			_interactionRect.w = _interactionNorthSouthWidthHeight.X;
			_interactionRect.h = _interactionNorthSouthWidthHeight.Y;
			break;
		default:
			return;
	}
}

void Player::handleInteractions() {
	updateInteractionRect();
	IInteractable* interactable = nullptr;
	for (auto interact : GameObject::_interactables) {
		if (interact.expired()) continue;
		auto interactPtr = interact.lock().get();
		if (Engine::CheckForRectCollision(_interactionRect, interactPtr->InteractionRect)) {
			interactable = interactPtr;
			break;
		}
	}

	if (interactable && !_currentInteractable) {
		Engine::SetSpriteVisible(_InteractionSprite, true);
	} else if (!interactable && _currentInteractable) {
		Engine::SetSpriteVisible(_InteractionSprite, false);
	}
	_currentInteractable = interactable;
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
		_collisionRect = {desiredX + _collisionOffsetAndSizeRect.x, desiredY + _collisionOffsetAndSizeRect.y, _collisionOffsetAndSizeRect.w, _collisionOffsetAndSizeRect.h};
		CheckRectForCollisionWithSolids(&_collisionRect);
		auto actualX = (_collisionRect.x - _collisionOffsetAndSizeRect.x);
		auto actualY = (_collisionRect.y - _collisionOffsetAndSizeRect.y);
		X() = actualX;
		Y() = actualY;
		if (_direction != previousDirection) {
			Engine::Animation::StartAnimatorAnimation(_animator, getAnimNameFromDirection());
		}

	} else {
		Engine::Animation::UpdateAnimatorAnimationSpeed(_animator, 0.0);
	}
	return moved;
}

bool Player::handleMapExits() {
	return MapExit::CheckAndHandleMapExitOverlaps(_collisionRect);
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

void Player::Draw() {
	if (GameConfig::GetGameConfig().debug.interactions) Engine::DrawRectPrimitive(_interactionRect);
}
