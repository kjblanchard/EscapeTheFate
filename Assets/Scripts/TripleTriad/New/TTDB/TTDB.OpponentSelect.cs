﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public partial class TTDB
	{

		#region Configuration
		
		
		public enum MovementDirections
		{
			Up,
			Right,
			Left,
			Down
		}
		private int _opponentSelectionCurrentSelection;
		private int _currentOpponentPlaying;

		//[SerializeField] private EnemyCardHand[] _opponentEnemyCardHands;
		
		
		#endregion



		#region Functions

		public void InitializeOpponentSelectionDb()
		{
			_opponentSelectionCurrentSelection = 7;
		}
		
		public void MoveOpponentSelectionInDb(MovementDirections whichWayToMove)
		{
			switch (whichWayToMove)
			{
				case MovementDirections.Up:
					_opponentSelectionCurrentSelection -= 3;
					break;
				case MovementDirections.Right:
					_opponentSelectionCurrentSelection++;
					break;
				case MovementDirections.Down:
					_opponentSelectionCurrentSelection += 3;
					break;
				case MovementDirections.Left:
					_opponentSelectionCurrentSelection--;
					break;
			}
		}

		public int RetrieveOpponentSelectionCurrentValue()
		{
			return _opponentSelectionCurrentSelection;
		}


		public void BringInCurrentSelectedEnemyInformationToDb()
		{
			 ttMan.ttUi._opponentBoxes[_opponentSelectionCurrentSelection].opponentCardInfo.SendInfoToTtDb();
		}

		public void SetCurrentOpponentYouArePlaying()
		{
			_currentOpponentPlaying = _opponentSelectionCurrentSelection;
		}

		public int RetrieveCurrentOpponentInOpponentList()
		{
			return _currentOpponentPlaying;
		}

		#endregion
	}
}
