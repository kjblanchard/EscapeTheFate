#pragma once
#include <Supergoon/Primitives/rectangle.h>
#include <Supergoon/map.h>

#include <gameobject/GameObject.hpp>

namespace Etf {

class BattleLocation : public GameObject {
   public:
	static void Create(TiledObject* objData);
	static void ClearAllBattleLocations();
    static GameObject* GetBattleLocation(int location);
	BattleLocation(TiledObject* objData);

   private:
	unsigned int _id = 0;
};

}  // namespace Etf
