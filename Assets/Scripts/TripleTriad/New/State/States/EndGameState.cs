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

		public override void Startup()
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

		#region Functions



		#endregion
	}
}
