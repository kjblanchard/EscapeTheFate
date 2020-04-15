using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public partial class TTUI
	{

		#region Configuration

		[SerializeField] private TripleTriadUiFade _tripleTriadUiFade;

		#endregion
		

		#region Functions

		public void ReturnAllBoardCardsToBase()
		{
			var boardCards = ttdb.RetrieveAllCardsOnBoard();
			for (int i = 0; i < boardCards.Length; i++)
			{	
				boardCards[i].cardAnimator.SetTrigger("moveToBase");
			}
		}

		public void RewardSelectionFadeIn()
		{
			_tripleTriadUiFade.animator.SetTrigger("rewardFadeIn");
		}
		
		public void RewardSelectionFadeOut()
		{
			_tripleTriadUiFade.animator.SetTrigger("rewardFadeOut");
		}
		
		public IEnumerator MoveAllPlayerHandCardsToRewardSelection()
		{
			var handCards = ttdb.RetrieveAllPlayerHandCardsFull();
			for (int i = 0; i < handCards.Length ; i++)
			{
				handCards[i].cardAnimator.SetTrigger("moveToReward");
				SoundManager.instance.PlaySFX(6);
				yield return new WaitForSeconds(0.1f);
			}
		}
		public void MoveAllEnemyHandCardsToRewardSelection()
		{
			
			for (int i = 0; i < ttMan.ttDb.fullEnemyTripleTriadCards.Length ; i++)
			{
				ttMan.ttDb.fullEnemyTripleTriadCards[i].cardAnimator.SetTrigger("moveToReward");
			}
		}

		public void InitializeRewardScreenUiElements()
		{
			_endGameCanvas.gameObject.SetActive(true);
			_playerScoreText.gameObject.SetActive(false);
			_enemyScoreText.gameObject.SetActive(false);
			enemyDialogBoxDialogText.gameObject.SetActive(false);
			print("done");
		}

		#endregion
	}
}
