﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public partial class TTUI
	{

		#region Configuration


		#endregion
	

		#region Functions

		public void PlayCardInEnemyTurn(int enemyHandCardNumber)
		{
			ttdb.currentEnemyTripleTriadCardsInHand[enemyHandCardNumber].CardPlayed();
		}
		
		
		public void setFocusOnEnemyCardSelected()
		{
			ttdb.currentEnemyTripleTriadCardsInHand[ttMan.ttLogic.RetrieveHandPositionToPlaceCard()].cardAnimator.SetTrigger("inFocus");
			//myHandCardAnimators[ttdb.RetrievePlayerTurnCurrentSelection()].SetTrigger("inFocus");
		}

		public void setOutFocusOnEnemyCardSelected()
		{
			ttdb.currentEnemyTripleTriadCardsInHand[ttMan.ttLogic.RetrieveHandPositionToPlaceCard()].cardAnimator.SetTrigger("outFocus");
			//myHandCardAnimators[ttdb.RetrievePlayerTurnCurrentSelection()].SetTrigger("outFocus");

		}

		#endregion
	}
}