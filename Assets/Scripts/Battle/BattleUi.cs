using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace ETF.Battle
{

	public class BattleUi : MonoBehaviour
	{

		#region Configuration

		public Image[] turnImages;

		public Image[] tempTurnImagesFullDisplay;


		#endregion



		#region Functions

		public void UpdateImageInTurnList(int spotToUpdate,Sprite pictureToChangeTo)
		{
			turnImages[spotToUpdate].sprite = pictureToChangeTo;

			tempTurnImagesFullDisplay[spotToUpdate].sprite = pictureToChangeTo;

		}

		#endregion
	}
}
