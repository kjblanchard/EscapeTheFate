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
			base.Startup();
		}

		public override void Execute()
		{
			if (Input.GetKeyDown(KeyCode.W) || Input.GetButtonDown("up"))
			{
				ttMan.ttDb.MoveLocationFingerUpInDb();
			}
			else if (Input.GetKeyDown(KeyCode.S) || Input.GetButtonDown("down"))
			{
				ttMan.ttDb.MoveLocationFingerDownInDb();
			}
			else if (Input.GetKeyDown(KeyCode.D)|| Input.GetButtonDown("right"))
			{
				ttMan.ttDb.MoveLocationFingerRightInDb();
			}
			else if (Input.GetKeyDown(KeyCode.A)|| Input.GetButtonDown("left"))
			{
				ttMan.ttDb.MoveLocationFingerLeftInDb();
			}
			else if (Input.GetKeyDown(KeyCode.C))
			{
				//go back to card selection
			}
			else if (Input.GetKeyDown(KeyCode.Space)|| Input.GetButtonDown("Fire1"))
			{
				ttMan.ttDb.RetrieveTripleTriadCardInPlayerSelection().SetLocationToGoTo(ttMan.ttDb.RetrieveLocationSelectionCurrentSelection());
				ttMan.ttUi.PlayCardInLocationSelection();
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
