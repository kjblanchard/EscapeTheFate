using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public class TripleTriadBoardLogic : MonoBehaviour
	{
	
		#region Configuration
		
		[SerializeField] private TripleTriadCardOnBoard[] _tripleTriadBoardCards;
		private int _adjacentCardLocationToCheckCached;
		private int _myCardValueCached;
		private int _enemyCardValueLocationToCheck;
		private int _enemyCardValueCached;
		
	
		#endregion



		#region Functions

		public void CheckToSeeIfCardsAreFlipping(int locationCardPlaced)
		{
			for (int i = 0; i < _tripleTriadBoardCards[locationCardPlaced].locationBoardAdjacency.Length; i++)
			{

				_adjacentCardLocationToCheckCached = _tripleTriadBoardCards[locationCardPlaced].locationBoardAdjacency[i];
				//print($"checking adjacency [i], which is location {_adjacentCardLocationToCheckCached} and is he in play?  {_tripleTriadBoardCards[_adjacentCardLocationToCheckCached].cardInPlay} ");
				if (_tripleTriadBoardCards[_adjacentCardLocationToCheckCached].cardInPlay)
				{

					_myCardValueCached = _tripleTriadBoardCards[locationCardPlaced].whatCardIAm
						.cardValues[_tripleTriadBoardCards[locationCardPlaced].myValueToCheck[i]];
					_enemyCardValueCached = _tripleTriadBoardCards[_adjacentCardLocationToCheckCached].whatCardIAm
						.cardValues[_tripleTriadBoardCards[locationCardPlaced].adjacencyValueToCheck[i]];
					//print($"my value is {_myCardValueCached} and his value is {_enemyCardValueCached} and he is at location {_adjacentCardLocationToCheckCached}");
					if (_myCardValueCached > _enemyCardValueCached)
					{
						//print($"flipping card");

						if (_tripleTriadBoardCards[locationCardPlaced].adjacencyValueToCheck[i] == 0)
						{
							_tripleTriadBoardCards[_adjacentCardLocationToCheckCached].BoardCardFlipUp();
						}
						else if (_tripleTriadBoardCards[locationCardPlaced].adjacencyValueToCheck[i] == 1)
						{
							_tripleTriadBoardCards[_adjacentCardLocationToCheckCached].BoardCardFlipRight();
							
						}
						else if (_tripleTriadBoardCards[locationCardPlaced].adjacencyValueToCheck[i] == 2)
						{
							_tripleTriadBoardCards[_adjacentCardLocationToCheckCached].BoardCardFlipDown();
						}
						else if(_tripleTriadBoardCards[locationCardPlaced].adjacencyValueToCheck[i] == 3)
						{
							_tripleTriadBoardCards[_adjacentCardLocationToCheckCached].BoardCardFlipLeft();
						}

					}
				}
			}
		}


		#endregion
	}
}
