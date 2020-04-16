using System.Collections;
using System.Collections.Generic;
using ETF.TripleTriad;
using UnityEngine;

namespace ETF.TripleTriad
{

	public partial class TTDB
	{

		#region Configuration

		private int _rewardSelectionCurrentOption;
		

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

		public Transform RetrieveCursorPositionInRewardScreen()
		{
			return fullEnemyTripleTriadCards[_rewardSelectionCurrentOption].fingerPointer.transform;
		}

		public void MoveRewardSelectionCurrentOptionRight()
		{
			_rewardSelectionCurrentOption++;
		}

		public void MoveRewardSelectionCurrentOptionLeft()
		{
			_rewardSelectionCurrentOption--;
		}

		public TripleTriadCardInHand RetrieveTripleTriadCardInEnemyHandSelected()
		{
			return fullEnemyTripleTriadCards[_rewardSelectionCurrentOption];
		}
		


		
			#endregion
	}
}
