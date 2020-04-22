using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public class TripleTriadUiFade : MonoBehaviour
	{

		#region Configuration

		[SerializeField] private TripleTriadManager _ttMan;
		[SerializeField] public Animator animator;

		#endregion

		

		#region Functions

		public void FadeInRewardScreen()
		{
			_ttMan.ttUi.RewardSelectionFadeIn();
		}

		public void StartShootingCards()
		{
			StartCoroutine(_ttMan.ttUi.MoveAllPlayerHandCardsToRewardSelection());
			StartCoroutine(_ttMan.ttUi.MoveAllEnemyHandCardsToRewardSelection());
		}

		public void ReturnBoardCardsToBase()
		{
			_ttMan.ttUi.ReturnAllBoardCardsToBase();
		}

		public void ReturnHandCardsToPreReward()
		{
			_ttMan.ttUi.MoveAllPlayerHandCardsToPreReward();
			_ttMan.ttUi.MoveAllEnemyHandCardsToPreReward();
	
		}

		public void InitializeRewardScreenUi()
		{
			_ttMan.ttUi.InitializeRewardScreenUiElements();
		}

		public void QuitGame()
		{
			//_ttMan.SendStateChange(_ttMan.showingRulesState,1);
			_ttMan.SendStateChange(_ttMan.disabledState);
			TurnOffTripleTriad();
			//Application.Quit();
		}

		public void ChangeToShowingRulesState()
		{
			_ttMan.SendStateChange(_ttMan.showingRulesState);
		}

		public void TurnOffTripleTriad()
		{
			SoundManager.instance.PlayIntroLoop(SoundManager.instance.bgmToPlay);
			StartCoroutine(UIFade.instance.EndCardBattleFade(0.5f));

		}
		

		#endregion
	}
}
