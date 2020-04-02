using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public partial class TTUI
	{

		#region Configuration

		[Header("Location Selection")]
		
		[SerializeField] private Transform[] locationSelectionFingerLocation;

		[SerializeField] private GameObject locationSelectionFinger;

		#endregion

		#region Functions

		public void KeepFingerOnProperLocationInLocationSelection()
		{
			if (isLoading == false)
			{
				if (!locationSelectionFinger.gameObject.activeInHierarchy)
				{
                    
					locationSelectionFinger.gameObject.SetActive(true);
				}

				locationSelectionFinger.transform.position =
					locationSelectionFingerLocation[ttdb.RetrieveLocationSelectionCurrentSelection()].transform.position;
			}
			else
			{
				locationSelectionFinger.gameObject.SetActive(false);
			}
		}

		public void PlayCardInLocationSelection()
		{
			ttdb.RetrieveTripleTriadCardInPlayerSelection().CardPlayed();
		}

		public void PlayCardToBoardInLocationSelection(int boardLocation, Card cardToBecome)
		{
			ttdb.RetrieveTripleTriadCardInBoardSelection(boardLocation).SetMyCurrentCard(cardToBecome);
			ttdb.RetrieveTripleTriadCardInBoardSelection(boardLocation).ChangeImage();
			ttdb.RetrieveTripleTriadCardInBoardSelection(boardLocation).MoveToBoard();
		}
		

		#endregion
	}
}
