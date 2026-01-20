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
#include "gameobject/GameObject.hpp"
#include "ui/uiObject.hpp"

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

static void battleEnd() {
	BattleLocation::ClearAllBattleLocations();
	_battlers.clear();
}

static unsigned int _currentMenuLocation = 0;
static UIObject* menuItems[] = {nullptr, nullptr, nullptr, nullptr};
static UIObject* finger = nullptr;

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

static void loadBattlers() {
	auto& p1BattlerData = _battlerData.at(0);
	auto spawnLocation = BattleLocation::GetBattleLocation(1);
	auto battler = new Battler(&p1BattlerData, spawnLocation->X(), spawnLocation->Y());
	_battlers.push_back(battler);
}

static void loadEnemies() {
	auto& currentBattleGroup = _battleGroups.at(GameState::Battle::NextBattleGroup);
	int i = 0;
	for (auto battlerID : currentBattleGroup) {
		if (battlerID == 0) {
			++i;
			continue;
		}
		auto spawnLocation = BattleLocation::GetBattleLocation(i + 4);
		auto& battlerData = _battlerData.at(battlerID);
		auto battler = new Battler(&battlerData, spawnLocation->X(), spawnLocation->Y());
		_battlers.push_back(battler);
		++i;
	}
}

static void loadBattleUIData() {
	auto vlg = UI::RootUIObject->GetChildByName("CommandsVLG");
	if (!vlg) {
		sgLogCritical("Could not fild child commandsvlg, exiting");
	}
	menuItems[0] = vlg->GetChildByName("AttackText");
	menuItems[1] = vlg->GetChildByName("MagicText");
	menuItems[2] = vlg->GetChildByName("SkillsText");
	menuItems[3] = vlg->GetChildByName("ItemsText");
	for (auto menuItem : menuItems) {
		if (!menuItem)
			sgLogCritical("Could not fild child thing, exiting");
	}
	finger = UI::RootUIObject->GetChildByName("Finger");
}

static void loadBattle() {
	loadBattleDB();
	loadBattleGroups();
	loadBattlers();
	loadEnemies();
	loadBattleUIData();
}

static void moveCursorInMenu(int menuLocation) {
	auto uiobject = menuItems[menuLocation];
	if (!uiobject) sgLogCritical("No menu item to switch to");
	auto thing = uiobject->GetAbsolutePosition();
	auto x = thing.x - 8;
	auto y = thing.y + (15 * menuLocation) + 8;
	sgLogDebug("Moving to %d, Setting finger to position %f, %f from: %f %f", menuLocation, (double)x, (double)y, (double)finger->X(), (double)finger->Y());
	finger->SetAbsolutePosition(x, y);
}

static void BattleUpdate() {
	auto newLocation = _currentMenuLocation;
	if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.UP)) {
		--newLocation;
	}

	else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.DOWN)) {
		++newLocation;
	}
	if (newLocation != _currentMenuLocation) {
		_currentMenuLocation = newLocation > 3 ? _currentMenuLocation == 3 ? 0 : 3 : newLocation;
		moveCursorInMenu(_currentMenuLocation);
	}
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
