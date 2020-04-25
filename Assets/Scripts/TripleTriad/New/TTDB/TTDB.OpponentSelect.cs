using System.Collections;
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

		#endregion



		#region Functions

		public void InitializeOpponentSelectionDb()
		{
			_opponentSelectionCurrentSelection = 6;
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

		#endregion
	}
}
