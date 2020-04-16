using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public class RewardSelectionState : TtState
	{

		#region Configuration

		[SerializeField] private TripleTriadManager _ttMan;

		#endregion

		public override void Startup(int additionalArgs = 0)
		{
			//_ttMan.ttUi.ReturnAllBoardCardsToBase();
			//_ttMan.ttUi.RewardSelectionFadeIn();
			//_ttMan.ttUi.MoveAllPlayerHandCardsToRewardSelection();
			//_ttMan.ttUi.MoveAllEnemyHandCardsToRewardSelection();
			
			
			if (additionalArgs == 1)
			{
				//player can select
			}

			if (additionalArgs == 2)
			{
				//enemy can select
			}
			else
			{
				//no selectionhappens
			}
		}

		public override void Execute()
		{
			ListenForUserInputs();
			_ttMan.ttUi.keepRewardSelectionFingerOnProperLocation();
		}

		public override void End()
		{
			base.End();
		}

		#region Functions

		private void ListenForUserInputs()
		{
			if (Input.GetKeyDown(KeyCode.D) || Input.GetButtonDown("right"))
			{
				_ttMan.ttDb.MoveRewardSelectionCurrentOptionRight();
				SoundManager.instance.PlaySFX(0);

			}
			else if (Input.GetKeyDown(KeyCode.A) || Input.GetButtonDown("left"))
			{
				_ttMan.ttDb.MoveRewardSelectionCurrentOptionLeft();
				SoundManager.instance.PlaySFX(0);

			}

			else if ((Input.GetKeyDown(KeyCode.Space) || Input.GetButtonDown("Fire1")) && !_ttMan.ttUi.isLoading)
			{
				
				_ttMan.ttUi.FlipCurrentCardSelected();
				_ttMan.SendStateChange(_ttMan.rewardConfirmState);
			}
		}


		#endregion
	}
}
