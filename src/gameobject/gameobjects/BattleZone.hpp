#pragma once
#include <Supergoon/Primitives/rectangle.h>

#include <gameobject/GameObject.hpp>
struct TiledObject;
namespace Etf {
class BattleZone : public GameObject {
   public:
	static void Create(TiledObject* objData);
	BattleZone(TiledObject* objData);
	~BattleZone();
	inline int Zone() { return zone_; }
	inline const RectangleF& Location() { return location_; }
	inline const float& EncounterTime() { return encounterTime_; }

   private:
	int zone_;
	float encounterTime_;
	RectangleF location_;
};

}  // namespace Etf
