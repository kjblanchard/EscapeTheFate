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
		#endregion

		public override void Startup(int additionalArgs = 0)
		{
			_ttMan.ttUi.InitializeEndGameInitialUi();
			
			if (_ttMan.ttDb.NumberOfPlayerOwnedCards() > _ttMan.ttDb.NumberOfEnemyOwnedCards())
			{
				_ttMan.ttUi.InitializeWinEndGameUi();
			}
			else if (_ttMan.ttDb.NumberOfPlayerOwnedCards() == _ttMan.ttDb.NumberOfEnemyOwnedCards())
			{
				_ttMan.ttUi.InitializeTieEndGameUi();
			}
			else
			{
				_ttMan.ttUi.InitializeLoseEndGameUi();
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
			if (Input.GetKeyDown(KeyCode.Space))
			{
				_ttMan.SendStateChange(_ttMan.rewardSelectionState);
			}
		}
		
		

		#endregion
	}
}
