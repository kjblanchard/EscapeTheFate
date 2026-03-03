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
#include <systems/PlayerSystem.hpp>

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
	// We should assign player to this, based on what we are creating.. for now, just assign the initial player to it.
	auto player = GetPlayerByNum(0);

	auto localPlayer = new LocalPlayer(objData, player);
	// We should override this if we are exiting from a battle.
	if (GameState::Battle::ExitingFromBattle) {
		localPlayer->SetX(GameState::NextLoadLocation.X);
		localPlayer->SetY(GameState::NextLoadLocation.Y);
		localPlayer->Direction_ = static_cast<Direction>(GameState::NextLoadDirection);
		GameState::NextLoadLocation = {0, 0};
	} else {
		localPlayer->Direction_ = direction;
	}
	localPlayer->Animator_->StartAnimation(localPlayer->getAnimNameFromDirection());
	SetCameraFollowTarget(localPlayer->GetXHandle(), localPlayer->GetYHandle());
	// make load location to where we are now incase we don't move and get into a battle.
	GameState::NextLoadLocation.X = localPlayer->X();
	GameState::NextLoadLocation.Y = localPlayer->Y();
	AddGameObjectToGameObjectSystem(localPlayer);
}

LocalPlayer::LocalPlayer(TiledObject* objData, const shared_ptr<Player>& player) : GameObject(objData->X, objData->Y), Player_(player) {
	Sprite_ = Engine::CreateSpriteFull("player1", &X_, &Y_, {0, 0, 32, 32}, {0, 0, 32, 32});
	InteractionSprite_ = Engine::CreateSpriteFull("interaction", &X_, &Y_, {0, 0, 16, 16}, {20, -5, 16, 16});
	Engine::SetSpriteVisible(InteractionSprite_, false);
	Animator_ = make_unique<SpriteAnimator>("player1", Sprite_);
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
	switch (Direction_) {
		case Direction::East:
			InteractionRect_.x = CollisionRect_.x + CollisionRect_.w;
			InteractionRect_.y = CollisionRect_.y + (CollisionRect_.h / 2.0f) - (_interactionEastWestWidthHeight.Y / 2.0f);
			InteractionRect_.w = _interactionEastWestWidthHeight.X;
			InteractionRect_.h = _interactionEastWestWidthHeight.Y;
			break;
		case Direction::West:
			InteractionRect_.x = CollisionRect_.x - _interactionEastWestWidthHeight.X;
			InteractionRect_.y = CollisionRect_.y + (CollisionRect_.h / 2.0f) - (_interactionEastWestWidthHeight.Y / 2.0f);
			InteractionRect_.w = _interactionEastWestWidthHeight.X;
			InteractionRect_.h = _interactionEastWestWidthHeight.Y;
			break;
		case Direction::North:
			InteractionRect_.x = CollisionRect_.x + (CollisionRect_.w / 2.0f) - (_interactionNorthSouthWidthHeight.X / 2.0f);
			InteractionRect_.y = CollisionRect_.y - _interactionNorthSouthWidthHeight.Y;
			InteractionRect_.w = _interactionNorthSouthWidthHeight.X;
			InteractionRect_.h = _interactionNorthSouthWidthHeight.Y;
			break;
		case Direction::South:
			InteractionRect_.x = CollisionRect_.x + (CollisionRect_.w / 2.0f) - (_interactionNorthSouthWidthHeight.X / 2.0f);
			InteractionRect_.y = CollisionRect_.y + CollisionRect_.h;
			InteractionRect_.w = _interactionNorthSouthWidthHeight.X;
			InteractionRect_.h = _interactionNorthSouthWidthHeight.Y;
			break;
		default:
			return;
	}
}

void LocalPlayer::handleInteractions() {
	updateInteractionRect();
	IInteractable* interactable = nullptr;
	for (auto interact : GameObjectSystem::GetGameObjectsOfType<IInteractable>()) {
		if (Engine::CheckForRectCollision(InteractionRect_, interact->InteractionRect)) {
			interactable = interact;
			break;
		}
	}
	// Hide or show the interaction rect based off state
	if (interactable && !CurrentInteractable_) {
		Engine::SetSpriteVisible(InteractionSprite_, true);
	} else if (!interactable && CurrentInteractable_) {
		Engine::SetSpriteVisible(InteractionSprite_, false);
	}
	CurrentInteractable_ = interactable;
	if (CurrentInteractable_ && IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.A)) {
		// if (_currentInteractable && Controller::IsButtonJustPressed(GameButtons::A)) {
		CurrentInteractable_->Interact();
		Animator_->UpdateAnimatorSpeed(0.0);
	}
}

bool LocalPlayer::handlePlayerMovement() {
	if (GameState::InDialog) return false;
	auto moved = false;
	auto previousDirection = Direction_;
	auto velocityX = 0;
	auto velocityY = 0;
	// if (Player_->GetController().(GameButtons::UP)) {
	if (Player_->GetController().IsButtonPressed(GameButtons::UP)) {
		// if (IsKeyboardKeyDown(GameConfig::GetGameConfig().Controls.UP)) {
		moved = true;
		velocityY -= 1;
		Direction_ = Direction::North;
	}

	// if (IsKeyboardKeyDown(GameConfig::GetGameConfig().Controls.DOWN)) {
	if (Player_->GetController().IsButtonPressed(GameButtons::DOWN)) {
		// if (Controller::IsButtonPressed(GameButtons::DOWN)) {
		moved = true;
		velocityY += 1;
		Direction_ = Direction::South;
	}

	if (Player_->GetController().IsButtonPressed(GameButtons::LEFT)) {
		// if (IsKeyboardKeyDown(GameConfig::GetGameConfig().Controls.LEFT)) {
		// if (Controller::IsButtonPressed(GameButtons::LEFT)) {
		moved = true;
		velocityX -= 1;
		Direction_ = Direction::West;
	}

	if (Player_->GetController().IsButtonPressed(GameButtons::RIGHT)) {
		// if (IsKeyboardKeyDown(GameConfig::GetGameConfig().Controls.RIGHT)) {
		// if (Controller::IsButtonPressed(GameButtons::RIGHT)) {
		moved = true;
		velocityX += 1;
		Direction_ = Direction::East;
	}

	if (Direction_ != previousDirection) {
		Animator_->StartAnimation(getAnimNameFromDirection());
		GameState::NextLoadDirection = static_cast<int>(Direction_);
	}

	if (moved) {
		float desiredX = (X() + velocityX * _moveSpeed * GameState::DeltaTimeSeconds);
		float desiredY = (Y() + velocityY * _moveSpeed * GameState::DeltaTimeSeconds);
		CollisionRect_ = {desiredX + _collisionOffsetAndSizeRect.x, desiredY + _collisionOffsetAndSizeRect.y, _collisionOffsetAndSizeRect.w, _collisionOffsetAndSizeRect.h};
		CheckRectForCollisionWithSolids(&CollisionRect_);
		CollisionRect_.x = roundCollisionResolve(CollisionRect_.x);
		CollisionRect_.y = roundCollisionResolve(CollisionRect_.y);
		SetX(CollisionRect_.x - _collisionOffsetAndSizeRect.x);
		SetY(CollisionRect_.y - _collisionOffsetAndSizeRect.y);
		// Update gamestate with players location.
		GameState::NextLoadLocation.X = X();
		GameState::NextLoadLocation.Y = Y();
		Animator_->UpdateAnimatorSpeed(1.0f);

	} else {
		Animator_->UpdateAnimatorSpeed(0.0f);
	}
	return moved;
}

bool LocalPlayer::handleMapExits() {
	return MapExit::CheckAndHandleMapExitOverlaps(CollisionRect_);
}

constexpr const char* LocalPlayer::getAnimNameFromDirection() {
	switch (Direction_) {
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
	if (GameConfig::GetGameConfig().debug.interactions) Engine::DrawRectPrimitive(InteractionRect_);
}
