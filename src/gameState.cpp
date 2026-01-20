#include <gameState.hpp>
using namespace Etf;
int GameState::NextLoadScreen = 0;
float GameState::DeltaTimeSeconds;
float GameState::DeltaTimeMilliseconds;
unsigned int GameState::Ticks;
bool GameState::InDialog = false;
bool GameState::Battle::InBattle = false;
int GameState::Battle::NextBattleGroup = 4;
