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


		#endregion



		#region Functions

		public void UpdateImageInTurnList(int spotToUpdate,Sprite pictureToChangeTo)
		{
			turnImages[spotToUpdate].sprite = pictureToChangeTo;

			tempTurnImagesFullDisplay[spotToUpdate].sprite = pictureToChangeTo;

		}
		
		public void UpdateImageInTurnMoveCalcList(int spotToUpdate,Sprite pictureToChangeTo)
		{
			

			tempTurnImagesFullDisplayMoveCalculation[spotToUpdate].sprite = pictureToChangeTo;

		}

		#endregion
	}
}
