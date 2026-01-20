#pragma once
namespace Etf {
struct GameState {
	static int NextLoadScreen;
	static float DeltaTimeSeconds;
	static float DeltaTimeMilliseconds;
	static unsigned int Ticks;
	static bool InDialog;
	struct Battle {
		static bool InBattle;
		static int NextBattleGroup;
	} Battle;
};

}  // namespace Etf
