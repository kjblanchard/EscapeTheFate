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

#include "interfaces/IController.hpp"

using namespace std;
using namespace Etf;

static const int sMoveSpeed = 100;
static const RectangleF sCollisionOffsetAndSizeRect = {8, 8, 16, 22};
static const Point sInteractionEastWestWidthHeight = {26, 8};
static const Point sInteractionNorthSouthWidthHeight = {8, 26};

void LocalPlayer::CreatePlayerTwo(TiledObject* objData) {
}

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
	auto player = PlayerControllerSystem::GetPlayerByNum(0);

	auto p1 = new LocalPlayer(objData, player);
	vector<LocalPlayer*> players = {p1};
	// We should override this if we are exiting from a battle.
	for (auto i = 0; i < players.size(); ++i) {
		auto currentPlayer = players[i];
		if (GameState::Battle::ExitingFromBattle) {
			currentPlayer->SetX(GameState::NextLoadLocation.X);
			currentPlayer->SetY(GameState::NextLoadLocation.Y);
			currentPlayer->Direction_ = static_cast<Direction>(GameState::NextLoadDirection);
			GameState::NextLoadLocation = {0, 0};
		} else {
			currentPlayer->Direction_ = direction;
		}
		currentPlayer->Animator_->StartAnimation(p1->getAnimNameFromDirection());
		// We should only do this for the first player.
		if (i == 0) {
			SetCameraFollowTarget(p1->GetXHandle(), p1->GetYHandle());
			// make load location to where we are now incase we don't move and get into a battle.
			GameState::NextLoadLocation.X = currentPlayer->X();
			GameState::NextLoadLocation.Y = currentPlayer->Y();

		} else {
			currentPlayer->SetX(GameState::NextLoadLocation.X + 5);
			currentPlayer->SetY(GameState::NextLoadLocation.Y + 5);
		}
		AddGameObjectToGameObjectSystem(currentPlayer);
	}
}

LocalPlayer::~LocalPlayer() {
	DestroySprite(Sprite_);
	DestroySprite(InteractionSprite_);
}

LocalPlayer::LocalPlayer(TiledObject* objData, const shared_ptr<PlayerController>& player) : GameObject(objData->X, objData->Y), Player_(player) {
	auto spriteName = GameState::SelectedOverworldSprite + ".png";
	float fw = (float)GameState::SelectedOverworldFrameW;
	float fh = (float)GameState::SelectedOverworldFrameH;
	Sprite_ = Engine::Sprites::CreateSpriteFull(spriteName, &X_, &Y_, {0, 0, fw, fh}, {0, 0, fw, fh});
	InteractionSprite_ = Engine::Sprites::CreateSpriteFull("interaction.png", &X_, &Y_, {0, 0, 16, 16}, {20, -5, 16, 16});
	Engine::Sprites::SetSpriteVisible(InteractionSprite_, false);
	Animator_ = make_unique<SpriteAnimator>(GameState::SelectedOverworldSprite, Sprite_);
}

void LocalPlayer::Start() {}
void LocalPlayer::Update() {
	GameState::Players::LocalPlayerData[0].MovedThisFrame = false;
	if (!handlePlayerMovement()) {
		handleplayerJoystickMovement();
	}
	handleInteractions();
	if (handleMapExits()) {
		return;
	}
}

void LocalPlayer::updateInteractionRect() {
	switch (Direction_) {
		case Direction::East:
			InteractionRect_.x = CollisionRect_.x + CollisionRect_.w;
			InteractionRect_.y = CollisionRect_.y + (CollisionRect_.h / 2.0f) - (sInteractionEastWestWidthHeight.Y / 2.0f);
			InteractionRect_.w = sInteractionEastWestWidthHeight.X;
			InteractionRect_.h = sInteractionEastWestWidthHeight.Y;
			break;
		case Direction::West:
			InteractionRect_.x = CollisionRect_.x - sInteractionEastWestWidthHeight.X;
			InteractionRect_.y = CollisionRect_.y + (CollisionRect_.h / 2.0f) - (sInteractionEastWestWidthHeight.Y / 2.0f);
			InteractionRect_.w = sInteractionEastWestWidthHeight.X;
			InteractionRect_.h = sInteractionEastWestWidthHeight.Y;
			break;
		case Direction::North:
			InteractionRect_.x = CollisionRect_.x + (CollisionRect_.w / 2.0f) - (sInteractionNorthSouthWidthHeight.X / 2.0f);
			InteractionRect_.y = CollisionRect_.y - sInteractionNorthSouthWidthHeight.Y;
			InteractionRect_.w = sInteractionNorthSouthWidthHeight.X;
			InteractionRect_.h = sInteractionNorthSouthWidthHeight.Y;
			break;
		case Direction::South:
			InteractionRect_.x = CollisionRect_.x + (CollisionRect_.w / 2.0f) - (sInteractionNorthSouthWidthHeight.X / 2.0f);
			InteractionRect_.y = CollisionRect_.y + CollisionRect_.h;
			InteractionRect_.w = sInteractionNorthSouthWidthHeight.X;
			InteractionRect_.h = sInteractionNorthSouthWidthHeight.Y;
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
		Engine::Sprites::SetSpriteVisible(InteractionSprite_, true);
	} else if (!interactable && CurrentInteractable_) {
		Engine::Sprites::SetSpriteVisible(InteractionSprite_, false);
	}
	CurrentInteractable_ = interactable;
	if (CurrentInteractable_ && Player_->IsButtonJustPressed(ControllerButtons::A)) {
		// if (_currentInteractable && Controller::IsButtonJustPressed(GameButtons::A)) {
		CurrentInteractable_->Interact();
		Animator_->UpdateAnimatorSpeed(0.0);
	}
}

void LocalPlayer::handleplayerJoystickMovement() {
	if (GameState::InDialog || GameState::Battle::InBattle) return;
	auto xStick = Player_->JoystickAxisState(JoystickAxis::LeftThumbstickX);
	auto yStick = Player_->JoystickAxisState(JoystickAxis::LeftThumbstickY);
	if (fabs(xStick) < 0.1f && fabs(yStick) < 0.1f) return;

	auto previousDirection = Direction_;
	if (fabs(xStick) > fabs(yStick)) {
		Direction_ = xStick > 0.0f ? Direction::East : Direction::West;
	} else {
		Direction_ = yStick > 0.0f ? Direction::South : Direction::North;
	}
	if (Direction_ != previousDirection) {
		Animator_->StartAnimation(getAnimNameFromDirection());
		GameState::NextLoadDirection = static_cast<int>(Direction_);
	}

	float desiredX = X() + sMoveSpeed * xStick * GameState::DeltaTimeSeconds;
	float desiredY = Y() + sMoveSpeed * yStick * GameState::DeltaTimeSeconds;
	CollisionRect_ = {desiredX + sCollisionOffsetAndSizeRect.x, desiredY + sCollisionOffsetAndSizeRect.y,
					  sCollisionOffsetAndSizeRect.w, sCollisionOffsetAndSizeRect.h};
	CheckRectForCollisionWithSolids(&CollisionRect_);
	CollisionRect_.x = roundCollisionResolve(CollisionRect_.x);
	CollisionRect_.y = roundCollisionResolve(CollisionRect_.y);
	SetX(CollisionRect_.x - sCollisionOffsetAndSizeRect.x);
	SetY(CollisionRect_.y - sCollisionOffsetAndSizeRect.y);
	GameState::NextLoadLocation.X = X();
	GameState::NextLoadLocation.Y = Y();
	Animator_->UpdateAnimatorSpeed(1.0f);
	GameState::Players::LocalPlayerData[0].MovedThisFrame = true;
	GameState::Players::LocalPlayerData[0].Location.x = X();
	GameState::Players::LocalPlayerData[0].Location.y = Y();
	GameState::Players::LocalPlayerData[0].Location.w = 4;
	GameState::Players::LocalPlayerData[0].Location.h = 4;
}

bool LocalPlayer::handlePlayerMovement() {
	if (GameState::InDialog || GameState::Battle::InBattle) return false;
	auto moved = false;
	auto previousDirection = Direction_;
	auto velocityX = 0;
	auto velocityY = 0;
	if (Player_->IsButtonPressed(ControllerButtons::Up)) {
		moved = true;
		velocityY -= 1;
		Direction_ = Direction::North;
	}
	if (Player_->IsButtonPressed(ControllerButtons::Down)) {
		moved = true;
		velocityY += 1;
		Direction_ = Direction::South;
	}
	if (Player_->IsButtonPressed(ControllerButtons::Left)) {
		moved = true;
		velocityX -= 1;
		Direction_ = Direction::West;
	}
	if (Player_->IsButtonPressed(ControllerButtons::Right)) {
		moved = true;
		velocityX += 1;
		Direction_ = Direction::East;
	}

	if (Direction_ != previousDirection) {
		Animator_->StartAnimation(getAnimNameFromDirection());
		GameState::NextLoadDirection = static_cast<int>(Direction_);
	}

	if (moved) {
		float desiredX = (X() + velocityX * sMoveSpeed * GameState::DeltaTimeSeconds);
		float desiredY = (Y() + velocityY * sMoveSpeed * GameState::DeltaTimeSeconds);
		CollisionRect_ = {desiredX + sCollisionOffsetAndSizeRect.x, desiredY + sCollisionOffsetAndSizeRect.y, sCollisionOffsetAndSizeRect.w, sCollisionOffsetAndSizeRect.h};
		CheckRectForCollisionWithSolids(&CollisionRect_);
		CollisionRect_.x = roundCollisionResolve(CollisionRect_.x);
		CollisionRect_.y = roundCollisionResolve(CollisionRect_.y);
		SetX(CollisionRect_.x - sCollisionOffsetAndSizeRect.x);
		SetY(CollisionRect_.y - sCollisionOffsetAndSizeRect.y);
		// Update gamestate with players location.
		GameState::NextLoadLocation.X = X();
		GameState::NextLoadLocation.Y = Y();
		Animator_->UpdateAnimatorSpeed(1.0f);
		GameState::Players::LocalPlayerData[0].MovedThisFrame = true;
		GameState::Players::LocalPlayerData[0].Location.x = X();
		GameState::Players::LocalPlayerData[0].Location.y = Y();
		GameState::Players::LocalPlayerData[0].Location.w = 4;
		GameState::Players::LocalPlayerData[0].Location.h = 4;

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
	if (GameConfig::GetGameConfig().debug.interactions) Engine::Debug::DrawRectPrimitive(InteractionRect_);
}
