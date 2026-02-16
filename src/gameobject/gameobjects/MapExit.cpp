#include <Supergoon/log.h>
#include <Supergoon/map.h>

#include <algorithm>
#include <bindings/engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/MapExit.hpp>

#include "gameobject/GameObject.hpp"
using namespace Etf;
using namespace std;

const float MAP_FADEOUT_TIME = 0.25f;
const float MAP_FADEIN_TIME = 0.35f;
std::vector<MapExit*> MapExit::_mapExits;

void MapExit::Create(TiledObject* objData) {
	auto newMap = new MapExit(objData);
	AddGameObjectToGameObjectSystem(newMap);
	_mapExits.push_back(newMap);
}

MapExit::MapExit(TiledObject* objData) : GameObject(objData->X, objData->Y) {
	for (auto i = 0; i < objData->NumProperties; ++i) {
		auto prop = objData->Properties[i];
		if (prop.Name == string("loadLocation")) {
			_locationToLoad = prop.Data.IntData;
		} else if (prop.Name == string("loadMap")) {
			_mapToLoad = prop.Data.StringData;
		}
	}
	_location.x = X();
	_location.y = Y();
	_location.w = objData->Width;
	_location.h = objData->Height;
}

MapExit::~MapExit() {
	_mapExits.erase(std::find_if(_mapExits.begin(), _mapExits.end(), [this](MapExit* item) {
		return item == this;
	}));
}

bool MapExit::CheckAndHandleMapExitOverlaps(RectangleF& rect) {
	for (auto mapExit : _mapExits) {
		if (Engine::CheckForRectCollision(rect, mapExit->_location)) {
			GameState::NextLoadScreen = mapExit->_locationToLoad;
			Engine::LoadScene(mapExit->_mapToLoad, MAP_FADEOUT_TIME, MAP_FADEIN_TIME);
			return true;
		}
	}
	return false;
}

void MapExit::Draw() {
	if (GameConfig::GetGameConfig().debug.mapExits) Engine::DrawRectPrimitive(_location);
}
