#include "gameobject/gameobjects/BattleZone.hpp"

#include <Supergoon/map.h>
#include <sgtools/log.h>

#include <cstdlib>
#include <engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/MapExit.hpp>

#include "gameobject/GameObject.hpp"
#include "systems/BattleZoneSystem.hpp"
using namespace Etf;
using namespace std;

void BattleZone::Create(TiledObject* objData) {
	auto newZone = new BattleZone(objData);
	AddGameObjectToGameObjectSystem(newZone);
}

BattleZone::BattleZone(TiledObject* objData) : GameObject(objData->X, objData->Y) {
	for (auto i = 0; i < objData->NumProperties; ++i) {
		auto prop = objData->Properties[i];
		if (prop.Name == string("id")) {
			zone_ = prop.Data.IntData;
		}
	}
	location_.x = X();
	location_.y = Y();
	location_.w = objData->Width;
	location_.h = objData->Height;
	auto& data = BattleZoneSystem::GetBattleZoneData(zone_);
	encounterTime_ = data.EncounterTime * (0.5f + (float)rand() / RAND_MAX);  // between .5 and 1.5 for the time.
	BattleZoneSystem::AddBattleZone(this);
}

BattleZone::~BattleZone() {
	BattleZoneSystem::DestroyBattleZoneGameObject(this);
}
