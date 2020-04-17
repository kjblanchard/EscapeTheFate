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
				_ttMan.ttDb.InitializeRewardSelectionDb();
			}

			if (additionalArgs == 2)
			{
				//enemy can select
			}
			else
			{
				//no selectionhappens
			}

			if (additionalArgs == 4)
			{//coming back from card selection screen
				_ttMan.ttUi.GoingBackToRewardSelectionUi();
				_ttMan.ttUi.isLoading = false;
				

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
			if ((Input.GetKeyDown(KeyCode.D) ||
			    Input.GetButtonDown("right")) && _ttMan.ttLogic.CanIScrollRightInRewardSelection())
			{
				_ttMan.ttDb.MoveRewardSelectionCurrentOptionRight();
				SoundManager.instance.PlaySFX(0);
				_ttMan.ttUi.UpdateTextInInfoPanelRewardSelection();

			}
			else if ((Input.GetKeyDown(KeyCode.A) ||
			         Input.GetButtonDown("left")) && _ttMan.ttLogic.CanIScrollLeftInRewardSelection())
			{
				_ttMan.ttDb.MoveRewardSelectionCurrentOptionLeft();
				SoundManager.instance.PlaySFX(0);
				_ttMan.ttUi.UpdateTextInInfoPanelRewardSelection();
			}

			else if ((Input.GetKeyDown(KeyCode.Space) ||
			         Input.GetButtonDown("Fire1")) && _ttMan.ttLogic.CanISelectInRewardSelection())
			{
				_ttMan.ttUi.FlipCurrentCardSelected();
			}
		}


		#endregion
	}
}
