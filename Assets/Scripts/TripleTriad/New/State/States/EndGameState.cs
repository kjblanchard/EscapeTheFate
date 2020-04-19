using System.Collections;
using System.Collections.Generic;
using ETF.TripleTriad;
using UnityEngine;

namespace ETF.TripleTriad
{

	public class EndGameState : TtState
	{

		#region Configuration

		[SerializeField] private TripleTriadManager _ttMan;
		private int whoWonTheGame;
		#endregion

		public override void Startup(int additionalArgs = 0)
		{
			
			//whoWonTheGame = additionalArgs;
			_ttMan.ttUi.InitializeEndGameInitialUi();
			//_ttMan.ttUi.InitializeWinEndGameUi();
			
			if (_ttMan.ttDb.NumberOfPlayerOwnedCards() > _ttMan.ttDb.NumberOfEnemyOwnedCards())
			{
				whoWonTheGame = 1;
				_ttMan.ttUi.InitializeWinEndGameUi();
			}
			else if (_ttMan.ttDb.NumberOfPlayerOwnedCards() == _ttMan.ttDb.NumberOfEnemyOwnedCards())
			{
				_ttMan.ttUi.InitializeTieEndGameUi();
				whoWonTheGame = 2;
			}
			else
			{
				_ttMan.ttUi.InitializeLoseEndGameUi();
				whoWonTheGame = 3;
			}

		}

		public override void Execute()
		{
			ListenForUserInput();
		}

		public override void End()
		{
			_ttMan.ttUi.isLoading = true;
			_ttMan.ttUi.RewardSelectionFadeOut();
		}

		#region Functions

		public void ListenForUserInput()
		{
			if (Input.GetKeyDown(KeyCode.Space) && !_ttMan.ttUi.isLoading)
			{
				switch (whoWonTheGame)  
				{
					case 1:
						_ttMan.SendStateChange(_ttMan.rewardSelectionState,1);
						break;
					case 2:
						_ttMan.ttUi.isLoading = true;
						_ttMan.ttUi.FinishedGameFadeOut();
						break;
					case 3:
						_ttMan.SendStateChange(_ttMan.rewardSelectionState,3);
						break;
				
				}
			}
		}
		
		

		#endregion
	}
}
