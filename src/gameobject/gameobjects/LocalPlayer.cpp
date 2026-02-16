#include <Supergoon/Input/keyboard.h>
#include <Supergoon/Primitives/Point.h>
#include <Supergoon/camera.h>
#include <Supergoon/log.h>
#include <Supergoon/map.h>

#include <bindings/Controller.hpp>
#include <bindings/engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/LocalPlayer.hpp>
#include <gameobject/gameobjects/MapExit.hpp>
#include <interfaces/IInteractable.hpp>
#include <memory>
#include <systems/GameObjectSystem.hpp>

using namespace std;
using namespace Etf;

static const int _moveSpeed = 100;
static const RectangleF _collisionOffsetAndSizeRect = {8, 8, 16, 22};
static const Point _interactionEastWestWidthHeight = {26, 8};
static const Point _interactionNorthSouthWidthHeight = {8, 26};

void LocalPlayer::Create(TiledObject* objData) {
	int loadLocation = -1;
	Direction direction = Direction::South;
	for (auto i = 0; i < objData->NumProperties; ++i) {
		auto prop = objData->Properties[i];
		// Only load player if we are on the right map start.
		if (prop.Name == string_view("loadLocation")) {
			loadLocation = prop.Data.IntData;
		} else if (prop.Name == string_view("direction")) {
			direction = static_cast<Direction>(prop.Data.IntData);
		}
	}
	if (loadLocation != GameState::NextLoadScreen) return;
	sgLogDebug("Making player start at pos %d!!", loadLocation);
	auto player = new LocalPlayer(objData);
	// We should override this if we are exiting from a battle.
	if (GameState::Battle::ExitingFromBattle) {
		player->X() = GameState::NextLoadLocation.X;
		player->Y() = GameState::NextLoadLocation.Y;
		player->_direction = static_cast<Direction>(GameState::NextLoadDirection);
		GameState::NextLoadLocation = {0, 0};
	} else {
		player->_direction = direction;
	}
	player->_animator->StartAnimation(player->getAnimNameFromDirection());
	SetCameraFollowTarget(&player->X(), &player->Y());
	// make load location to where we are now incase we don't move and get into a battle.
	GameState::NextLoadLocation.X = player->X();
	GameState::NextLoadLocation.Y = player->Y();
	AddGameObjectToGameObjectSystem(player);
}

LocalPlayer::LocalPlayer(TiledObject* objData) : GameObject(objData->X, objData->Y) {
	_sprite = Engine::CreateSpriteFull("player1", &X_, &Y_, {0, 0, 32, 32}, {0, 0, 32, 32});
	_InteractionSprite = Engine::CreateSpriteFull("interaction", &X_, &Y_, {0, 0, 16, 16}, {20, -5, 16, 16});
	Engine::SetSpriteVisible(_InteractionSprite, false);
	_animator = make_unique<SpriteAnimator>("player1", _sprite);
}

void LocalPlayer::Start() {}
void LocalPlayer::Update() {
	handlePlayerMovement();
	handleInteractions();
	if (handleMapExits()) {
		return;
	}
}

void LocalPlayer::updateInteractionRect() {
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

void LocalPlayer::handleInteractions() {
	updateInteractionRect();
	IInteractable* interactable = nullptr;
	for (auto interact : GameObjectSystem::GetGameObjectsOfType<IInteractable>()) {
		if (Engine::CheckForRectCollision(_interactionRect, interact->InteractionRect)) {
			interactable = interact;
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
		// if (_currentInteractable && Controller::IsButtonJustPressed(GameButtons::A)) {
		_currentInteractable->Interact();
		_animator->UpdateAnimatorSpeed(0.0);
	}
}

bool LocalPlayer::handlePlayerMovement() {
	if (GameState::InDialog) return false;
	auto moved = false;
	auto previousDirection = _direction;
	auto velocityX = 0;
	auto velocityY = 0;
	// if (Controller::IsButtonPressed(GameButtons::UP)) {
	if (IsKeyboardKeyDown(GameConfig::GetGameConfig().Controls.UP)) {
		moved = true;
		velocityY -= 1;
		_direction = Direction::North;
	}

	if (IsKeyboardKeyDown(GameConfig::GetGameConfig().Controls.DOWN)) {
		// if (Controller::IsButtonPressed(GameButtons::DOWN)) {
		moved = true;
		velocityY += 1;
		_direction = Direction::South;
	}

	if (IsKeyboardKeyDown(GameConfig::GetGameConfig().Controls.LEFT)) {
		// if (Controller::IsButtonPressed(GameButtons::LEFT)) {
		moved = true;
		velocityX -= 1;
		_direction = Direction::West;
	}

	if (IsKeyboardKeyDown(GameConfig::GetGameConfig().Controls.RIGHT)) {
		// if (Controller::IsButtonPressed(GameButtons::RIGHT)) {
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

bool LocalPlayer::handleMapExits() {
	return MapExit::CheckAndHandleMapExitOverlaps(_collisionRect);
}

constexpr const char* LocalPlayer::getAnimNameFromDirection() {
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

void LocalPlayer::Draw() {
	if (GameConfig::GetGameConfig().debug.interactions) Engine::DrawRectPrimitive(_interactionRect);
}
