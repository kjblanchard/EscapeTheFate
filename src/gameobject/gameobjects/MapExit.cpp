#include <Supergoon/log.h>
#include <Supergoon/map.h>

#include <bindings/engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/MapExit.hpp>
#include <memory>
#include <algorithm>

#include "gameobject/GameObject.hpp"
using namespace Etf;
using namespace std;

std::vector<std::weak_ptr<MapExit>> MapExit::_mapExits;

void MapExit::Create(TiledObject* objData) {
	auto newMap = make_shared<MapExit>(objData);
	_gameObjects.push_back(newMap);
	_mapExits.push_back(newMap);
	if (_mapExits.size() > 20) {
		_mapExits.erase(remove_if(_mapExits.begin(), _mapExits.end(), [](auto const& w) {
							return w.expired();
						}),
						_mapExits.end());
	}
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

bool MapExit::CheckAndHandleMapExitOverlaps(RectangleF& rect) {
	for (auto& mapExit : _mapExits) {
		if (mapExit.expired()) continue;
		auto mapPtr = mapExit.lock();

		if (Engine::CheckForRectCollision(rect, mapPtr->_location)) {
			GameState::NextLoadScreen = mapPtr->_locationToLoad;
			Engine::LoadScene(mapPtr->_mapToLoad);
			return true;
		}
	}
	return false;
}

void MapExit::Draw() {
	if (GameConfig::GetGameConfig().debug.mapExits) Engine::DrawRectPrimitive(_location);
}
