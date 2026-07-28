#include <engine.hpp>
#include <gameState.hpp>
using namespace Etf;

void GameState::ResetForNewGame() {
	NextLoadScreen = 0;
	NextLoadLocation = {0, 0};
	NextLoadMapName = "";
	NextLoadDirection = 0;
	InDialog = false;
	CurrentFadeState = (int)LoadingScreenFadeTypes::NotFading;
	Battle::InBattle = false;
	Battle::NextBattleGroup = 1;
	Battle::ExitingFromBattle = false;
	Battle::CurrentStepsWithoutBattle = 0;
	for (auto& pd : Players::LocalPlayerData) {
		pd.Location = {0, 0, 0, 0};
		pd.MovedThisFrame = false;
		pd.Spawned = false;
	}
}

// int GameState::IsEngineStarted = false;
int GameState::NextLoadScreen = 0;
sgVector2 GameState::NextLoadLocation = {0, 0};
float GameState::DeltaTimeSeconds;
float GameState::DeltaTimeMilliseconds;
bool GameState::InDialog = false;
int GameState::CurrentFadeState = (int)LoadingScreenFadeTypes::NotFading;
PlayerData GameState::Players::LocalPlayerData[2];
bool GameState::Battle::InBattle = false;
int GameState::Battle::NextBattleGroup = 1;
bool GameState::Battle::ExitingFromBattle = false;
std::string GameState::NextLoadMapName = "";
int GameState::NextLoadDirection = 0;
float GameState::Battle::CurrentStepsWithoutBattle = 0;
