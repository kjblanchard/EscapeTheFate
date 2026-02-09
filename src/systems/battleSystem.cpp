#include <Supergoon/Input/keyboard.h>
#include <Supergoon/camera.h>
#include <Supergoon/engine.h>
#include <Supergoon/filesystem.h>
#include <Supergoon/json.h>
#include <Supergoon/log.h>
#include <assert.h>

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
	NotInitialized,
	NotInBattle,
	BattleStartTriggered,
	Battle,
	BattleVictory,
	BattleEnd,
};
using enum BattleStates;

static bool _initialized = false;

static BattleStates _currentBattleState = NotInBattle;
static BattleStates _nextBattleState = NotInBattle;
static vector<BattlerData> _battlerData;
// Loaded battle groups from the database, used when loading battle and stays loaded
static vector<vector<int>> _battleGroups;
// Current battlers spawned in, always the size of all positions.
static vector<Battler*> _battlers;
static string _loadMap = "";

// Holds all of the UI objects in a (organized?) place.
struct BattleUI {
	// Top level, hide or show the whole thing
	UIObject* RootPanel = nullptr;
	// Player hud
	UIObject* PlayerHUD = nullptr;
	// All the player commands menus, usually controlled by the battler.
	UIObject* PlayerCommandsObjects[3] = {nullptr, nullptr, nullptr};
	UIObject* VictoryPanel = nullptr;
} static _battleUI;

static void battleEnd() {
	_battleUI.VictoryPanel->SetVisible(false);
	BattleLocation::ClearAllBattleLocations();
	_battlers.clear();
	ResetCameraFollow();
	Engine::LoadScene(_loadMap, 0.75f, 0.25f, false);
	_nextBattleState = NotInBattle;
	GameState::Battle::InBattle = false;
	GameState::Battle::ExitingFromBattle = true;
}

static void loadBattleGroups() {
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
	jReleaseObjectFromFile(dataRootJsonArray);
}

static void loadBattleDB() {
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
	jReleaseObjectFromFile(dataRootJsonArray);
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

static void cacheBattleUIElements() {
	// Need to find the command menu, HUD, etc.
	_battleUI.RootPanel = UI::GetRootUIObject()->GetChildByName("BattlePanel");
	assert(_battleUI.RootPanel && "No root object found");
	_battleUI.PlayerHUD = UI::GetRootUIObject()->GetChildByName("PlayerStatusHUD");
	assert(_battleUI.PlayerHUD && "No hud object found");
	int battlerNum = 1;
	for (auto& obj : _battleUI.PlayerCommandsObjects) {
		auto nameLookup = format("Player{}CommandsUI", to_string(battlerNum));
		obj = _battleUI.RootPanel->GetChildByName(nameLookup);
		++battlerNum;
		assert(obj && "No command object found");
	}
	_battleUI.VictoryPanel = _battleUI.RootPanel->GetChildByName("VictoryPanel");
	assert(_battleUI.VictoryPanel && "No victory panel found");
}

static void initializeBattleSystem() {
	loadBattleDB();
	loadBattleGroups();
	cacheBattleUIElements();
	_initialized = true;
	_battleUI.RootPanel->SetVisible(false);
	_battlers.clear();
	_nextBattleState = NotInBattle;
}

static void loadBattle() {
	IsGameLoading = true;
	if (!_initialized) initializeBattleSystem();
	_battlers.resize(8);
	sgLogDebug("loading battle");
	//Something is terrible with load players.
	loadPlayers();
	loadEnemies();
	_battleUI.RootPanel->SetVisible(true);
	_battleUI.PlayerHUD->SetVisible(true);
	_battleUI.VictoryPanel->SetVisible(false);
}

static void battleVictory() {
	_battleUI.VictoryPanel->SetVisible(true);
}

static void BattleUpdate() {}

// Used to reduce the boilerplate if we change states in multiple places
static void triggerStateChange() {
	switch (_nextBattleState) {
		case BattleStartTriggered:
			loadBattle();
			break;
		case BattleVictory:
			battleVictory();
			break;
		case BattleEnd:
			battleEnd();
			break;
		default:
			break;
	}
	_currentBattleState = _nextBattleState;
}

void BattleSystem::TriggerBattleStart() {
	if ((_currentBattleState == NotInBattle) && _nextBattleState != BattleStartTriggered) {
		_loadMap = Engine::CurrentScene();
		_nextBattleState = BattleStates::BattleStartTriggered;
		GameState::Battle::InBattle = true;
	}
}
void BattleSystem::TriggerBattleEnd() {
	_nextBattleState = BattleEnd;
	triggerStateChange();
}
void BattleSystem::TriggerBattleVictoryStart() {
	_nextBattleState = BattleVictory;
	triggerStateChange();
}

void BattleSystem::BattleSystemUpdate() {
	if (_nextBattleState != _currentBattleState) triggerStateChange();
	switch (_currentBattleState) {
		case NotInBattle:
			break;
		case BattleStartTriggered:
			_nextBattleState = Battle;
			IsGameLoading = false;
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

void BattleSystem::InitializeBattleSystem() {
	initializeBattleSystem();
}

const std::vector<Battler*>& BattleSystem::GetEnemyBattlers() {
	return _battlers;
}
