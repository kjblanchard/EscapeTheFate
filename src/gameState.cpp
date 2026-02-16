#include <bindings/engine.hpp>
#include <gameState.hpp>
using namespace Etf;
int GameState::NextLoadScreen = 0;
sgVector2 GameState::NextLoadLocation = {0, 0};
float GameState::DeltaTimeSeconds;
float GameState::DeltaTimeMilliseconds;
bool GameState::InDialog = false;
int GameState::CurrentFadeState = (int)ScreenFadeTypes::NotFading;
bool GameState::Players::Player2Spawned = false;

bool GameState::Battle::InBattle = false;
int GameState::Battle::NextBattleGroup = 1;
bool GameState::Battle::ExitingFromBattle = false;
std::string GameState::NextLoadMapName = "";
int GameState::NextLoadDirection = 0;
