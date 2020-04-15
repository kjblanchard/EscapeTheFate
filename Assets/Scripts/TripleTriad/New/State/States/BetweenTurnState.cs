using System.Collections;
using System.Collections.Generic;
using ETF.TripleTriad;
using UnityEngine;

namespace ETF.TripleTriad
{

	public class BetweenTurnState : TtState
	{

		#region Configuration

		private TtState _stateComingFrom;
		[SerializeField] private TripleTriadManager _ttMan;
		[SerializeField] private FingerAnimationChanger _turnSelectionFinger;

		#endregion

		
		public override void Startup(int additionalArgs = 0)
		{
			_ttMan.ttUi.InitializeBetweenTurnStateUi();
			_ttMan.ttUi.UpdateScoreCount();
			
			if (!_ttMan.ttLogic.CheckToSeeIfWeShouldEndTheGame())
			{
				_stateComingFrom = _ttMan.RetrievePreviousState();
				if (_stateComingFrom == _ttMan.locationSelectionState)
				{
					_turnSelectionFinger.PlayChangeToEnemyTurnAnimation();
				}
				else if (_stateComingFrom == _ttMan.enemyTurnState)
				{
					_turnSelectionFinger.PlayChangeToPlayerTurnAnimation();
				}
			}else
			{
				_ttMan.SendStateChange(_ttMan.endGameState);
			}
			
		}



		#region Functions



		#endregion
	}
}
