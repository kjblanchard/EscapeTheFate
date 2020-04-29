using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public partial class TTLogic
	{

		#region Configuration


		#endregion



		#region Functions

		public bool CanIScrollInOpponentSelect(TTDB.MovementDirections whichDirectionToMove)
		{
			switch (whichDirectionToMove)
			{
				case TTDB.MovementDirections.Up:
					return ttDb.RetrieveOpponentSelectionCurrentValue() > 2;
				case TTDB.MovementDirections.Right:
					return ttDb.RetrieveOpponentSelectionCurrentValue() < 8;
				case TTDB.MovementDirections.Down:
					return ttDb.RetrieveOpponentSelectionCurrentValue() < 6;
				case TTDB.MovementDirections.Left:
					return ttDb.RetrieveOpponentSelectionCurrentValue() > 0;
				default:
					return false;
				
			}
			
		}


		#endregion
	}
}
