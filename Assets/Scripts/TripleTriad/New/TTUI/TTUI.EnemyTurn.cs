using System.Collections;
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
			SoundManager.instance.PlaySFX(6);
		}

		public void InitializeEnemyTurnStateUi()
		{
			enemyHandDialogBoxAnimator.gameObject.SetActive(false);
			locationSelectionFinger.SetActive(false);
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

		public void InitializeBetweenTurnUiPlayerToEnemy()
		{
			//setOutFocusOnPlayerCardSelected();
			isLoading = true;
			playerSelectionFinger.SetActive(false);
		}
		
		public void InitializeBetweenTurnUiEnemyToPlayer()
		{
			isLoading = false;
		}

		#endregion
	}
}
