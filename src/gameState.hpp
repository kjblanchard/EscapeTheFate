#pragma once
#include <Supergoon/Primitives/Vector2.h>
#include <Supergoon/Primitives/rectangle.h>

#include <battle/statuseffects/statusEffects.hpp>
#include <string>
#include <vector>
namespace Etf {
struct PlayerData {
	RectangleF Location = {0, 0, 0, 0};
	bool MovedThisFrame = false;
	bool Spawned = false;
};

class LocalPlayer;
struct GameState {
	static void ResetForNewGame();
	static int CurrentFadeState;
	// This is the location that we will load on the next screen, used when switching maps to one of the predefined places.
	static int NextLoadScreen;
	// Used when we are exiting a battle, so that we load from this location.
	static sgVector2 NextLoadLocation;
	static sgVector2 NextLoadLocation2;
	// Used when we are exiting a battle, so that we load this map
	static std::string NextLoadMapName;
	// Used when we are exiting a battle, so we know what direction we should be facing.
	static int NextLoadDirection;
	static float DeltaTimeSeconds;
	static float DeltaTimeMilliseconds;
	static float TotalPlaytimeSeconds;
	static bool InDialog;
	static bool Paused;
	static bool IsMultiplayer;
	static bool IsLanHost;
	static std::string LanServerIP;
	static int SelectedPlayerCharacter;
	static std::string SelectedOverworldSprite;
	static int SelectedOverworldFrameW;
	static int SelectedOverworldFrameH;
	static int SelectedPlayerCharacter2;
	static std::string SelectedOverworldSprite2;
	static int SelectedOverworldFrameW2;
	static int SelectedOverworldFrameH2;
	struct Players {
		static PlayerData LocalPlayerData[2];
	} Players;
	struct Battle {
		static float CurrentStepsWithoutBattle;
		// If this is set to true, players will load at the last load location
		static bool ExitingFromBattle;
		// If this is set to true, the battle system will uddate each frame.
		static bool InBattle;
		static int NextBattleGroup;
		static std::vector<StatusEffects> PlayerRelics;
	} Battle;
	struct Menu {
		static bool MenuOpen[2];
	} Menu;
};

}  // namespace Etf
