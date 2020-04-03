using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public partial class TTDB
	{

		#region Configuration

		private int _currentFingerLocationInLocationSelection;
		[SerializeField] private TripleTriadCard[] _boardTripleTriadCards;

		#endregion


		#region Functions

		public int RetrieveLocationSelectionCurrentSelection()
		{
			return _currentFingerLocationInLocationSelection;
		}

		public void MoveLocationFingerRightInDb()
		{
			_currentFingerLocationInLocationSelection++;
		}

		public void MoveLocationFingerLeftInDb()
		{
			_currentFingerLocationInLocationSelection--;
		}

		public void MoveLocationFingerDownInDb()
		{
			_currentFingerLocationInLocationSelection += 3;
		}

		public void MoveLocationFingerUpInDb()
		{
			_currentFingerLocationInLocationSelection -= 3;
		}

		public TripleTriadCard RetrieveTripleTriadCardInPlayerSelection()
		{
			return _myHandTripleTriadCards[_playerTurnCurrentSelection];
		}

		public TripleTriadCard RetrieveTripleTriadCardInBoardSelection(int boardLocation)
		{
			return _boardTripleTriadCards[boardLocation];
		}

		public TripleTriadCard RetrieveCard2InMyHandTest()
		{
			return _myHandTripleTriadCards[1];
		}
		

		#endregion
	}
}
