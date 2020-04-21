using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace ETF.Battle
{

	public class BattleUi : MonoBehaviour
	{

		#region Configuration
		[Header("Turn Order Image References")]
		public Image[] turnImages;
		public Image[] tempTurnImagesFullDisplay;
		public Image[] tempTurnImagesFullDisplayMoveCalculation;

		[Header("Turn Order TMP References")] 
		public TMPro.TMP_Text[] tempTurnTmpFullDisplay;
		public TMPro.TMP_Text[] tempTurnTmpFullDisplayMoveCalculation;
		


		#endregion



		#region Functions

		public void UpdateImageInTurnList(int spotToUpdate,Sprite pictureToChangeTo,int clockValue)
		{
			turnImages[spotToUpdate].sprite = pictureToChangeTo;

			tempTurnImagesFullDisplay[spotToUpdate].sprite = pictureToChangeTo;

			tempTurnTmpFullDisplay[spotToUpdate].text = clockValue.ToString();

		}
		
		public void UpdateImageInTurnMoveCalcList(int spotToUpdate,Sprite pictureToChangeTo, int clockValue)
		{
			

			tempTurnImagesFullDisplayMoveCalculation[spotToUpdate].sprite = pictureToChangeTo;
			tempTurnTmpFullDisplayMoveCalculation[spotToUpdate].text = clockValue.ToString();

		}

		#endregion
	}
}
