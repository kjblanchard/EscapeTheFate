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

		public bool CanIScrollRightInLocationSelection()
		{
			return ttDb.RetrieveLocationSelectionCurrentSelection() < ttUi.RetrieveLocationSelectionTransformCount()-1 && !ttUi.isLoading;
		}

		public bool CanIScrollLeftInLocationSelection()
		{
			return ttDb.RetrieveLocationSelectionCurrentSelection() > 0 && !ttUi.isLoading;
		}

		public bool CanIMoveDownInLocationSelection()
		{
			return ttDb.RetrieveLocationSelectionCurrentSelection() <
			       (ttUi.RetrieveLocationSelectionTransformCount() - 3);
		}

		public bool CanIMoveUpInLocationSelection()
		{
			return ttDb.RetrieveLocationSelectionCurrentSelection() > 2;
		}

		#endregion
	}
}
