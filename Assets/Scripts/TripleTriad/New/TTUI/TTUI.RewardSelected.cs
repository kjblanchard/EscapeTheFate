﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace ETF.TripleTriad
{

	public partial class TTUI
	{

		#region Configuration
		[Header("RewardSelected Stuff")]
		[SerializeField] private Canvas _cardWonCanvas;
		[SerializeField] private Text _cardWonText;

		#endregion


		#region Functions

		public void InitializeRewardSelectedUi()
		{
			_rewardConfirmCanvas.enabled = false;
			_cardRewardCanvas.enabled = false;
		}

		public void TurnOnCardWonCanvas()
		{
			_cardWonCanvas.enabled = true;
		}

		#endregion
	}
}
