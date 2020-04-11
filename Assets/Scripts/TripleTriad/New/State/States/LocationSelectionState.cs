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
			ListenForUserInputs();
			ttMan.ttUi.KeepFingerOnProperLocationInLocationSelection();
			print("inlocationselection");
		}



		public override void End()
		{
			base.End();
		}

		#region Functions

		private void ListenForUserInputs()
		{
			if (Input.GetKeyDown(KeyCode.W) || Input.GetButtonDown("up"))
			{
				if (!ttMan.ttLogic.CanIMoveUpInLocationSelection()) return;
				ttMan.ttDb.MoveLocationFingerUpInDb();
				SoundManager.instance.PlaySFX(0);

			}
			else if (Input.GetKeyDown(KeyCode.S) || Input.GetButtonDown("down"))
			{
				if (!ttMan.ttLogic.CanIMoveDownInLocationSelection()) return;
				ttMan.ttDb.MoveLocationFingerDownInDb();
				SoundManager.instance.PlaySFX(0);

			}
			else if (Input.GetKeyDown(KeyCode.D) || Input.GetButtonDown("right"))
			{
				if (!ttMan.ttLogic.CanIScrollRightInLocationSelection()) return;
				ttMan.ttDb.MoveLocationFingerRightInDb();
				SoundManager.instance.PlaySFX(0);

			}
			else if (Input.GetKeyDown(KeyCode.A) || Input.GetButtonDown("left"))
			{
				if (!ttMan.ttLogic.CanIScrollLeftInLocationSelection()) return;
				ttMan.ttDb.MoveLocationFingerLeftInDb();
				SoundManager.instance.PlaySFX(0);

			}
			else if ((Input.GetKeyDown(KeyCode.C) || Input.GetButtonDown("Fire2")) && !ttMan.ttUi.isLoading)
			{
				SoundManager.instance.PlaySFX(1);
				ttMan.ttDb.ModifyPlayerTurnIsCancelling(true);
				ttMan.SendChangeToPreviousState();
			}
			// else if (Input.GetKeyDown(KeyCode.Y))
			// {
			// 	ttMan.SwitchToEnemyTurn();
			// }
			else if ((Input.GetKeyDown(KeyCode.Space) || Input.GetButtonDown("Fire1"))  && !ttMan.ttUi.isLoading)
			{
				if (ttMan.ttLogic.CanIPlaceCardInLocationSelection())
				{
					//ttMan.ttUi.InitializeBetweenTurnUiPlayerToEnemy();
					ttMan.ttUi.keepPlayerSelectionFingerOnProperLocation();
					ttMan.ttDb.RetrieveTripleTriadCardInPlayerSelection()
						.SetLocationToGoTo(ttMan.ttDb.RetrieveLocationSelectionCurrentSelection());
					ttMan.ttDb.ChangeCurrentLocationCardToPlayed();
					ttMan.ttUi.PlayCardInLocationSelection();	
					SoundManager.instance.PlaySFX(6);
				}
				else
				{
					SoundManager.instance.PlaySFX(3);
				}


			}
		}


		#endregion
	}
}
