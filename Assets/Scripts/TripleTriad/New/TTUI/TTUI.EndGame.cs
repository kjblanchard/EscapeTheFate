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
			locationSelectionFinger.SetActive(false);
			_endGameCanvas.gameObject.SetActive(true);
		}


		#endregion
	}
}
