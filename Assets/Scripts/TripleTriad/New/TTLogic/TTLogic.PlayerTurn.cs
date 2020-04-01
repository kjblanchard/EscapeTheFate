using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF
{

	public partial class TTLogic
	{

		#region Configuration


		#endregion

		#region Functions

		public bool CanIScrollDownInPlayerTurn()
		{
			return ttDb.RetrievePlayerTurnCurrentSelection() < ttDb.RetrieveTripleTriadCardLength() -1 && ttDb.RetrieveTripleTriadCardLength() > 1 && !ttUi.isLoading;
		}

		public bool CanIScrollUpInPlayerTurn()
		{
			return ttDb.RetrievePlayerTurnCurrentSelection() > 0&& ttDb.RetrieveTripleTriadCardLength() > 1 && !ttUi.isLoading;
		}

		public bool CanILoopDownInPlayerTurn()
		{
			return ttDb.RetrievePlayerTurnCurrentSelection() == ttDb.RetrieveTripleTriadCardLength()-1 && ttDb.RetrieveTripleTriadCardLength() > 1 && !ttUi.isLoading;
		}

		public bool CanILoopUpInPlayerTurn()
		{
			return ttDb.RetrievePlayerTurnCurrentSelection() == 0 && ttDb.RetrieveTripleTriadCardLength() > 1 && !ttUi.isLoading;
		}

		#endregion
	}
}
