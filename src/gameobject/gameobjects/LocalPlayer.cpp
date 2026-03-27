#include <Supergoon/Input/keyboard.h>
#include <Supergoon/Primitives/Point.h>
#include <Supergoon/camera.h>
#include <sgtools/log.h>
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
	auto player = GetPlayerByNum(0);

	auto p1 = new LocalPlayer(objData, player);
	vector<LocalPlayer*> players = {p1};
	if (GameState::Players::Player2Spawned) {
		player = GetPlayerByNum(1);
		players.emplace_back(new LocalPlayer(objData, player));
	}
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
		Engine::SetSpriteVisible(InteractionSprite_, true);
	} else if (!interactable && CurrentInteractable_) {
		Engine::SetSpriteVisible(InteractionSprite_, false);
	}
	CurrentInteractable_ = interactable;
	if (CurrentInteractable_ && Player_->GetController().IsButtonJustPressed(GameButtons::A)) {
		// if (_currentInteractable && Controller::IsButtonJustPressed(GameButtons::A)) {
		CurrentInteractable_->Interact();
		Animator_->UpdateAnimatorSpeed(0.0);
	}
}

void LocalPlayer::handleplayerJoystickMovement() {
	auto& controller = Player_->GetController();
	auto xStick = controller.JoystickAxisState(JoystickAxis::LeftThumbstickX);
	auto yStick = controller.JoystickAxisState(JoystickAxis::LeftThumbstickY);
	auto direction = Direction_;
	if (fabs(xStick) > fabs(yStick)) {
		if (xStick > 0.1f) {
			direction = Direction::East;
		} else if (xStick < -0.1f) {
			direction = Direction::West;
		}
	} else {
		if (yStick > 0.1f) {
			direction = Direction::South;
		} else if (yStick < -0.1f) {
			direction = Direction::North;
		}
	}
	// 	// Check for joystick movement
	if (xStick || yStick) {
		auto moveX = sMoveSpeed * xStick * GameState::DeltaTimeSeconds;
		auto moveY = sMoveSpeed * yStick * GameState::DeltaTimeSeconds;
		// moved = true;
	}
}

bool LocalPlayer::handlePlayerMovement() {
	if (GameState::InDialog) return false;
	auto moved = false;
	auto previousDirection = Direction_;
	auto velocityX = 0;
	auto velocityY = 0;
	auto& controller = Player_->GetController();
	if (controller.IsButtonPressed(GameButtons::UP)) {
		moved = true;
		velocityY -= 1;
		Direction_ = Direction::North;
	}
	if (controller.IsButtonPressed(GameButtons::DOWN)) {
		moved = true;
		velocityY += 1;
		Direction_ = Direction::South;
	}
	if (controller.IsButtonPressed(GameButtons::LEFT)) {
		moved = true;
		velocityX -= 1;
		Direction_ = Direction::West;
	}
	if (controller.IsButtonPressed(GameButtons::RIGHT)) {
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
