#include <Supergoon/Primitives/Point.h>
#include <Supergoon/camera.h>
#include <Supergoon/map.h>
#include <Supergoon/sprite.h>
#include <sgtools/log.h>

#include <cmath>
#include <components/PlayerController.hpp>
#include <engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/LocalPlayer.hpp>
#include <gameobject/gameobjects/MapExit.hpp>
#include <interfaces/IInteractable.hpp>
#include <memory>
#include <systems/GameObjectSystem.hpp>
#include <systems/PlayerControllerSystem.hpp>

#include <interfaces/IController.hpp>

using namespace std;
using namespace Etf;

namespace {
constexpr int kMoveSpeed = 100;
constexpr RectangleF kCollisionOffsetAndSize = {8, 8, 16, 22};
constexpr Point kInteractionEastWestSize = {26, 8};
constexpr Point kInteractionNorthSouthSize = {8, 26};
}  // namespace

void LocalPlayer::CreatePlayerTwo(TiledObject* objData) {
}

void LocalPlayer::Create(TiledObject* objData) {
	int loadLocation = -1;
	Direction direction = Direction::South;
	for (auto i = 0; i < objData->NumProperties; ++i) {
		auto prop = objData->Properties[i];
		if (prop.Name == string_view("loadLocation")) {
			loadLocation = prop.Data.IntData;
		} else if (prop.Name == string_view("direction")) {
			direction = static_cast<Direction>(prop.Data.IntData);
		}
	}
	if (loadLocation != GameState::NextLoadScreen) return;
	sgLogDebug("Making player start at pos %d!!", loadLocation);

	auto player1Controller = PlayerControllerSystem::GetPlayerByNum(0);
	auto p1 = new LocalPlayer(objData, player1Controller, 0,
							  GameState::SelectedOverworldSprite, GameState::SelectedOverworldFrameW, GameState::SelectedOverworldFrameH);

	if (GameState::Battle::ExitingFromBattle) {
		p1->SetX(GameState::NextLoadLocation.X);
		p1->SetY(GameState::NextLoadLocation.Y);
		p1->direction_ = static_cast<Direction>(GameState::NextLoadDirection);
	} else {
		p1->direction_ = direction;
	}
	p1->animator_->StartAnimation(p1->getAnimNameFromDirection());
	SetCameraFollowTarget(p1->GetXHandle(), p1->GetYHandle());
	GameState::NextLoadLocation.X = p1->X();
	GameState::NextLoadLocation.Y = p1->Y();
	AddGameObjectToGameObjectSystem(p1);

	if (GameState::IsMultiplayer) {
		auto player2Controller = PlayerControllerSystem::GetPlayerByNum(1);
		auto p2 = new LocalPlayer(objData, player2Controller, 1,
								  GameState::SelectedOverworldSprite2, GameState::SelectedOverworldFrameW2, GameState::SelectedOverworldFrameH2);
		p2->SetX(p1->X() + 16);
		p2->SetY(p1->Y());
		p2->direction_ = p1->direction_;
		p2->animator_->StartAnimation(p2->getAnimNameFromDirection());
		AddGameObjectToGameObjectSystem(p2);
	}

	if (GameState::Battle::ExitingFromBattle) {
		GameState::NextLoadLocation = {0, 0};
	}
}

LocalPlayer::~LocalPlayer() {
	DestroySprite(sprite_);
	DestroySprite(interactionSprite_);
}

LocalPlayer::LocalPlayer(TiledObject* objData, const shared_ptr<PlayerController>& player, int playerIndex,
						   const string& overworldSprite, int frameW, int frameH)
	: GameObject(objData->X, objData->Y), playerIndex_(playerIndex), player_(player) {
	auto spriteName = overworldSprite + ".png";
	float fw = (float)frameW;
	float fh = (float)frameH;
	sprite_ = Engine::Sprites::CreateSpriteFull(spriteName, &X_, &Y_, {0, 0, fw, fh}, {0, 0, fw, fh});
	interactionSprite_ = Engine::Sprites::CreateSpriteFull("interaction.png", &X_, &Y_, {0, 0, 16, 16}, {20, -5, 16, 16});
	Engine::Sprites::SetSpriteVisible(interactionSprite_, false);
	animator_ = make_unique<SpriteAnimator>(overworldSprite, sprite_);
}

void LocalPlayer::Start() {}
void LocalPlayer::Update() {
	GameState::Players::LocalPlayerData[playerIndex_].MovedThisFrame = false;
	if (!handlePlayerMovement()) {
		handleplayerJoystickMovement();
	}
	handleInteractions();
	if (handleMapExits()) {
		return;
	}
}

void LocalPlayer::updateInteractionRect() {
	switch (direction_) {
		case Direction::East:
			interactionRect_.x = collisionRect_.x + collisionRect_.w;
			interactionRect_.y = collisionRect_.y + (collisionRect_.h / 2.0f) - (kInteractionEastWestSize.Y / 2.0f);
			interactionRect_.w = kInteractionEastWestSize.X;
			interactionRect_.h = kInteractionEastWestSize.Y;
			break;
		case Direction::West:
			interactionRect_.x = collisionRect_.x - kInteractionEastWestSize.X;
			interactionRect_.y = collisionRect_.y + (collisionRect_.h / 2.0f) - (kInteractionEastWestSize.Y / 2.0f);
			interactionRect_.w = kInteractionEastWestSize.X;
			interactionRect_.h = kInteractionEastWestSize.Y;
			break;
		case Direction::North:
			interactionRect_.x = collisionRect_.x + (collisionRect_.w / 2.0f) - (kInteractionNorthSouthSize.X / 2.0f);
			interactionRect_.y = collisionRect_.y - kInteractionNorthSouthSize.Y;
			interactionRect_.w = kInteractionNorthSouthSize.X;
			interactionRect_.h = kInteractionNorthSouthSize.Y;
			break;
		case Direction::South:
			interactionRect_.x = collisionRect_.x + (collisionRect_.w / 2.0f) - (kInteractionNorthSouthSize.X / 2.0f);
			interactionRect_.y = collisionRect_.y + collisionRect_.h;
			interactionRect_.w = kInteractionNorthSouthSize.X;
			interactionRect_.h = kInteractionNorthSouthSize.Y;
			break;
		default:
			return;
	}
}

void LocalPlayer::handleInteractions() {
	updateInteractionRect();
	IInteractable* interactable = nullptr;
	for (auto interact : GameObjectSystem::GetGameObjectsOfType<IInteractable>()) {
		if (Engine::CheckForRectCollision(interactionRect_, interact->InteractionRect)) {
			interactable = interact;
			break;
		}
	}
	// Hide or show the interaction rect based off state
	if (interactable && !currentInteractable_) {
		Engine::Sprites::SetSpriteVisible(interactionSprite_, true);
	} else if (!interactable && currentInteractable_) {
		Engine::Sprites::SetSpriteVisible(interactionSprite_, false);
	}
	currentInteractable_ = interactable;
	if (currentInteractable_ && player_->IsButtonJustPressed(ControllerButtons::A)) {
		// if (_currentInteractable && Controller::IsButtonJustPressed(GameButtons::A)) {
		currentInteractable_->Interact();
		animator_->UpdateAnimatorSpeed(0.0);
	}
}

void LocalPlayer::handleplayerJoystickMovement() {
	if (GameState::InDialog || GameState::Battle::InBattle) return;
	auto xStick = player_->JoystickAxisState(JoystickAxis::LeftThumbstickX);
	auto yStick = player_->JoystickAxisState(JoystickAxis::LeftThumbstickY);
	if (fabs(xStick) < 0.1f && fabs(yStick) < 0.1f) return;

	auto previousDirection = direction_;
	if (fabs(xStick) > fabs(yStick)) {
		direction_ = xStick > 0.0f ? Direction::East : Direction::West;
	} else {
		direction_ = yStick > 0.0f ? Direction::South : Direction::North;
	}
	if (direction_ != previousDirection) {
		animator_->StartAnimation(getAnimNameFromDirection());
		if (playerIndex_ == 0) GameState::NextLoadDirection = static_cast<int>(direction_);
	}

	float desiredX = X() + kMoveSpeed * xStick * GameState::DeltaTimeSeconds;
	float desiredY = Y() + kMoveSpeed * yStick * GameState::DeltaTimeSeconds;
	collisionRect_ = {desiredX + kCollisionOffsetAndSize.x, desiredY + kCollisionOffsetAndSize.y,
					  kCollisionOffsetAndSize.w, kCollisionOffsetAndSize.h};
	CheckRectForCollisionWithSolids(&collisionRect_);
	collisionRect_.x = roundCollisionResolve(collisionRect_.x);
	collisionRect_.y = roundCollisionResolve(collisionRect_.y);
	SetX(collisionRect_.x - kCollisionOffsetAndSize.x);
	SetY(collisionRect_.y - kCollisionOffsetAndSize.y);
	if (playerIndex_ == 0) {
		GameState::NextLoadLocation.X = X();
		GameState::NextLoadLocation.Y = Y();
	}
	animator_->UpdateAnimatorSpeed(1.0f);
	GameState::Players::LocalPlayerData[playerIndex_].MovedThisFrame = true;
	GameState::Players::LocalPlayerData[playerIndex_].Location.x = X();
	GameState::Players::LocalPlayerData[playerIndex_].Location.y = Y();
	GameState::Players::LocalPlayerData[playerIndex_].Location.w = 4;
	GameState::Players::LocalPlayerData[playerIndex_].Location.h = 4;
}

bool LocalPlayer::handlePlayerMovement() {
	if (GameState::InDialog || GameState::Battle::InBattle) return false;
	auto moved = false;
	auto previousDirection = direction_;
	auto velocityX = 0;
	auto velocityY = 0;
	if (player_->IsButtonPressed(ControllerButtons::Up)) {
		moved = true;
		velocityY -= 1;
		direction_ = Direction::North;
	}
	if (player_->IsButtonPressed(ControllerButtons::Down)) {
		moved = true;
		velocityY += 1;
		direction_ = Direction::South;
	}
	if (player_->IsButtonPressed(ControllerButtons::Left)) {
		moved = true;
		velocityX -= 1;
		direction_ = Direction::West;
	}
	if (player_->IsButtonPressed(ControllerButtons::Right)) {
		moved = true;
		velocityX += 1;
		direction_ = Direction::East;
	}

	if (direction_ != previousDirection) {
		animator_->StartAnimation(getAnimNameFromDirection());
		if (playerIndex_ == 0) GameState::NextLoadDirection = static_cast<int>(direction_);
	}

	if (moved) {
		float desiredX = (X() + velocityX * kMoveSpeed * GameState::DeltaTimeSeconds);
		float desiredY = (Y() + velocityY * kMoveSpeed * GameState::DeltaTimeSeconds);
		collisionRect_ = {desiredX + kCollisionOffsetAndSize.x, desiredY + kCollisionOffsetAndSize.y, kCollisionOffsetAndSize.w, kCollisionOffsetAndSize.h};
		CheckRectForCollisionWithSolids(&collisionRect_);
		collisionRect_.x = roundCollisionResolve(collisionRect_.x);
		collisionRect_.y = roundCollisionResolve(collisionRect_.y);
		SetX(collisionRect_.x - kCollisionOffsetAndSize.x);
		SetY(collisionRect_.y - kCollisionOffsetAndSize.y);
		if (playerIndex_ == 0) {
			GameState::NextLoadLocation.X = X();
			GameState::NextLoadLocation.Y = Y();
		}
		animator_->UpdateAnimatorSpeed(1.0f);
		GameState::Players::LocalPlayerData[playerIndex_].MovedThisFrame = true;
		GameState::Players::LocalPlayerData[playerIndex_].Location.x = X();
		GameState::Players::LocalPlayerData[playerIndex_].Location.y = Y();
		GameState::Players::LocalPlayerData[playerIndex_].Location.w = 4;
		GameState::Players::LocalPlayerData[playerIndex_].Location.h = 4;
	} else {
		animator_->UpdateAnimatorSpeed(0.0f);
	}
	return moved;
}

bool LocalPlayer::handleMapExits() {
	return MapExit::CheckAndHandleMapExitOverlaps(collisionRect_);
}

constexpr const char* LocalPlayer::getAnimNameFromDirection() {
	switch (direction_) {
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
	if (GameConfig::GetGameConfig().debug.interactions) Engine::Debug::DrawRectPrimitive(interactionRect_);
}
