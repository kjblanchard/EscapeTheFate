#include <gameState.hpp>
using namespace Etf;
int GameState::NextLoadScreen = 0;
sgVector2 GameState::NextLoadLocation = {0, 0};
float GameState::DeltaTimeSeconds;
float GameState::DeltaTimeMilliseconds;
unsigned int GameState::Ticks;
bool GameState::InDialog = false;
bool GameState::Battle::InBattle = false;
int GameState::Battle::NextBattleGroup = 1;
std::string GameState::NextLoadMapName = "";
	int GameState:: NextLoadDirection = 0;
