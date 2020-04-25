using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace ETF.TripleTriad
{

	public partial class TTUI
	{

		#region Configuration

		[Header("Opponent Selection Stuff")]
		[SerializeField] private Canvas _opponentSelectionCanvas;
		[SerializeField] private Image[] _opponentBoxImages;

		#endregion


		#region Functions

		public void InitializeOpponentSelectionUi()
		{
			_opponentSelectionCanvas.enabled = true;
		}
		
		public void TurnOnProperBoxImage()
		{//probably make this do the right ones not all of them in the future
			for (int i = 0; i < _opponentBoxImages.Length; i++)
			{
				_opponentBoxImages[i].enabled = false;
			}

			_opponentBoxImages[ttdb.RetrieveOpponentSelectionCurrentValue()].enabled = true;

		}

		#endregion
	}
}
