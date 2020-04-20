using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public class RewardSelectedState : TtState
	{

		#region Configuration

		[SerializeField] private TripleTriadManager _ttMan;
		private TripleTriadCardInHand wonCard;
		private int _whoWonTheGame;

		#endregion

		public override void Startup(int additionalArgs = 0)
		{
			_whoWonTheGame = _ttMan.ttDb.RetrieveWhoWonTheGame();

			if (_whoWonTheGame == 1)
			{//playerwon
				wonCard = _ttMan.ttDb.fullEnemyTripleTriadCards[
					_ttMan.ttDb.RetrieveNumberOfCurrentSelectionInRewardSelection()];
				_ttMan.ttUi.InitializeRewardSelectedUiWin(wonCard.whatCardIAm.cardName);
				wonCard.cardAnimator.SetTrigger("rewardSelected");
				//SoundManager.instance.PlaySFX(6);
			}
			else
			{//enemy won
				wonCard = _ttMan.ttDb.RetrievePlayerTripleTriadCardAtLocation(additionalArgs);
				_ttMan.ttUi.InitializeRewardSelectedUiLose(wonCard.whatCardIAm.cardName);
				wonCard.cardAnimator.SetTrigger("rewardSelected");
				
			}

		}

		public override void Execute()
		{
			ListenForUserInput();
		}

		public override void End()
		{
			base.End();
		}

		#region Functions

		public void ListenForUserInput()
		{
			if (Input.GetKeyDown(KeyCode.Space))
			{
				if(_whoWonTheGame ==1)
				{
					SoundManager.instance.PlaySFX(17);
					
				}
				else
				{
					SoundManager.instance.PlaySFX(6);
				}
				wonCard.cardAnimator.SetTrigger("rewardConfirmed");

			}
		}

		#endregion
	}
}
