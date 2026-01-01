#pragma once
#include <gameobject/GameObject.hpp>
#include <memory>
#include <vector>
class TiledObject;
namespace Etf {

class Player : public GameObject {
   public:
	static void Create(TiledObject* objData);
	static void UpdatePlayers();
	static void DrawPlayers();
	~Player();

   protected:
	Player(TiledObject* objData);

   private:
	static std::vector<std::unique_ptr<Player>> _players;
	void Start() override;
	void Update() override;
	void Draw() override;
	bool DoNotDestroy = false;
	int _width, _height;
};

}  // namespace Etf
