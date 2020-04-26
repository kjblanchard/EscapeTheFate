using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public class OpponentSelectState : TtState
	{

		#region Configuration

		[SerializeField] private TripleTriadManager _ttMan;
		

		#endregion

		public override void Startup(int additionalArgs = 0)
		{
			SoundManager.instance.PlayIntroLoop(10);
			_ttMan.ttUi.isLoading = false;
			_ttMan.ttUi.InitializeOpponentSelectionUi();
			_ttMan.ttDb.InitializeOpponentSelectionDb();
		}

		public override void Execute()
		{
			ListenForUserInput();
		}


		#region Functions

		private void ListenForUserInput()
		{
			if (!_ttMan.ttUi.isLoading)	
			{
				if (Input.GetKeyDown(KeyCode.D) || (Input.GetButtonDown("right")))
				{
					_ttMan.ttDb.MoveOpponentSelectionInDb(TTDB.MovementDirections.Right);
					TurnOnBoxImageAndAnimator();
					
					SoundManager.instance.PlaySFX(18);
				}
				else if (Input.GetKeyDown(KeyCode.A) || (Input.GetButtonDown("left")))
				{
					_ttMan.ttDb.MoveOpponentSelectionInDb(TTDB.MovementDirections.Left);
					TurnOnBoxImageAndAnimator();
					SoundManager.instance.PlaySFX(18);
				}
				else if (Input.GetKeyDown(KeyCode.W) || (Input.GetButtonDown("up")))
				{
					_ttMan.ttDb.MoveOpponentSelectionInDb(TTDB.MovementDirections.Up);
					TurnOnBoxImageAndAnimator();
					SoundManager.instance.PlaySFX(18);
				}
				else if (Input.GetKeyDown(KeyCode.S) || (Input.GetButtonDown("down")))
				{
					_ttMan.ttDb.MoveOpponentSelectionInDb(TTDB.MovementDirections.Down);
					TurnOnBoxImageAndAnimator();
					SoundManager.instance.PlaySFX(18);
				}
				else if (Input.GetKeyDown(KeyCode.Space) || (Input.GetButtonDown("Fire1")))
				{
					TurnOnSelectedAnimator();
					SoundManager.instance.PlaySFX(19);
				}
				else if (Input.GetKeyDown(KeyCode.C) || (Input.GetButtonDown("Fire2")))
				{
				}
			}
		}

		private void TurnOnProperBoxImage()
		{
			_ttMan.ttUi.TurnOnProperBoxImage();
		}

		private void TurnOnProperAnimator()
		{
			_ttMan.ttUi.TurnOnProperAnimatorOpponentSelection();
		}

		private void TurnOnBoxImageAndAnimator()
		{
			TurnOnProperBoxImage();
			TurnOnProperAnimator();
		}

		private void TurnOnSelectedAnimator()
		{
			_ttMan.ttUi.SelectedAnimatorOpponentSelection();
		}

		#endregion
	}
}
