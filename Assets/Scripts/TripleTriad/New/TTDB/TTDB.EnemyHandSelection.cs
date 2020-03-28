using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public partial class TTDB
{
    [Header("EnemyHandReferences")] 
    private EnemyCardHand.WhatTypeOfCardPlayerAmI _typeOfEnemyPlayer;
    private List<Card> _currentEnemyRareCards;
    public List<Card> currentEnemyHandList = new List<Card>();
    public TripleTriadCard[] currentEnemyTripleTriadCards;
    
    
    public void BringInEnemyCardInformation(EnemyCardHand.WhatTypeOfCardPlayerAmI typeOfEnemyPlayer,List<Card> currentEnemyRareCards)
    {
        _typeOfEnemyPlayer = typeOfEnemyPlayer;
        _currentEnemyRareCards = currentEnemyRareCards;
    }

    public void GenerateEnemyCardHand()
    {
        switch (_typeOfEnemyPlayer)
        {
            case EnemyCardHand.WhatTypeOfCardPlayerAmI.Beginner:
                BeginnerChooseCards();
                break;
        }
    }

    private void BeginnerChooseCards()
    {
        if (_currentEnemyRareCards.Count > 0)
        {
            DetermineIfRareCardsShouldBeAddedToHand();
        }
        BeginnerAddCardsToHandUntilFull();
        AddEnemySelectedHandToTheEnemyGameObjects();
    }
    

    private void DetermineIfRareCardsShouldBeAddedToHand()
    {
        for (var i = 0; i < _currentEnemyRareCards.Count; i++)
        {
            var randomNumber = Random.Range(0, 100);
            var chanceToUse = _currentEnemyRareCards[i].percentChanceForEnemyToUseInBattle;
            if (randomNumber <= chanceToUse)
            {
                currentEnemyHandList.Add(_currentEnemyRareCards[i]);
            }
        }
    }
    
    private void BeginnerAddCardsToHandUntilFull()
    {
        while (currentEnemyHandList.Count < 5)
        {
           var randomNumber = Random.Range(0, CardInventory.instance.ttCardListOfCommonCards.Count);
          currentEnemyHandList.Add(CardInventory.instance.ttCardListOfCommonCards[randomNumber]);
        }
    }

    private void AddEnemySelectedHandToTheEnemyGameObjects()
    {
        for (int i = 0; i < currentEnemyTripleTriadCards.Length; i++)
        {
            currentEnemyTripleTriadCards[i].SetMyCurrentCard(currentEnemyHandList[i]);
        }
    }
}
