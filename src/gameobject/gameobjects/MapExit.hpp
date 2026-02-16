#pragma once
#include <Supergoon/Primitives/rectangle.h>

#include <gameobject/GameObject.hpp>
#include <memory>
#include <string>
#include <vector>
struct TiledObject;
namespace Etf {
class MapExit : public GameObject {
   public:
	static void Create(TiledObject* objData);
	// Checks for overlaps, and also handles by setting the scene to load.
	static bool CheckAndHandleMapExitOverlaps(RectangleF& rect);
	MapExit(TiledObject* objData);
	~MapExit();
	void Draw() override;

   private:
	static std::vector<MapExit*> _mapExits;
	std::string _mapToLoad;
	int _locationToLoad;
	RectangleF _location;
};

}  // namespace Etf
