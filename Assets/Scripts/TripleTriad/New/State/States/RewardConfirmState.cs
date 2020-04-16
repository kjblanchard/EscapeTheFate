using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public class RewardConfirmState : TtState
	{

		#region Configuration

		[SerializeField] private TripleTriadManager _ttMan;

		#endregion

		public override void Startup(int additionalArgs = 0)
		{
			_ttMan.ttUi.InitializeRewardConfirmUi();
		}

		public override void Execute()
		{
			ListenForUserInput();
			_ttMan.ttUi.KeepFingerOnProperLocationInRewardConfirm();
		}

		#region Functions

		private void ListenForUserInput()
		{
			if(Input.GetKeyDown(KeyCode.D) || Input.GetButtonDown("right"))
			{

				_ttMan.ttDb.MoveCursorLeftInRewardConfirm();
				SoundManager.instance.PlaySFX(0);

			}
			else if (Input.GetKeyDown(KeyCode.A) || Input.GetButtonDown("left"))
			{
				_ttMan.ttDb.MoveCursorRightInRewardConfirm();
				SoundManager.instance.PlaySFX(0);

			}
			else if ((Input.GetKeyDown(KeyCode.C) || Input.GetButtonDown("Fire2")) && !_ttMan.ttUi.isLoading)
			{
				SoundManager.instance.PlaySFX(1);
			}

			else if ((Input.GetKeyDown(KeyCode.Space) || Input.GetButtonDown("Fire1")) && !_ttMan.ttUi.isLoading)
			{
				SoundManager.instance.PlaySFX(6);
			}
		}

		#endregion
	}
}
