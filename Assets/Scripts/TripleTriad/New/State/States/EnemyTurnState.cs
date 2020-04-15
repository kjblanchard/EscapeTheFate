using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public class EnemyTurnState : TtState
	{

		#region Configuration

		[SerializeField] private TripleTriadManager ttMan;
		[SerializeField] private WaitForSeconds inFocusWait = new WaitForSeconds(0.4f);
		#endregion

		public override void Startup(int additionalArgs = 0)
		{
			StartCoroutine(CycleThroughCardsAnimation());
		}

		public override void Execute()
		{
			//print("in enemy Turn");
		}

		#region Functions

		private IEnumerator CycleThroughCardsAnimation()
		{
			ttMan.ttDb.InitializeEnemyTurnDbValues();
			
			for (int i = 0; i < ttMan.ttDb.currentEnemyTripleTriadCardsInHand.Count; i++)
			{
				ttMan.ttDb.currentEnemyTripleTriadCardsInHand[i].InFocus();
				yield return inFocusWait;
				ttMan.ttDb.currentEnemyTripleTriadCardsInHand[i].OutFocus();
				//yield return inFocusWait;

			}
			
			ttMan.ttLogic.FullEnemyTurnHandChoices();
			
			
			var boardLocation = ttMan.ttLogic.RetrieveBoardPositionToPlaceCard();
			var handPosition = ttMan.ttLogic.RetrieveHandPositionToPlaceCard();
			//print($"the spot I'm going to put the card is {boardLocation} and I'm going to use this spot in my hand {handPosition}, which is card {ttMan.ttDb.currentEnemyTripleTriadCardsInHand[handPosition].whatCardIAm.name}");

			ttMan.ttDb.currentEnemyTripleTriadCardsInHand[handPosition].SetLocationToGoTo(boardLocation);
			ttMan.ttDb.ChangeBoardLocationToInPlayEnemy(boardLocation);
			ttMan.ttUi.setFocusOnEnemyCardSelected();
			ttMan.ttUi.PlayCardInEnemyTurn(handPosition);
                
		}


		#endregion
	}
}
