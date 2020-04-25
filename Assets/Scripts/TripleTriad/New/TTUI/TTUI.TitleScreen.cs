using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public partial class TTUI
	{

		#region Configuration
		[Header("Title Screen Stuff")]
		[SerializeField] private Canvas _titleScreenCanvas;

		#endregion



		#region Functions

		public void InitializeTitleScreenUi()
		{
			_titleScreenCanvas.enabled = true;
		}

		public void TurnOffTitleScreenUi()
		{
			_titleScreenCanvas.enabled = false;
		}


		#endregion
	}
}
