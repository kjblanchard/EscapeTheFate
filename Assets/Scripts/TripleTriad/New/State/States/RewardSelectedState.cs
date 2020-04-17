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

		#endregion

		public override void Startup(int additionalArgs = 0)
		{
			_ttMan.ttUi.InitializeRewardSelectedUi();
			wonCard = _ttMan.ttDb.fullEnemyTripleTriadCards[
				_ttMan.ttDb.RetrieveNumberOfCurrentSelectionInRewardSelection()];
			wonCard.cardAnimator.SetTrigger("rewardSelected");
			SoundManager.instance.PlaySFX(6);
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
				SoundManager.instance.PlaySFX(17);
				wonCard.cardAnimator.SetTrigger("rewardConfirmed");
			}
		}

		#endregion
	}
}
