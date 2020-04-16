using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

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
		public IEnumerator MoveAllEnemyHandCardsToRewardSelection()
		{
			
			for (int i = 0; i < ttMan.ttDb.fullEnemyTripleTriadCards.Length ; i++)
			{
				ttMan.ttDb.fullEnemyTripleTriadCards[i].cardAnimator.SetTrigger("moveToReward");
				SoundManager.instance.PlaySFX(6);
				yield return new WaitForSeconds(0.1f);
			}
		}

		public void MoveAllPlayerHandCardsToPreReward()
		{
			var handCards = ttdb.RetrieveAllPlayerHandCardsFull();
			for (int i = 0; i < handCards.Length; i++)
			{
				handCards[i].cardAnimator.SetTrigger("preRewardScreen");
			}
		}
		public void MoveAllEnemyHandCardsToPreReward()
		{

			for (int i = 0; i < ttdb.fullEnemyTripleTriadCards.Length; i++)
			{
				ttdb.fullEnemyTripleTriadCards[i].cardAnimator.SetTrigger("preRewardScreen");
			}
		}
		
		
		public void InitializeRewardScreenUiElements()
		{
			_endGameCanvas.enabled = false;
			_playerScoreText.gameObject.SetActive(false);
			_enemyScoreText.gameObject.SetActive(false);
			enemyHandDialogBoxAnimator.gameObject.SetActive(false);
			print("done");
		}
		
		public void keepRewardSelectionFingerOnProperLocation()
		{
			if (isLoading == false)
			{
				if (!locationSelectionFinger.gameObject.activeInHierarchy)
				{
                    
					locationSelectionFinger.gameObject.SetActive(true);
				}

				locationSelectionFinger.transform.position =
					ttdb.RetrieveCursorPositionInRewardScreen().position;// [ttdb.RetrievePlayerTurnCurrentSelection()].transform.position;
			}
			else
			{
				locationSelectionFinger.gameObject.SetActive(false);
			}

		}
		
		public void FlipCurrentCardSelected()
		{
			var currentCard = ttdb.RetrieveTripleTriadCardInEnemyHandSelected();

			if (!currentCard.cardOwnedByPlayer)
			{
			
				ttdb.RetrieveTripleTriadCardInEnemyHandSelected().cardOwnedByPlayer = true;
				ttdb.RetrieveTripleTriadCardInEnemyHandSelected().cardAnimator.SetTrigger("cardFlip");

			}
			else
			{
				
				ttdb.RetrieveTripleTriadCardInEnemyHandSelected().cardOwnedByPlayer = false;
				ttdb.RetrieveTripleTriadCardInEnemyHandSelected().cardAnimator.SetTrigger("cardFlip");

			}
		}

		#endregion
	}
}
