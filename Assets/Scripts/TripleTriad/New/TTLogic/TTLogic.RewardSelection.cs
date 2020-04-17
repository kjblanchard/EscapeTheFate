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

		public bool CanIScrollRightInRewardSelection()
		{
			return 
				!ttUi.isLoading && ttDb.RetrieveNumberOfCurrentSelectionInRewardSelection() > 0;
			
		}

		public bool CanIScrollLeftInRewardSelection()
		{
			return !ttUi.isLoading && ttDb.RetrieveNumberOfCurrentSelectionInRewardSelection() < 4;
		}

		public bool CanISelectInRewardSelection()
		{
			return !ttUi.isLoading;
		}


		#endregion
	}
}
