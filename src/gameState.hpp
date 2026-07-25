#pragma once
#include <Supergoon/Primitives/Vector2.h>
#include <Supergoon/Primitives/rectangle.h>

#include <string>
namespace Etf {
struct PlayerData {
	RectangleF Location = {0, 0, 0, 0};
	bool MovedThisFrame = false;
	bool Spawned = false;
};

class LocalPlayer;
struct GameState {
	// static int IsEngineStarted;
	static int CurrentFadeState;
	// This is the location that we will load on the next screen, used when switching maps to one of the predefined places.
	static int NextLoadScreen;
	// Used when we are exiting a battle, so that we load from this location.
	static sgVector2 NextLoadLocation;
	// Used when we are exiting a battle, so that we load this map
	static std::string NextLoadMapName;
	// Used when we are exiting a battle, so we know what direction we should be facing.
	static int NextLoadDirection;
	static float DeltaTimeSeconds;
	static float DeltaTimeMilliseconds;
	static bool InDialog;
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
	} Battle;
};

}  // namespace Etf
