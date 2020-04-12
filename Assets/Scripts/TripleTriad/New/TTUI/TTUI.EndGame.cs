using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public partial class TTUI
	{

		#region Configuration

		[Header("End Game Stuff")] [SerializeField]
		private Canvas _endGameCanvas;

		#endregion



		#region Functions

		public void InitializeEndGameUi()
		{
			SoundManager.instance.PlayIntroLoop(3);
			turnIndicatorFingerAnimator.gameObject.SetActive(false);

			ttMan.ttUi.enemyHandDialogBoxAnimator.gameObject.SetActive(true);
			UpdateEnemyDialogBoxToEndGameWin();
			PlayEnemyHandDialogBoxAnimation();

			_endGameCanvas.gameObject.SetActive(true);
			
		}
		
		
		private void UpdateEnemyDialogBoxToEndGameWin()
		{
			enemyDialogBoxDialogText.text = ttdb.WhatWillTheEnemySayWhenPlayerWins();
		}
		
		public void UpdateEnemyDialogBoxToEndGameLose()
		{
			enemyDialogBoxDialogText.text = ttdb.WhatWillTheEnemySayWhenPlayerLoses();
		}


		#endregion
	}
}
