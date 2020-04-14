using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace ETF.TripleTriad
{

	public partial class TTUI
	{

		#region Configuration

		[Header("End Game Stuff")] [SerializeField]
		private Canvas _endGameCanvas;

		[SerializeField] private Animator _endGameAnimator;
		[SerializeField] private Text _endGameText;




		#endregion



		#region Functions

		public void InitializeEndGameInitialUi()
		{
			turnIndicatorFingerAnimator.gameObject.SetActive(false);
			_endGameCanvas.gameObject.SetActive(true);
		}
		
		
		public void InitializeWinEndGameUi()
		{
			SoundManager.instance.PlayIntroLoop(3);
			UpdateEnemyDialogBoxToEndGameWin();
			ttMan.ttUi.enemyHandDialogBoxAnimator.gameObject.SetActive(true);
			PlayEnemyHandDialogBoxAnimation();
			_endGameText.text = "You Win!";
			_endGameAnimator.SetTrigger("win");

		}
		public void InitializeTieEndGameUi()
		{
			UpdateEnemyDialogBoxToEndGameTie();
			ttMan.ttUi.enemyHandDialogBoxAnimator.gameObject.SetActive(true);
			PlayEnemyHandDialogBoxAnimation();
			_endGameText.text = "Tie..";
			_endGameAnimator.SetTrigger("tie");

		}
		public void InitializeLoseEndGameUi()
		{
			SoundManager.instance.PlaySFX(16);
			UpdateEnemyDialogBoxToEndGameLose();
			ttMan.ttUi.enemyHandDialogBoxAnimator.gameObject.SetActive(true);
			PlayEnemyHandDialogBoxAnimation();
			_endGameText.text = "Loser!";
			_endGameAnimator.SetTrigger("lose");

		}
		
		
		
		private void UpdateEnemyDialogBoxToEndGameWin()
		{
			enemyDialogBoxDialogText.text = ttdb.WhatWillTheEnemySayWhenPlayerWins();
		}
		
		public void UpdateEnemyDialogBoxToEndGameLose()
		{
			enemyDialogBoxDialogText.text = ttdb.WhatWillTheEnemySayWhenPlayerLoses();
		}
		
		public void UpdateEnemyDialogBoxToEndGameTie()
		{
			enemyDialogBoxDialogText.text = ttdb.WhatWillTheEnemySayWhenPlayerTies();
		}


		#endregion
	}
}
