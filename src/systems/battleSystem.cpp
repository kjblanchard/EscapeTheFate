#include <Supergoon/Input/keyboard.h>
#include <Supergoon/filesystem.h>
#include <Supergoon/json.h>
#include <Supergoon/log.h>

#include <battle/battlerData.hpp>
#include <format>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/BattleLocation.hpp>
#include <gameobject/gameobjects/EnemyBattler.hpp>
#include <gameobject/gameobjects/PlayerBattler.hpp>
#include <systems/battleSystem.hpp>
#include <ui/ui.hpp>
#include <vector>

#include "bindings/engine.hpp"

using namespace Etf;
using namespace std;

enum class BattleStates {
	NotInBattle,
	BattleStartTriggered,
	Battle,
	BattleEnd,
};
using enum BattleStates;

static BattleStates _currentBattleState = NotInBattle;
static BattleStates _nextBattleState = NotInBattle;
static vector<BattlerData> _battlerData;
// Loaded battle groups from the database, used when loading battle and stays loaded
static vector<vector<int>> _battleGroups;
// Current battlers spawned in, always the size of all positions.
static vector<Battler*> _battlers;

static void battleEnd() {
	BattleLocation::ClearAllBattleLocations();
	_battlers.clear();
}

static void loadBattleGroups() {
	if (!_battleGroups.empty()) return;
	auto loadPath = format("{}assets/battle/battleGroups.json", GetBasePath());
	auto dataRootJsonArray = jGetObjectFromFile(loadPath.c_str());
	if (!dataRootJsonArray) sgLogCritical("No battle groups found at %s, exiting", loadPath.c_str());
	auto numData = jGetObjectArrayLength(dataRootJsonArray);
	_battleGroups.reserve(numData);
	if (!numData) sgLogCritical("No battle groups found in db, exiting!");
	for (auto i = 0; i < numData; ++i) {
		auto currentBattleGroupJsonObject = jGetObjectInObjectWithIndex(dataRootJsonArray, i);
		if (!currentBattleGroupJsonObject) continue;
		auto currentJsonObjectLength = jGetObjectArrayLength(currentBattleGroupJsonObject);
		if (!currentJsonObjectLength) continue;
		vector<int> newBattleGroup;
		newBattleGroup.reserve(currentJsonObjectLength);
		for (auto j = 0; j < currentJsonObjectLength; ++j) {
			auto battlerID = jintIndex(currentBattleGroupJsonObject, j);
			newBattleGroup.push_back(battlerID);
		}
		_battleGroups.push_back(std::move(newBattleGroup));
	}
}

static void loadBattleDB() {
	if (!_battlerData.empty()) return;
	auto loadPath = format("{}assets/battle/battleDB.json", GetBasePath());
	auto dataRootJsonArray = jGetObjectFromFile(loadPath.c_str());
	if (!dataRootJsonArray) sgLogCritical("No battler Database found at %s, exiting", loadPath.c_str());
	auto numData = jGetObjectArrayLength(dataRootJsonArray);
	if (!numData) sgLogCritical("No battlers found in db, exiting!");
	for (auto i = 0; i < numData; ++i) {
		auto currentJsonObject = jGetObjectInObjectWithIndex(dataRootJsonArray, i);
		if (!currentJsonObject) continue;
		_battlerData.emplace_back();
		_battlerData.back().Name = jstr(currentJsonObject, "name");
		_battlerData.back().HP = jint(currentJsonObject, "hp");
		_battlerData.back().Str = jint(currentJsonObject, "str");
		_battlerData.back().Mag = jint(currentJsonObject, "mag");
		_battlerData.back().Def = jint(currentJsonObject, "def");
		_battlerData.back().MDef = jint(currentJsonObject, "mdef");
		_battlerData.back().Spd = jint(currentJsonObject, "spd");
		_battlerData.back().Pow = jint(currentJsonObject, "pow");
		_battlerData.back().Sprite = jstr(currentJsonObject, "sprite");
		_battlerData.back().IdleAnimation = jstr(currentJsonObject, "idle");
		_battlerData.back().Location = Engine::Json::GetRectFromObject(currentJsonObject, "rect");
	}
}

static void loadPlayers() {
	const int playerData = 0;
	const int playerSpawnLocation = 1;
	auto& p1BattlerData = _battlerData.at(playerData);
	auto spawnLocation = BattleLocation::GetBattleLocation(playerSpawnLocation);
	BattlerArgs args;
	args.BattlerNum = 0;
	args.BattleData = &p1BattlerData;
	args.X = spawnLocation->X();
	args.Y = spawnLocation->Y();
	auto battler = new PlayerBattler(args);
	_battlers.at(playerSpawnLocation) = battler;
}

static void loadEnemies() {
	auto& currentBattleGroup = _battleGroups.at(GameState::Battle::NextBattleGroup);
	int i = 0;
	for (auto battlerID : currentBattleGroup) {
		if (battlerID == 0) {
			++i;
			continue;
		}
		BattlerArgs args;
		auto spawnLocation = BattleLocation::GetBattleLocation(i + 4);
		args.BattlerNum = 0;
		args.X = spawnLocation->X();
		args.Y = spawnLocation->Y();
		args.BattleData = &_battlerData.at(battlerID);
		auto battler = new EnemyBattler(args);
		_battlers.at(i + 4) = battler;
		++i;
	}
}

static void loadBattle() {
	_battlers.resize(8);
	loadBattleDB();
	loadBattleGroups();
	loadPlayers();
	loadEnemies();
}

static void BattleUpdate() {}

// Used to reduce the boilerplate if we change states in multiple places
static void triggerStateChange() {
	switch (_nextBattleState) {
		case BattleStartTriggered:
			loadBattle();
			break;
		case BattleEnd:
			battleEnd();
		default:
			break;
	}
	_currentBattleState = _nextBattleState;
}

void BattleSystem::TriggerBattleStart() {
	if (_currentBattleState == NotInBattle && _nextBattleState != BattleStartTriggered) {
		_nextBattleState = BattleStates::BattleStartTriggered;
		GameState::Battle::InBattle = true;
	}
}

void BattleSystem::BattleSystemUpdate() {
	if (_nextBattleState != _currentBattleState) triggerStateChange();
	switch (_currentBattleState) {
		case NotInBattle:
			break;
		case BattleStartTriggered:
			_nextBattleState = Battle;
			break;
		case Battle:
			BattleUpdate();
			break;
		default:
			break;
	}
}

void BattleSystem::SendBattleDamage(int battlerNum, int damage) {
    _battlers.at(battlerNum)->TakeDamage(damage);
}
