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

		public bool CanIScrollRightOnRuleSelection()
		{
			return ttDb.ReturnFingerPositionInShowingRules() < ttUi.ReturnShowingRulesFingerPositionCount() - 1;
		}
		
		public bool CanIScrollLeftOnRuleSelection()
		{
			return ttDb.ReturnFingerPositionInShowingRules() > 0 ;
		}
		
		public bool CanIPressCancelOnRuleSelection()
		{
			return ttDb.ReturnFingerPositionInShowingRules() == 0 ;
		}

		#endregion
	}
}
