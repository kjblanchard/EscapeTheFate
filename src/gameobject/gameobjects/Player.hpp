#pragma once
#include <Supergoon/Primitives/rectangle.h>

#include <gameobject/GameObject.hpp>
#include <memory>
#include <vector>
class TiledObject;
class Sprite;
namespace Etf {

class Player : public GameObject {
	enum class Direction { North,
						   East,
						   South,
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
	constexpr const char* getAnimNameFromDirection();
	//Used to fix small movements in float issues
	inline float roundCollisionResolve(float x, float grid = 0.01f) { return std::round(x / grid) * grid; }

   private:
	static std::vector<std::unique_ptr<Player>> _players;
	Player(TiledObject* objData);
	Sprite* _sprite;
	Direction _direction = Direction::South;
	unsigned int _animator;
	bool DoNotDestroy = false;
	int _width, _height;
};

}  // namespace Etf
