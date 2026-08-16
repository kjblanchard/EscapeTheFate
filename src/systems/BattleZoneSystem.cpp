#include <Supergoon/json.h>
#include <sgtools/log.h>

#include <battle/battleZones.hpp>
#include <engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/BattleZone.hpp>
#include <systems/BattleTransitionSystem.hpp>
#include <systems/BattleZoneSystem.hpp>
#include <systems/NetworkSystem.hpp>
#include <vector>

using namespace Etf;
using namespace std;

constexpr int maxNumLocalPlayers_ = 2;
vector<BattleZone*> battleZones_;
vector<BattleZoneData> battleZoneDatabase_;

static void loadBattleZones() {
	auto dataRootJsonArray = Engine::Json::GetJsonObjectFromDirectory("battleZones");
	if (!dataRootJsonArray) sgLogCritical("No battle zones found exiting");
	auto numData = jGetObjectArrayLength(dataRootJsonArray);
	if (!numData) sgLogCritical("No zones found in zonedb, exiting!");
	for (auto i = 0; i < numData; ++i) {
		auto currentJsonObject = jGetObjectInObjectWithIndex(dataRootJsonArray, i);
		if (!currentJsonObject) continue;
		battleZoneDatabase_.emplace_back();
		battleZoneDatabase_.back().ID = jint(currentJsonObject, "id");
		battleZoneDatabase_.back().EncounterTime = jfloat(currentJsonObject, "encounterTime");
		auto groupsObj = jobj(currentJsonObject, "groups");
		auto numGroups = jGetObjectArrayLength(groupsObj);
		for (auto j = 0; j < numGroups; ++j) {
			auto groupNum = jintIndex(groupsObj, j);
			battleZoneDatabase_.back().BattleGroups.push_back(groupNum);
		}
		auto mapsObj = jobj(currentJsonObject, "maps");
		auto numMaps = jGetObjectArrayLength(mapsObj);
		for (auto j = 0; j < numMaps; ++j) {
			auto mapName = jstrIndex(mapsObj, j);
			battleZoneDatabase_.back().Maps.push_back(mapName);
		}
	}
	jReleaseObjectFromFile(dataRootJsonArray);
}

const string& getBattleSceneRandom(const BattleZoneData& zone) {
	if (zone.Maps.empty()) {
		sgLogCritical("Zone has no maps, cannot load scene!");
	}
	return zone.Maps[rand() % zone.Maps.size()];
}

int getBattleGroupRandom(const BattleZoneData& zone) {
	if (zone.Maps.empty()) {
		sgLogCritical("Zone has no groups, cannot select battlers!");
	}
	return zone.BattleGroups[rand() % zone.BattleGroups.size()];
}

void BattleZoneSystem::Start() {
	loadBattleZones();
}

void BattleZoneSystem::Update() {
	if (BattleTransitionSystem::IsTransitioning()) return;
	// If a player is moving within a battle zone, increase walk time
	for (auto zone : battleZones_) {
		for (auto i = 0; i < maxNumLocalPlayers_; ++i) {
			auto& player = GameState::Players::LocalPlayerData[i];
			if (player.MovedThisFrame && Engine::CheckForRectCollision(zone->Location(), player.Location)) {
				GameState::Battle::CurrentStepsWithoutBattle += GameState::DeltaTimeSeconds;
				// If walktime is more than the current zones battle encounter time, then we should start the battle
				if (GameState::Battle::CurrentStepsWithoutBattle >= zone->EncounterTime()) {
					GameState::NextLoadMapName = Engine::CurrentSceneName();
					auto& battleZoneData = GetBattleZoneDataFromDB(zone->Zone());
					auto battleGroup = getBattleGroupRandom(battleZoneData);
					sgLogDebug("Setting battle group to be %d", battleGroup);
					GameState::Battle::NextBattleGroup = battleGroup;
					auto& nextbattleScene = getBattleSceneRandom(battleZoneData);
					sgLogDebug("Setting battle scene to be %s", nextbattleScene.c_str());
					if (GameState::IsOnline) {
						NetworkSystem::SendBattleStart(static_cast<uint8_t>(battleGroup), nextbattleScene.c_str());
					}
					BattleTransitionSystem::TriggerTransition(nextbattleScene);
					GameState::Battle::CurrentStepsWithoutBattle = 0;
				}
			}
		}
	}
}

void BattleZoneSystem::Draw() {
	if (!GameConfig::GetGameConfig().debug.battleZones) return;
	for (auto zone : battleZones_) {
		if (!zone) continue;
		Engine::Debug::DrawRectPrimitive(zone->Location());
	}
}

void BattleZoneSystem::Shutdown() {}

void BattleZoneSystem::DestroyBattleZoneGameObject(const BattleZone* zone) {
	battleZones_.erase(std::find_if(battleZones_.begin(), battleZones_.end(), [zone](BattleZone* item) {
		return item == zone;
	}));
}

const BattleZoneData& BattleZoneSystem::GetBattleZoneDataFromDB(int id) {
	if (id >= battleZoneDatabase_.size()) {
		sgLogWarn("Trying to get battle zone id %d from battle zone db with db size of %d", id, battleZoneDatabase_.size());
		return battleZoneDatabase_.at(0);
	}
	return battleZoneDatabase_.at(id);
}

void BattleZoneSystem::AddBattleZone(BattleZone* zone) {
	battleZones_.push_back(zone);
}
