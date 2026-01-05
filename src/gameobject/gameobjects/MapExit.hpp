#pragma once
#include <Supergoon/Primitives/rectangle.h>

#include <gameobject/GameObject.hpp>
#include <memory>
class TiledObject;
namespace Etf {
class MapExit : public GameObject {
   public:
	static void Create(TiledObject* objData);
	// Checks for overlaps, and also handles by setting the scene to load.
	static bool CheckAndHandleMapExitOverlaps(RectangleF& rect);
	MapExit(TiledObject* objData);
	void Draw() override;

   private:
	static std::vector<std::weak_ptr<MapExit>> _mapExits;
	std::string _mapToLoad;
	int _locationToLoad;
	RectangleF _location;
};

}  // namespace Etf
