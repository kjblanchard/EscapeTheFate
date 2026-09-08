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
		p1->Direction_ = static_cast<Direction>(GameState::NextLoadDirection);
	} else {
		p1->Direction_ = direction;
	}
	p1->Animator_->StartAnimation(p1->getAnimNameFromDirection());
	SetCameraFollowTarget(p1->GetXHandle(), p1->GetYHandle());
	GameState::NextLoadLocation.X = p1->X();
	GameState::NextLoadLocation.Y = p1->Y();
	GameState::NextLoadDirection = (int)p1->Direction_;
	AddGameObjectToGameObjectSystem(p1);

	if (GameState::IsMultiplayer) {
		auto player2Controller = PlayerControllerSystem::GetPlayerByNum(1);
		auto p2 = new LocalPlayer(objData, player2Controller, 1,
								  GameState::SelectedOverworldSprite2, GameState::SelectedOverworldFrameW2, GameState::SelectedOverworldFrameH2);
		if (GameState::Battle::ExitingFromBattle) {
			p2->SetX(GameState::NextLoadLocation2.X);
			p2->SetY(GameState::NextLoadLocation2.Y);
		} else {
			p2->SetX(p1->X() + 16);
			p2->SetY(p1->Y());
		}
		p2->Direction_ = p1->Direction_;
		p2->Animator_->StartAnimation(p2->getAnimNameFromDirection());
		GameState::NextLoadLocation2.X = p2->X();
		GameState::NextLoadLocation2.Y = p2->Y();
		AddGameObjectToGameObjectSystem(p2);
	}
}

LocalPlayer::~LocalPlayer() {
	DestroySprite(Sprite_);
	DestroySprite(InteractionSprite_);
}

LocalPlayer::LocalPlayer(TiledObject* objData, const shared_ptr<PlayerController>& player, int playerIndex,
						 const string& overworldSprite, int frameW, int frameH)
	: GameObject(objData->X, objData->Y), PlayerIndex_(playerIndex), Player_(player) {
	auto spriteName = overworldSprite + ".png";
	float fw = (float)frameW;
	float fh = (float)frameH;
	Sprite_ = Engine::Sprites::CreateSpriteFull(spriteName, &x, &y, {0, 0, fw, fh}, {0, 0, fw, fh});
	InteractionSprite_ = Engine::Sprites::CreateSpriteFull("interaction.png", &x, &y, {0, 0, 16, 16}, {20, -5, 16, 16});
	Engine::Sprites::SetSpriteVisible(InteractionSprite_, false);
	Animator_ = make_unique<SpriteAnimator>(overworldSprite, Sprite_);
}

void LocalPlayer::start() {}
void LocalPlayer::update() {
	GameState::Players::LocalPlayerData[PlayerIndex_].MovedThisFrame = false;
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
	if (GameState::Menu::MenuOpen[PlayerIndex_]) return;
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
	if (GameState::Menu::MenuOpen[PlayerIndex_]) return;
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
		if (PlayerIndex_ == 0) GameState::NextLoadDirection = static_cast<int>(Direction_);
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
	if (PlayerIndex_ == 0) {
		GameState::NextLoadLocation.X = X();
		GameState::NextLoadLocation.Y = Y();
	} else if (PlayerIndex_ == 1) {
		GameState::NextLoadLocation2.X = X();
		GameState::NextLoadLocation2.Y = Y();
	}
	Animator_->UpdateAnimatorSpeed(1.0f);
	GameState::Players::LocalPlayerData[PlayerIndex_].MovedThisFrame = true;
	GameState::Players::LocalPlayerData[PlayerIndex_].Location.x = X();
	GameState::Players::LocalPlayerData[PlayerIndex_].Location.y = Y();
	GameState::Players::LocalPlayerData[PlayerIndex_].Location.w = 4;
	GameState::Players::LocalPlayerData[PlayerIndex_].Location.h = 4;
}

bool LocalPlayer::handlePlayerMovement() {
	if (GameState::InDialog || GameState::Battle::InBattle) return false;
	if (GameState::Menu::MenuOpen[PlayerIndex_]) return false;
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
		if (PlayerIndex_ == 0) GameState::NextLoadDirection = static_cast<int>(Direction_);
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
		if (PlayerIndex_ == 0) {
			GameState::NextLoadLocation.X = X();
			GameState::NextLoadLocation.Y = Y();
		} else if (PlayerIndex_ == 1) {
			GameState::NextLoadLocation2.X = X();
			GameState::NextLoadLocation2.Y = Y();
		}
		Animator_->UpdateAnimatorSpeed(1.0f);
		GameState::Players::LocalPlayerData[PlayerIndex_].MovedThisFrame = true;
		GameState::Players::LocalPlayerData[PlayerIndex_].Location.x = X();
		GameState::Players::LocalPlayerData[PlayerIndex_].Location.y = Y();
		GameState::Players::LocalPlayerData[PlayerIndex_].Location.w = 4;
		GameState::Players::LocalPlayerData[PlayerIndex_].Location.h = 4;
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

void LocalPlayer::draw() {
	if (GameConfig::GetGameConfig().debug.interactions) Engine::Debug::DrawRectPrimitive(InteractionRect_);
}
