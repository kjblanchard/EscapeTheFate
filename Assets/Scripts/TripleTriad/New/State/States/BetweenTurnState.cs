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

		
		public override void Startup()
		{
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

		// public override void End()
		// {
		// 	_ttMan.ttUi.LeavingTurnSelectionUiChange();
		// 	print("hi");
		// }


		#region Functions



		#endregion
	}
}
