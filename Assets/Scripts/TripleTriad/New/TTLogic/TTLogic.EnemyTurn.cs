using System.Collections;
using System.Collections.Generic;
using ETF.TripleTriad;
using UnityEngine;

namespace ETF.TripleTriad
{

	public partial class TTLogic
	{


		#region Configuration

		private TripleTriadCardOnBoard _potentialEmptyTripleTriadSpotToPlay;
		private int _howManyCardsCanIFlipHere;
		private TripleTriadCardOnBoard _cardToCheckAgainst;
		private int _myCurrentValue;
		private int _enemySideToCheck;
		private int _enemyCurrentValue;
		private int _fourCardMoves;
		private int _threeCardMoves;
		private int _twoCardMoves;
		private int _oneCardMoves;
		private int _randomCardMoves;
		private int _cardInListToPlay;
		private int _boardLocInListToPlay;

		private int _cardInHandToPlay;
		private int _boardLocationToPlaceCard;


		#endregion
	

		#region Functions

		public void SearchEnemyHandForCardToPlay(Card enemyCard, int cardNumberInInventory)
		{//creates a list of all the spots on the board that he can flip a card

			//for each board location
			for (int i = 0; i < ttDb.RetrieveBoardLocationsCount(); i++)
			{
				if (!ttDb.RetrieveTripleTriadCardInBoardSelection(i).cardInPlay)
				{			//that is not in play
					//var potentialSpotToPlay = i;
					_potentialEmptyTripleTriadSpotToPlay = ttDb.RetrieveTripleTriadCardInBoardSelection(i);
					_howManyCardsCanIFlipHere = 0;
					for (int j = 0; j < _potentialEmptyTripleTriadSpotToPlay.locationBoardAdjacency.Length; j++)
					{			//search adjacent sides
						if (ttDb.RetrieveTripleTriadCardInBoardSelection((_potentialEmptyTripleTriadSpotToPlay.locationBoardAdjacency[j])).cardInPlay)
						{			//that are in play
							//var adjacentSideNumber = j;
							_cardToCheckAgainst = ttDb.RetrieveTripleTriadCardInBoardSelection(
								_potentialEmptyTripleTriadSpotToPlay
									.locationBoardAdjacency[j]);
							_myCurrentValue = enemyCard.cardValues[_potentialEmptyTripleTriadSpotToPlay.myValueToCheck[j]];
							_enemySideToCheck = _potentialEmptyTripleTriadSpotToPlay.adjacencyValueToCheck[j];
							_enemyCurrentValue = _cardToCheckAgainst.whatCardIAm.cardValues[_enemySideToCheck];
							if (_myCurrentValue > _enemyCurrentValue)
							{			//check to see if my value is higher than theirs
								//print($"My Value is higher than theirs with card {enemyCard.cardName}, and I'm going to go to position {i} cause my value is {_myCurrentValue} and their value is {_enemyCurrentValue}");
								ttDb.AddBoardLocationToPotentialOneCardMoves(i);
								ttDb.AddCardNumberToPotentialOneCardMoves(cardNumberInInventory);
								_howManyCardsCanIFlipHere++;
								if (_howManyCardsCanIFlipHere > 1)
								{
									switch(_howManyCardsCanIFlipHere)
									{
										case 2:
											//print($"I can flip 2 cards at position {i} with my card {enemyCard.name} and he is in spot {cardNumberInInventory} in my card hand");
											ttDb.AddBoardLocationToPotentialTwoCardMoves(i);
											ttDb.AddCardNumberToPotentialTwoCardMoves(cardNumberInInventory);
											break;
										case 3:
											//print($"I can flip 3 cards at position {i} with my card {enemyCard.name} and he is in spot {cardNumberInInventory} in my card hand");
											ttDb.AddBoardLocationToPotentialThreeCardMoves(i);
											ttDb.AddCardNumberToPotentialThreeCardMoves(cardNumberInInventory);
											break;
										case 4:
											//print($"I can flip 4 cards at position {i} with my card {enemyCard.name} and he is in spot {cardNumberInInventory} in my card hand");
											ttDb.AddBoardLocationToPotentialFourCardMoves(i);
											ttDb.AddCardNumberToPotentialFourCardMoves(cardNumberInInventory);
											break;
									}
								}
							}
							// else
							// {
							// 	print($"Found a spot at {potentialSpotToPlay}, but I can't flip it cause my value is {myCurrentValue} and their value is {enemyCurrentValue}");
							// }
						}
					}
				}
			}
			//play card at this location below..
			ChooseCardToPlayFromListOfChoices();
			
		}

		private void SearchForRandomCardPlacement()
		{
			for (int i = 0; i < ttDb.RetrieveBoardLocationsCount(); i++)
			{
				if (!ttDb.RetrieveTripleTriadCardInBoardSelection(i).cardInPlay)
				{
					ttDb.AddBoardLocationToPotentialRandomCardMoves(i);
				}
			}
		}
		
		public void FullEnemyTurnHandChoices()
		{
			for (int i = 0; i < ttDb.currentEnemyTripleTriadCardsInHand.Count; i++)
			{
				SearchEnemyHandForCardToPlay(ttDb.currentEnemyTripleTriadCardsInHand[i].whatCardIAm,i);
					
			}
		}

		private void ChooseCardToPlayFromListOfChoices()
		{
			var enemyDifficulty = ttDb.RetrieveEnemyDifficulty();
			switch(enemyDifficulty)
			{
				case EnemyCardHand.WhatTypeOfCardPlayerAmI.Beginner:
					ChooseCardMoveAndReturnCardToPlayInHandBeginner();
					break;
				case EnemyCardHand.WhatTypeOfCardPlayerAmI.Moderate:
					break;
				case EnemyCardHand.WhatTypeOfCardPlayerAmI.Hard:
					break;
				case EnemyCardHand.WhatTypeOfCardPlayerAmI.ChooseAllMyCards:
					break;
			}
		}

		private void ChooseCardMoveAndReturnCardToPlayInHandBeginner()
		{
			GatherPotentialMoveCountsFromDb();
			
			if (_fourCardMoves > 0)
			{

				_cardInListToPlay = Random.Range(0, _fourCardMoves);
				_cardInHandToPlay =  ttDb.FindCardToPlayInFourCardList(_cardInListToPlay);
				_boardLocationToPlaceCard = ttDb.FindBoardLocationToPlayInFourCardList(_cardInListToPlay);

			}
			else if (_threeCardMoves > 0)
			{
				_cardInListToPlay = Random.Range(0, _threeCardMoves);
				_cardInHandToPlay =  ttDb.FindCardToPlayInThreeCardList(_cardInListToPlay);
				_boardLocationToPlaceCard = ttDb.FindBoardLocationToPlayInThreeCardList(_cardInListToPlay);

			}
			else if (_twoCardMoves > 0)
			{
				_cardInListToPlay = Random.Range(0, _twoCardMoves);
				_cardInHandToPlay =  ttDb.FindCardToPlayInTwoCardList(_cardInListToPlay);
				_boardLocationToPlaceCard = ttDb.FindBoardLocationToPlayInTwoCardList(_cardInListToPlay);

			}
			else if (_oneCardMoves > 0)
			{
				_cardInListToPlay = Random.Range(0, _oneCardMoves);
				_cardInHandToPlay =  ttDb.FindCardToPlayInOneCardList(_cardInListToPlay);
				_boardLocationToPlaceCard = ttDb.FindBoardLocationToPlayInOneCardList(_cardInListToPlay);

			}
			else
			{
				print("noCardsCanBeFlipped");
				SearchForRandomCardPlacement();
				_randomCardMoves = ttDb.RetrieveNumberOfPotentialMovesRandomCards();
				_boardLocInListToPlay = Random.Range(0, _randomCardMoves);
				_boardLocationToPlaceCard =  ttDb.FindBoardLocationToPlayInRandomCardList(_boardLocInListToPlay);
				_cardInHandToPlay = Random.Range(0, ttDb.currentEnemyTripleTriadCardsInHand.Count);
			}
		}

		private void GatherPotentialMoveCountsFromDb()
		{
			_fourCardMoves = ttDb.RetrieveNumberOfPotentialMovesFourCards();
			_threeCardMoves = ttDb.RetrieveNumberOfPotentialMovesThreeCards();
			_twoCardMoves = ttDb.RetrieveNumberOfPotentialMovesTwoCards();
			_oneCardMoves = ttDb.RetrieveNumberOfPotentialMovesOneCard();

		}

		public int RetrieveBoardPositionToPlaceCard()
		{
			return _boardLocationToPlaceCard;
		}

		public int RetrieveHandPositionToPlaceCard()
		{
			return _cardInHandToPlay;
		}
		
		#endregion
	}
}
