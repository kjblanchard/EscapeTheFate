#pragma once
#include <Supergoon/Primitives/rectangle.h>

#include <components/PlayerController.hpp>
#include <components/SpriteAnimator.hpp>
#include <cmath>
#include <gameobject/GameObject.hpp>
#include <memory>
#include <vector>
struct TiledObject;
struct Sprite;
namespace Etf {

class IInteractable;

class LocalPlayer : public GameObject {
	enum class Direction { South,
						   East,
						   North,
						   West };

   public:
	static void Create(TiledObject* objData);
	static void CreatePlayerTwo(TiledObject* objData);
	~LocalPlayer() override;

   private:
	void Start() override;
	void Update() override;
	void Draw() override;
	void handleplayerJoystickMovement();
	// Returns if player moved this frame
	bool handlePlayerMovement();
	void updateInteractionRect();
	void handleInteractions();
	bool handleMapExits();
	constexpr const char* getAnimNameFromDirection();
	// Used to fix small movements in float issues, fix for this is to just make solids not be a float, they should all be rounded to 0
	inline float roundCollisionResolve(float x, float grid = 0.01) { return std::round(x / grid) * grid; }

   private:
	static std::vector<std::unique_ptr<LocalPlayer>> players_;
	LocalPlayer(TiledObject* objData, const std::shared_ptr<PlayerController>& player, int playerIndex,
				const std::string& spriteName, int frameW, int frameH);
	int playerIndex_ = 0;
	Sprite* sprite_;
	Sprite* interactionSprite_;
	Direction direction_ = Direction::South;
	std::unique_ptr<SpriteAnimator> animator_;
	RectangleF collisionRect_ = {};
	RectangleF interactionRect_ = {};
	IInteractable* currentInteractable_ = nullptr;
	const std::shared_ptr<PlayerController> player_;
};

}  // namespace Etf
