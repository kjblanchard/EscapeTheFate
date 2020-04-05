using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public class LocationSelectionState : TtState
	{

		#region Configuration

		[SerializeField] private TripleTriadManager ttMan;
		

		#endregion

		public override void Startup()
		{
			ttMan.ttDb.InitializeLocationSelectionDbValues();
			ttMan.ttUi.InitializeLocationSelectionUiElements();


		}

		public override void Execute()
		{
			if (Input.GetKeyDown(KeyCode.W) || Input.GetButtonDown("up"))
			{
				ttMan.ttDb.MoveLocationFingerUpInDb();
				SoundManager.instance.PlaySFX(0);
			}
			else if (Input.GetKeyDown(KeyCode.S) || Input.GetButtonDown("down"))
			{
				ttMan.ttDb.MoveLocationFingerDownInDb();
				SoundManager.instance.PlaySFX(0);
			}
			else if (Input.GetKeyDown(KeyCode.D)|| Input.GetButtonDown("right"))
			{
				ttMan.ttDb.MoveLocationFingerRightInDb();
				SoundManager.instance.PlaySFX(0);
			}
			else if (Input.GetKeyDown(KeyCode.A)|| Input.GetButtonDown("left"))
			{
				ttMan.ttDb.MoveLocationFingerLeftInDb();
				SoundManager.instance.PlaySFX(0);
			}
			else if (Input.GetKeyDown(KeyCode.C))
			{
				ttMan.ttDb.ModifyPlayerTurnIsCancelling(true);
				ttMan.SendChangeToPreviousState();
			}
			else if (Input.GetKeyDown(KeyCode.Space)|| Input.GetButtonDown("Fire1"))
			{
				ttMan.ttDb.RetrieveTripleTriadCardInPlayerSelection().SetLocationToGoTo(ttMan.ttDb.RetrieveLocationSelectionCurrentSelection());
				//ttMan.ttDb.RetrieveTripleTriadCardInPlayerSelection().SetTtCardToHavePlayed();
				//ttMan.ttDb.RemoveOneFromCurrentHand();
				ttMan.ttDb.ChangeCurrentLocationCardToPlayed();
				ttMan.ttUi.PlayCardInLocationSelection();
				
				SoundManager.instance.PlaySFX(6);
			}
			ttMan.ttUi.KeepFingerOnProperLocationInLocationSelection();
		}

		public override void End()
		{
			base.End();
		}

		#region Functions



		#endregion
	}
}
