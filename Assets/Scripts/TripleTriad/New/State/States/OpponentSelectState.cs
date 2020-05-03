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
		private string[] _cheatCode;
		private int _index;

		#endregion

		public override void Startup(int additionalArgs = 0)
		{
			_ttui._tripleTriadBoardImage.enabled = false;
			Initialize();
			_cheatCode = new[] { "k", "b", "c", "a", "r", "d", "s" };
			_index = 0; 
		}
		

		
 
		void Update() {

			}



		public override void Execute()
		{
			ListenForUserInput();
			_ttMan.ttUi.KeepCursorOnProperSelectionOpponentSelect();
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
			}
			else if (Input.GetKeyDown(KeyCode.C) || (Input.GetButtonDown("Fire2")))
			{
			}
			// Check if any key is pressed
			if (Input.anyKeyDown)
			{
				// Check if the next key in the code is pressed
				if (Input.GetKeyDown(_cheatCode[_index]))
				{
					// Add 1 to index to check the next key in the code
					_index++;
				}
				// Wrong key entered, we reset code typing
				else
				{
					_index = 0;
				}
			}

			// If index reaches the length of the cheatCode string, 
			// the entire code was correctly entered
			if (_index == _cheatCode.Length)
			{
				SoundManager.instance.PlaySFX(20);
				_index = 0;
				_ttMan.ttUi.CheatCodeEntered();
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

			//_ttMan.ttUi.isLoading = false;
			_ttMan.ttDb.InitializeOpponentSelectionDb();
			//TurnOnProperBoxImage();
			_ttMan.ttUi.InitializeOpponentSelectionUi();
		}

		


		private void TurnOnProperAnimator()
		{
			_ttMan.ttUi.TurnOnProperAnimatorOpponentSelection();
		}

		private void UpdateOpponent()
		{
			_ttMan.ttUi.UpdateOpponentInfoOpponentSelection();
			SoundManager.instance.PlaySFX(18);

		}

		private void TurnOnBoxImageAndAnimator()
		{
			//TurnOnProperBoxImage();
			TurnOnProperAnimator();
			UpdateOpponent();
			
		}

		private void TurnOnSelectedAnimator()
		{
			_ttMan.ttUi.SelectedAnimatorOpponentSelection();
		}

		#endregion
	}
}
