﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public class RewardSelectedState : TtState
	{

		#region Configuration

		[SerializeField] private TripleTriadManager _ttMan;
		private TripleTriadCardInHand _wonCard;
		private int _whoWonTheGame;

		#endregion

		public override void Startup(int additionalArgs = 0)
		{
			_whoWonTheGame = _ttMan.ttDb.RetrieveWhoWonTheGame();

			if (_whoWonTheGame == 1)
			{//playerwon
				_wonCard = _ttMan.ttDb.fullEnemyTripleTriadCards[
					_ttMan.ttDb.RetrieveNumberOfCurrentSelectionInRewardSelection()];
				_ttMan.ttUi.InitializeRewardSelectedUiWin(_wonCard.whatCardIAm.cardName);
				_wonCard.cardAnimator.SetTrigger("rewardSelected");
				//SoundManager.instance.PlaySFX(6);
			}
			else
			{//enemy won
				_wonCard = _ttMan.ttDb.RetrievePlayerTripleTriadCardAtLocation(additionalArgs);
				_ttMan.ttUi.InitializeRewardSelectedUiLose(_wonCard.whatCardIAm.cardName);
				_wonCard.cardAnimator.SetTrigger("rewardSelected");
				
			}

		}

		public override void Execute()
		{
			ListenForUserInput();
		}
		

		#region Functions

		private void ListenForUserInput()
		{
			if (_ttMan.ttUi.isLoading) return;
			if (!Input.GetKeyDown(KeyCode.Space)) return;
			SoundManager.instance.PlaySFX(_whoWonTheGame == 1 ? 17 : 6);
			_wonCard.cardAnimator.SetTrigger("rewardConfirmed");
			_ttMan.ttUi.isLoading = true;
		}

		#endregion
	}
}
