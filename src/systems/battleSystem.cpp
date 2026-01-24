#include <Supergoon/Input/keyboard.h>
#include <Supergoon/filesystem.h>
#include <Supergoon/json.h>
#include <Supergoon/log.h>

#include <battle/battlerData.hpp>
#include <format>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/BattleLocation.hpp>
#include <gameobject/gameobjects/Battler.hpp>
#include <systems/battleSystem.hpp>
#include <ui/ui.hpp>
#include <vector>

#include "bindings/engine.hpp"
#include "ui/uiObject.hpp"
#include "ui/uiText.hpp"

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
static vector<Battler*> _battlers;
static vector<vector<int>> _battleGroups;
static vector<BattlerUI*> _battlerUIs;

static void battleEnd() {
	BattleLocation::ClearAllBattleLocations();
	_battlers.clear();
}

static unsigned int _currentMenuLocation = 0;
static UIObject* enemyHPObject = nullptr;

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
	auto& p1BattlerData = _battlerData.at(0);
	auto spawnLocation = BattleLocation::GetBattleLocation(1);
	auto battlerUI = new BattlerUI(0);
	BattlerArgs args;
	args.BattlerNum = 0;
	args.BattleData = &p1BattlerData;
	args.X = spawnLocation->X();
	args.Y = spawnLocation->Y();
	args.BattleUI = battlerUI;
	auto battler = new Battler(args);
	_battlers.push_back(battler);
	_battlerUIs.push_back(battlerUI);
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
		auto battlerUI = new BattlerUI(i + 4);
		args.BattlerNum = 0;
		args.X = spawnLocation->X();
		args.Y = spawnLocation->Y();
		args.BattleUI = battlerUI;
		args.BattleData = &_battlerData.at(battlerID);
		auto battler = new Battler(args);
		_battlers.push_back(battler);
		++i;
	}
}

static void loadBattle() {
	loadBattleDB();
	loadBattleGroups();
	loadPlayers();
	loadEnemies();
}

static void handleClickAction() {
	sgLogDebug("Just pressed button num %d", _currentMenuLocation);
	switch (_currentMenuLocation) {
		case 0:
			Engine::PlaySFX("menuSelect", 1.0f);
            _battlers[0]->StartAnimation("slash2", 1);
			_battlers[1]->TakeDamage(1);
			break;

		default:
			sgLogDebug("Button not implemented", _currentMenuLocation);
			Engine::PlaySFX("error1", 1.0f);
			break;
	}
}

static void handleInput() {
	auto newLocation = _currentMenuLocation;
	if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.UP)) {
		--newLocation;
	}

	else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.DOWN)) {
		++newLocation;
	}
	if (newLocation != _currentMenuLocation) {
		_currentMenuLocation = newLocation > 3 ? _currentMenuLocation == 3 ? 0 : 3 : newLocation;
		_battlerUIs[0]->MoveCursorInMenu(_currentMenuLocation);
		Engine::PlaySFX("menuMove", 1.0f);
	}

	else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.A)) {
		handleClickAction();
	}
}

static void BattleUpdate() {
	handleInput();
}

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
