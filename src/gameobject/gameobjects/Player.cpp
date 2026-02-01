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
		// We should override this if we are exiting from a battle.
		if (GameState::Battle::ExitingFromBattle) {
			player->X() = GameState::NextLoadLocation.X;
			player->Y() = GameState::NextLoadLocation.Y;
			player->_direction = static_cast<Direction>(GameState::NextLoadDirection);
			player->_animator->StartAnimation(player->getAnimNameFromDirection());
			GameState::NextLoadLocation = {0, 0};
		}
		SetCameraFollowTarget(&player->X(), &player->Y());
		_gameObjects.push_back(unique_ptr<GameObject>(player));
	}
}

Player::Player(TiledObject* objData) : GameObject(objData->X, objData->Y) {
	_sprite = Engine::CreateSpriteFull("player1", &_x, &_y, {0, 0, 32, 32}, {0, 0, 32, 32});
	_InteractionSprite = Engine::CreateSpriteFull("interaction", &_x, &_y, {0, 0, 16, 16}, {20, -5, 16, 16});
	Engine::SetSpriteVisible(_InteractionSprite, false);
	_animator = make_unique<SpriteAnimator>("player1", _sprite);
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
	// Hide or show the interaction rect based off state
	if (interactable && !_currentInteractable) {
		Engine::SetSpriteVisible(_InteractionSprite, true);
	} else if (!interactable && _currentInteractable) {
		Engine::SetSpriteVisible(_InteractionSprite, false);
	}
	_currentInteractable = interactable;
	if (_currentInteractable && IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.A)) {
		_currentInteractable->Interact();
		_animator->UpdateAnimatorSpeed(0.0);
	}
}

bool Player::handlePlayerMovement() {
	if (GameState::InDialog) return false;
	auto moved = false;
	auto previousDirection = _direction;
	auto velocityX = 0;
	auto velocityY = 0;
	if (IsKeyboardKeyDown(GameConfig::GetGameConfig().Controls.UP)) {
		moved = true;
		velocityY -= 1;
		_direction = Direction::North;
	}

	if (IsKeyboardKeyDown(GameConfig::GetGameConfig().Controls.DOWN)) {
		moved = true;
		velocityY += 1;
		_direction = Direction::South;
	}

	if (IsKeyboardKeyDown(GameConfig::GetGameConfig().Controls.LEFT)) {
		moved = true;
		velocityX -= 1;
		_direction = Direction::West;
	}

	if (IsKeyboardKeyDown(GameConfig::GetGameConfig().Controls.RIGHT)) {
		moved = true;
		velocityX += 1;
		_direction = Direction::East;
	}

	if (_direction != previousDirection) {
		_animator->StartAnimation(getAnimNameFromDirection());
		GameState::NextLoadDirection = static_cast<int>(_direction);
	}

	if (moved) {
		float desiredX = (X() + velocityX * _moveSpeed * GameState::DeltaTimeSeconds);
		float desiredY = (Y() + velocityY * _moveSpeed * GameState::DeltaTimeSeconds);
		_collisionRect = {desiredX + _collisionOffsetAndSizeRect.x, desiredY + _collisionOffsetAndSizeRect.y, _collisionOffsetAndSizeRect.w, _collisionOffsetAndSizeRect.h};
		CheckRectForCollisionWithSolids(&_collisionRect);
		_collisionRect.x = roundCollisionResolve(_collisionRect.x);
		_collisionRect.y = roundCollisionResolve(_collisionRect.y);
		auto actualX = (_collisionRect.x - _collisionOffsetAndSizeRect.x);
		auto actualY = (_collisionRect.y - _collisionOffsetAndSizeRect.y);
		X() = actualX;
		Y() = actualY;
		// Update gamestate with players location.
		GameState::NextLoadLocation.X = X();
		GameState::NextLoadLocation.Y = Y();
		_animator->UpdateAnimatorSpeed(1.0f);

	} else {
		_animator->UpdateAnimatorSpeed(0.0f);
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
