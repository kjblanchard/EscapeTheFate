using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public class OpponentSelectState : TtState
	{

		#region Configuration

		[SerializeField] private TripleTriadManager _ttMan;
		private TTDB.MovementDirections whichWayToMove;
		

		#endregion

		public override void Startup(int additionalArgs = 0)
		{
			Initialize();
		}



		public override void Execute()
		{
			ListenForUserInput();
		}

		public override void End()
		{
			_ttMan.ttUi.TurnOffOpponentSelectUi();
		}


		#region Functions

		private void ListenForUserInput()
		{
			if (_ttMan.ttUi.isLoading) return;
			if (Input.GetKeyDown(KeyCode.D) || (Input.GetButtonDown("right")))
			{
				whichWayToMove = TTDB.MovementDirections.Right;
				if (_ttMan.ttLogic.CanIScrollInOpponentSelect(whichWayToMove))
				{
					_ttMan.ttDb.MoveOpponentSelectionInDb(whichWayToMove);
					TurnOnBoxImageAndAnimator();
				}
			}
			else if (Input.GetKeyDown(KeyCode.A) || (Input.GetButtonDown("left")))
			{
				whichWayToMove = TTDB.MovementDirections.Left;

				if (_ttMan.ttLogic.CanIScrollInOpponentSelect(whichWayToMove))
				{
					_ttMan.ttDb.MoveOpponentSelectionInDb(whichWayToMove);
					TurnOnBoxImageAndAnimator();
				}
			}
			else if (Input.GetKeyDown(KeyCode.W) || (Input.GetButtonDown("up")))
			{
				whichWayToMove = TTDB.MovementDirections.Up;
				if (_ttMan.ttLogic.CanIScrollInOpponentSelect(whichWayToMove))
				{
					_ttMan.ttDb.MoveOpponentSelectionInDb(whichWayToMove);
					TurnOnBoxImageAndAnimator();
				}
			}
			else if (Input.GetKeyDown(KeyCode.S) || (Input.GetButtonDown("down")))
			{
				whichWayToMove = TTDB.MovementDirections.Down;
				if (_ttMan.ttLogic.CanIScrollInOpponentSelect(whichWayToMove))
				{
					_ttMan.ttDb.MoveOpponentSelectionInDb(whichWayToMove);
					TurnOnBoxImageAndAnimator();
				}
			}
			else if (Input.GetKeyDown(KeyCode.Space) || (Input.GetButtonDown("Fire1")))
			{
				_ttMan.ttDb.SetCurrentOpponentYouArePlaying();
				TurnOnSelectedAnimator();
				_ttMan.ttDb.BringInCurrentSelectedEnemyInformationToDb();
				SoundManager.instance.PlaySFX(19);
			}
			else if (Input.GetKeyDown(KeyCode.C) || (Input.GetButtonDown("Fire2")))
			{
			}
		}
		
		private void Initialize()
		{
			if (Application.platform == RuntimePlatform.WebGLPlayer)
			{
				SoundManager.instance.PlayBgm(1);
			}
			else
			{
				SoundManager.instance.PlayIntroLoop(10);
				SoundManager.instance.CacheNextMusic(1);
			}

			_ttMan.ttUi.isLoading = false;
			_ttMan.ttDb.InitializeOpponentSelectionDb();
			TurnOnProperBoxImage();
			_ttMan.ttUi.InitializeOpponentSelectionUi();
		}

		
		private void TurnOnProperBoxImage()
		{
			_ttMan.ttUi.TurnOnProperBoxImage();
		}

		private void TurnOnProperAnimator()
		{
			_ttMan.ttUi.TurnOnProperAnimatorOpponentSelection();
		}

		private void UpdateOpponent()
		{
			_ttMan.ttUi.UpdateOpponentInfoOpponentSelection();
		}

		private void TurnOnBoxImageAndAnimator()
		{
			TurnOnProperBoxImage();
			TurnOnProperAnimator();
			UpdateOpponent();
			SoundManager.instance.PlaySFX(18);
		}

		private void TurnOnSelectedAnimator()
		{
			_ttMan.ttUi.SelectedAnimatorOpponentSelection();
		}

		#endregion
	}
}
