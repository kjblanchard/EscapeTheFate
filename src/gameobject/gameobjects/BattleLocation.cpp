#include <Supergoon/log.h>
#include <Supergoon/map.h>

#include <gameobject/gameobjects/BattleLocation.hpp>
#include <memory>
#include <string_view>

#include "gameobject/GameObject.hpp"
#include "systems/battleSystem.hpp"

using namespace Etf;
using namespace std;

vector<unique_ptr<BattleLocation>> _battleLocations;

void BattleLocation::Create(TiledObject* objData) {
	_battleLocations.emplace_back(make_unique<BattleLocation>(objData));
	BattleSystem::TriggerBattleStart();
}

void BattleLocation::ClearAllBattleLocations() {
	_battleLocations.clear();
}

BattleLocation::BattleLocation(TiledObject* objData) : GameObject(objData->X, objData->Y) {
	for (auto i = 0; i < objData->NumProperties; ++i) {
		auto& prop = objData->Properties[i];
		if (prop.Name == string_view("id")) {
			_id = prop.Data.IntData;
			break;
		}
	}
}

GameObject* BattleLocation::GetBattleLocation(int location) {
	for (const auto& currentLocation : _battleLocations) {
		if (location == currentLocation->_id) {
			return currentLocation.get();
		}
	}
	return nullptr;
}
