#pragma once
#include <Supergoon/Primitives/rectangle.h>

#include <bindings/SpriteAnimator.hpp>
#include <cmath>
#include <gameobject/GameObject.hpp>
#include <memory>
#include <vector>
struct TiledObject;
class Sprite;
namespace Etf {

class IInteractable;

class Player : public GameObject {
	enum class Direction { South,
						   East,
						   North,
						   West };

   public:
	static void Create(TiledObject* objData);
	~Player();

   private:
	void Start() override;
	void Update() override;
	void Draw() override;
	// Returns if player moved this frame
	bool handlePlayerMovement();
	void updateInteractionRect();
	void handleInteractions();
	bool handleMapExits();
	constexpr const char* getAnimNameFromDirection();
	// Used to fix small movements in float issues, fix for this is to just make solids not be a float, they should all be rounded to 0
	inline float roundCollisionResolve(float x, float grid = 0.01) { return std::round(x / grid) * grid; }

   private:
	static std::vector<std::unique_ptr<Player>> _players;
	Player(TiledObject* objData);
	Sprite* _sprite;
	Sprite* _InteractionSprite;
	Direction _direction = Direction::South;
	std::unique_ptr<SpriteAnimator> _animator;
	bool DoNotDestroy = false;
	int _width, _height;
	RectangleF _collisionRect = {};
	RectangleF _interactionRect = {};
	IInteractable* _currentInteractable = nullptr;
};

}  // namespace Etf
