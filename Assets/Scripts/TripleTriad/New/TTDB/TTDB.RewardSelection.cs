using System.Collections;
using System.Collections.Generic;
using ETF.TripleTriad;
using UnityEngine;

namespace ETF.TripleTriad
{

	public partial class TTDB
	{

		#region Configuration


		#endregion



		#region Functions

		public TripleTriadCardOnBoard[] RetrieveAllCardsOnBoard()
		{
			return _boardTripleTriadCards;
		}

		public TripleTriadCardInHand[] RetrieveAllPlayerHandCardsFull()
		{
			return _myFullHandTripleTriadCards;
		}

		#endregion
	}
}
