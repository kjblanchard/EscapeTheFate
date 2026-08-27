#include <Supergoon/Input/keyboard.h>
#include <Supergoon/camera.h>
#include <Supergoon/engine.h>
#include <Supergoon/filesystem.h>
#include <Supergoon/json.h>
#include <assert.h>
#include <sgtools/log.h>

#include <battle/battleZones.hpp>
#include <battle/battlerData.hpp>
#include <engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/BattleLocation.hpp>
#include <gameobject/gameobjects/EnemyBattler.hpp>
#include <gameobject/gameobjects/PlayerBattler.hpp>
#include <systems/BattleSpoilsSystem.hpp>
#include <systems/PlayerControllerSystem.hpp>
#include <systems/battleSystem.hpp>
#include <ui/ui.hpp>
#include <vector>

using namespace Etf;
using namespace std;

// Non statics for debugging
bool battleInitialized_ = false;
BattleStates currentBattleState_ = NotInBattle;
BattleStates nextBattleState_ = NotInBattle;
string sceneToLoadAfterBattle_ = "";
vector<BattlerData> battlerDatabase_;
vector<AbilityData> abilityDatabase_;
//  Loaded battle groups from the database, used when loading battle and stays loaded
vector<vector<int>> _battleGroups;
// Current battlers spawned in, always the size of all positions.
vector<Battler*> _battlers;

namespace {
// Holds all of the UI objects in a (organized?) place.
struct BattleUI {
	// Top level, hide or show the whole thing
	UIObject* RootPanel = nullptr;
	// Player hud
	UIObject* PlayerHUD = nullptr;
	// All the player commands menus, usually controlled by the battler.
	UIObject* PlayerCommandsObjects[3] = {nullptr, nullptr, nullptr};
	UIObject* VictoryPanel = nullptr;
} battleUI_;

}  // namespace

static void battleEnd() {
	battleUI_.VictoryPanel->SetVisible(false);
	BattleLocation::ClearAllBattleLocations();
	_battlers.clear();
	ResetCameraFollow();
	Engine::LoadScene(sceneToLoadAfterBattle_, 0.75f, 0.25f, false);
	nextBattleState_ = NotInBattle;
	GameState::Battle::InBattle = false;
	GameState::Battle::ExitingFromBattle = true;
}

static void loadBattleGroups() {
	char* buf;
	size_t sz;
	Engine::Json::GetJsonBufferFromDirectory("battleGroups", &buf, &sz);
	auto dataRootJsonArray = jGetObjectFromBuffer(buf, sz);
	if (!dataRootJsonArray) sgLogCritical("No battle groups found");
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
	char* buf;
	size_t sz;
	Engine::Json::GetJsonBufferFromDirectory("battleDB", &buf, &sz);
	auto dataRootJsonArray = jGetObjectFromBuffer(buf, sz);
	if (!dataRootJsonArray) sgLogCritical("No battler Database found exiting");
	auto numData = jGetObjectArrayLength(dataRootJsonArray);
	if (!numData) sgLogCritical("No battlers found in db, exiting!");
	for (auto i = 0; i < numData; ++i) {
		auto currentJsonObject = jGetObjectInObjectWithIndex(dataRootJsonArray, i);
		if (!currentJsonObject) continue;
		battlerDatabase_.emplace_back();
		battlerDatabase_.back().Name = jstr(currentJsonObject, "name");
		battlerDatabase_.back().HP = jint(currentJsonObject, "hp");
		battlerDatabase_.back().Str = jint(currentJsonObject, "str");
		battlerDatabase_.back().Mag = jint(currentJsonObject, "mag");
		battlerDatabase_.back().Def = jint(currentJsonObject, "def");
		battlerDatabase_.back().MDef = jint(currentJsonObject, "mdef");
		battlerDatabase_.back().Spd = jint(currentJsonObject, "spd");
		battlerDatabase_.back().Pow = jint(currentJsonObject, "pow");
		battlerDatabase_.back().Sprite = jstr(currentJsonObject, "sprite");
		battlerDatabase_.back().IdleAnimation = jstr(currentJsonObject, "idle");
		auto portraitStr = jstr(currentJsonObject, "portrait");
		battlerDatabase_.back().Portrait = portraitStr ? portraitStr : "";
		auto nick = jstr(currentJsonObject, "nick");
		battlerDatabase_.back().Nick = nick ? nick : "";
		auto portraitRectObj = jobj(currentJsonObject, "portraitRect");
		if (portraitRectObj) {
			battlerDatabase_.back().PortraitRect = Engine::Json::GetRectFromObject(currentJsonObject, "portraitRect");
		}
		battlerDatabase_.back().DamageOffsetX = jint(currentJsonObject, "damageOffsetX");
		battlerDatabase_.back().DamageOffsetY = jint(currentJsonObject, "damageOffsetY");
		battlerDatabase_.back().HpBarOffsetX = jint(currentJsonObject, "hpBarOffsetX");
		battlerDatabase_.back().HpBarOffsetY = jint(currentJsonObject, "hpBarOffsetY");
		battlerDatabase_.back().AnimOffsetX = jint(currentJsonObject, "animOffsetX");
		battlerDatabase_.back().AnimOffsetY = jint(currentJsonObject, "animOffsetY");
		battlerDatabase_.back().XPReward = jint(currentJsonObject, "xpReward");
		battlerDatabase_.back().CurrentXP = jint(currentJsonObject, "currentXP");
		battlerDatabase_.back().XPToNextLevel = jint(currentJsonObject, "xpToNextLevel");
		auto itemDropsArray = jobj(currentJsonObject, "itemDrops");
		if (itemDropsArray) {
			auto numDrops = jGetObjectArrayLength(itemDropsArray);
			for (auto d = 0; d < numDrops; ++d) {
				auto dropObj = jGetObjectInObjectWithIndex(itemDropsArray, d);
				if (!dropObj) continue;
				ItemDrop drop;
				drop.Name = jstr(dropObj, "name");
				drop.DropPercent = jint(dropObj, "percent");
				battlerDatabase_.back().ItemDrops.push_back(std::move(drop));
			}
		}
		battlerDatabase_.back().Location = Engine::Json::GetRectFromObject(currentJsonObject, "rect");
		battlerDatabase_.back().MaxAP = jKeyExists(currentJsonObject, "maxAP") ? jint(currentJsonObject, "maxAP") : 3;
		auto aiStr = jKeyExists(currentJsonObject, "ai") ? jstr(currentJsonObject, "ai") : nullptr;
		battlerDatabase_.back().AIStrategy = aiStr ? aiStr : "basic";
		auto abilitiesArray = jobj(currentJsonObject, "abilities");
		if (abilitiesArray) {
			auto numAbilities = jGetObjectArrayLength(abilitiesArray);
			for (auto a = 0; a < numAbilities; ++a) {
				auto abilityID = jintIndex(abilitiesArray, a);
				battlerDatabase_.back().Abilities.push_back(abilityID);
			}
		}
	}
	jReleaseObjectFromFile(dataRootJsonArray);
}

static void loadAbilityDB() {
	char* buf;
	size_t sz;
	Engine::Json::GetJsonBufferFromDirectory("abilities", &buf, &sz);
	auto dataRootJsonArray = jGetObjectFromBuffer(buf, sz);
	if (!dataRootJsonArray) sgLogCritical("No ability database found, exiting");
	auto numData = jGetObjectArrayLength(dataRootJsonArray);
	for (auto i = 0; i < numData; ++i) {
		auto currentJsonObject = jGetObjectInObjectWithIndex(dataRootJsonArray, i);
		if (!currentJsonObject) continue;
		abilityDatabase_.emplace_back();
		abilityDatabase_.back().Name = jstr(currentJsonObject, "name");
		abilityDatabase_.back().AnimationFile = jstr(currentJsonObject, "animFile");
		abilityDatabase_.back().AnimationTag = jstr(currentJsonObject, "animTag");
		abilityDatabase_.back().SFXName = jstr(currentJsonObject, "sfx");
		abilityDatabase_.back().PlayerAnim = jstr(currentJsonObject, "playerAnim");
		abilityDatabase_.back().BaseDamage = jint(currentJsonObject, "baseDamage");
		abilityDatabase_.back().Friendly = jbool(currentJsonObject, "friendly");
		abilityDatabase_.back().APCost = jKeyExists(currentJsonObject, "apCost") ? jint(currentJsonObject, "apCost") : 1;
		abilityDatabase_.back().Description = jKeyExists(currentJsonObject, "description") ? jstr(currentJsonObject, "description") : "";
		auto statusEffects = jobj(currentJsonObject, "statusEffects");
		if (statusEffects) {
			auto n = jGetObjectArrayLength(statusEffects);
			for (auto j = 0; j < n; ++j) {
				auto o = jGetObjectInObjectWithIndex(statusEffects, j);
				if (!o) continue;
				StatusEffectChance s;
				s.Id = jint(o, "id");
				s.ApplyChance = jfloat(o, "percent");
				abilityDatabase_.back().StatusEffects.push_back(std::move(s));
			}
		}
	}
	jReleaseObjectFromFile(dataRootJsonArray);
}

static void loadPlayers() {
	const int playerData = GameState::SelectedPlayerCharacter;
	const int playerSpawnLocation = 1;
	auto& p1BattlerData = battlerDatabase_.at(playerData);
	auto spawnLocation = BattleLocation::GetBattleLocation(playerSpawnLocation);
	BattlerArgs args;
	args.BattlerNum = 0;
	args.BattleData = &p1BattlerData;
	args.X = spawnLocation->X();
	args.Y = spawnLocation->Y();
	args.Controller = PlayerControllerSystem::GetPlayerByNum(0);
	auto battler = new PlayerBattler(args);
	_battlers.at(playerSpawnLocation) = battler;

	if (GameState::IsMultiplayer) {
		const int p2SpawnLocation = 2;
		auto& p2BattlerData = battlerDatabase_.at(GameState::SelectedPlayerCharacter2);
		auto p2Spawn = BattleLocation::GetBattleLocation(p2SpawnLocation);
		BattlerArgs p2Args;
		p2Args.BattlerNum = 1;
		p2Args.BattleData = &p2BattlerData;
		p2Args.X = p2Spawn->X();
		p2Args.Y = p2Spawn->Y();
		p2Args.Controller = PlayerControllerSystem::GetPlayerByNum(1);
		auto p2Battler = new PlayerBattler(p2Args);
		_battlers.at(p2SpawnLocation) = p2Battler;
	}
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
		args.BattleData = &battlerDatabase_.at(battlerID);
		auto battler = new EnemyBattler(args);
		_battlers.at(i + 4) = battler;
		++i;
	}
}

static void cacheBattleUIElements() {
	// Need to find the command menu, HUD, etc.
	battleUI_.RootPanel = UI::GetRootUIObject()->GetChildByName("BattlePanel");
	assert(battleUI_.RootPanel && "No root object found");
	battleUI_.PlayerHUD = UI::GetRootUIObject()->GetChildByName("PlayerStatusHUD");
	assert(battleUI_.PlayerHUD && "No hud object found");
	int battlerNum = 1;
	for (auto& obj : battleUI_.PlayerCommandsObjects) {
		auto nameLookup = "Player" + to_string(battlerNum) + "CommandsUI";
		obj = battleUI_.RootPanel->GetChildByName(nameLookup);
		++battlerNum;
		assert(obj && "No command object found");
	}
	battleUI_.VictoryPanel = battleUI_.RootPanel->GetChildByName("VictoryPanel");
	assert(battleUI_.VictoryPanel && "No victory panel found");
}

static void initializeBattleSystem() {
	sgLogWarn("Initializing battle system and battle db!");
	loadBattleDB();
	loadAbilityDB();
	loadBattleGroups();
	cacheBattleUIElements();
	battleInitialized_ = true;
	battleUI_.RootPanel->SetVisible(false);
	_battlers.clear();
	nextBattleState_ = NotInBattle;
}

static int relicDefaultDuration(StatusEffects type) {
	switch (type) {
		case StatusEffects::RelicDamageBonus: return -1;
		case StatusEffects::RelicSpeedBoost: return 2;
		case StatusEffects::RelicShield: return 5;
		default: return 0;
	}
}

static void loadBattle() {
	IsGameLoading = true;
	if (!battleInitialized_) initializeBattleSystem();
	_battlers.resize(8);
	sgLogDebug("loading battle");
	// Something is terrible with load players.
	loadPlayers();
	loadEnemies();
	for (auto relicType : GameState::Battle::PlayerRelics) {
		StatusEffectInstance sei = {relicType, relicDefaultDuration(relicType)};
		for (auto* b : _battlers) {
			if (b && b->IsPlayer()) {
				b->ApplyStatusEffect(sei);
				if (relicType == StatusEffects::RelicSpeedBoost) {
					b->AddSpdBonus(2);
				}
			}
		}
	}
	battleUI_.RootPanel->SetVisible(true);
	battleUI_.PlayerHUD->SetVisible(true);
	battleUI_.VictoryPanel->SetVisible(false);
	if (GameState::IsMultiplayer) {
		battleUI_.PlayerCommandsObjects[1]->SetVisible(true);
		auto p2StatusHUD = battleUI_.PlayerHUD->GetChildByName("Player2StatusHUD");
		if (p2StatusHUD) p2StatusHUD->SetVisible(true);
	}
}

static void battleVictory() {
	battleUI_.VictoryPanel->SetVisible(true);
}

static void BattleUpdate() {}

// Used to reduce the boilerplate if we change states in multiple places
static void triggerStateChange() {
	switch (nextBattleState_) {
		case BattleStartTriggered:
			loadBattle();
			break;
		case BattleVictory:
			battleVictory();
			break;
		case BattleSpoils:
			battleUI_.VictoryPanel->SetVisible(false);
			BattleSpoilsSystem::TriggerBattleSpoils();
			break;
		case BattleEnd:
			battleEnd();
			break;
		case BattleGameOver:
			battleUI_.RootPanel->SetVisible(false);
			_battlers.clear();
			break;
		default:
			break;
	}
	currentBattleState_ = nextBattleState_;
}

void BattleSystem::TriggerBattleStart() {
	if ((currentBattleState_ == NotInBattle) && nextBattleState_ != BattleStartTriggered) {
		sceneToLoadAfterBattle_ = Engine::CurrentSceneName();
		nextBattleState_ = BattleStates::BattleStartTriggered;
		GameState::Battle::InBattle = true;
	}
}
void BattleSystem::TriggerBattleSpoils() {
	if (currentBattleState_ == BattleSpoils || nextBattleState_ == BattleSpoils) return;
	nextBattleState_ = BattleSpoils;
	triggerStateChange();
}
void BattleSystem::TriggerBattleEnd() {
	nextBattleState_ = BattleEnd;
	triggerStateChange();
}

void BattleSystem::TriggerGameOver() {
	nextBattleState_ = BattleGameOver;
	triggerStateChange();
}

void BattleSystem::ResetAfterGameOver() {
	currentBattleState_ = NotInBattle;
	nextBattleState_ = NotInBattle;
	_battlers.clear();
}
void BattleSystem::TriggerBattleVictoryStart() {
	if (currentBattleState_ == BattleVictory || nextBattleState_ == BattleVictory) return;
	nextBattleState_ = BattleVictory;
	triggerStateChange();
}

void BattleSystem::BattleSystemUpdate() {
	if (!GameState::Battle::InBattle) return;
	if (nextBattleState_ != currentBattleState_) triggerStateChange();
	switch (currentBattleState_) {
		case NotInBattle:
			break;
		case BattleStartTriggered:
			nextBattleState_ = Battle;
			IsGameLoading = false;
			break;
		case Battle:
			BattleUpdate();
			break;
		case BattleSpoils:
			if (BattleSpoilsSystem::IsBattleSpoilsDone()) {
				nextBattleState_ = BattleEnd;
			}
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

const AbilityData& BattleSystem::GetAbilityByID(int id) {
	return abilityDatabase_.at(id);
}

bool BattleSystem::HasAbility(int id) {
	return id >= 0 && id < (int)abilityDatabase_.size();
}

const BattlerData* BattleSystem::GetPlayerBattlerData(int playerIndex) {
	int dbIdx = (playerIndex == 0) ? GameState::SelectedPlayerCharacter : GameState::SelectedPlayerCharacter2;
	if (dbIdx < 0 || dbIdx >= (int)battlerDatabase_.size()) return nullptr;
	return &battlerDatabase_[dbIdx];
}
