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
			base.Execute();
		}

		public override void End()
		{
			base.End();
		}

		#region Functions



		#endregion
	}
}
