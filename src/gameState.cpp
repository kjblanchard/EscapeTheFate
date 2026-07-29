#include <engine.hpp>
#include <gameState.hpp>
using namespace Etf;

void GameState::ResetForNewGame() {
	NextLoadScreen = 0;
	NextLoadLocation = {0, 0};
	NextLoadMapName = "";
	NextLoadDirection = 0;
	InDialog = false;
	Paused = false;
	SelectedPlayerCharacter = 0;
	SelectedOverworldSprite = "player1";
	SelectedOverworldFrameW = 32;
	SelectedOverworldFrameH = 32;
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
bool GameState::Paused = false;
int GameState::SelectedPlayerCharacter = 0;
std::string GameState::SelectedOverworldSprite = "player1";
int GameState::SelectedOverworldFrameW = 32;
int GameState::SelectedOverworldFrameH = 32;
int GameState::CurrentFadeState = (int)LoadingScreenFadeTypes::NotFading;
PlayerData GameState::Players::LocalPlayerData[2];
bool GameState::Battle::InBattle = false;
int GameState::Battle::NextBattleGroup = 1;
bool GameState::Battle::ExitingFromBattle = false;
std::string GameState::NextLoadMapName = "";
int GameState::NextLoadDirection = 0;
float GameState::Battle::CurrentStepsWithoutBattle = 0;
