﻿using System.Collections.Generic;
using UnityEngine;

public class CardInventory : MonoBehaviour
{
    [Header("Card List and Empty Card Reference")]
    public Card[] masterCardList;
    public List<Card> renumberedCardList = new List<Card>();
    private readonly List<Card> _ttBattleCardList = new List<Card>();
    public List<Card> ttCardListOfCommonCards = new List<Card>();
    public List<Card> ttCardListOfUncommonCards = new List<Card>();
    public List<Card> ttCardListOfRareCards = new List<Card>();
    public List<Card> ttCardListOfEliteCards = new List<Card>();
    private readonly List<int> _temporaryCardQuantityList = new List<int>();
    

    public Card emptyCard;

    [Header("Used for Your Usable Cards when entering Card Battle")]
    public string[] cardList;
    public int[] cardAmount;
    public string[] enemyCards;
    
    [SerializeField] int _numberOfCardsOnPage = 10;
    private int _totalNumberOfCards;
    private int _totalNumberOfPages;
    private int _totalNumberOfCardsOnLastPage;






    [Header("Singleton")]
    public static CardInventory instance;

    private void Awake()
    {//Singleton reference
        instance = this;
        int cardInventoryCount = FindObjectsOfType<CardInventory>().Length;
        if (cardInventoryCount > 1)
        {
            Destroy(gameObject);
        }
        else
        {
            DontDestroyOnLoad(gameObject);
        }
    }

    public void CalculateNumbers()
    {
        //looks at your master card list, and figures out how many pages and remaining cards are needed, these Values are referenced by other scripts
        _totalNumberOfCards = masterCardList.Length;
        var notRoundedUpPagesCount = ((float) _totalNumberOfCards) / _numberOfCardsOnPage;
        _totalNumberOfPages = (int) Mathf.Ceil(notRoundedUpPagesCount);
        _totalNumberOfCardsOnLastPage = _totalNumberOfCards % _numberOfCardsOnPage;
    }

    public void CreateUsableBattleCardLists()
    {
        _ttBattleCardList.Clear();
        _temporaryCardQuantityList.Clear();
        ttCardListOfCommonCards.Clear();
        
        //create lists of cards by type
        for (int i = 0; i < masterCardList.Length; i++)
        {
            ttCardListOfCommonCards.Add(masterCardList[i]);
            if (masterCardList[i].cardsRarity != Card.cardRarity.Common || masterCardList[i].amountOwned <= 0) continue;
            _ttBattleCardList.Add(masterCardList[i]);
            _temporaryCardQuantityList.Add(masterCardList[i].amountOwned);

        }
        
        for (int i = 0; i < masterCardList.Length; i++)
        {
            ttCardListOfUncommonCards.Add(masterCardList[i]);
            if (masterCardList[i].cardsRarity != Card.cardRarity.Uncommon ||
                masterCardList[i].amountOwned <= 0) continue;
            _ttBattleCardList.Add(masterCardList[i]);
            _temporaryCardQuantityList.Add(masterCardList[i].amountOwned);

        }
        
        for (int i = 0; i < masterCardList.Length; i++)
        {
            ttCardListOfRareCards.Add(masterCardList[i]);
            if (masterCardList[i].cardsRarity != Card.cardRarity.Rare || masterCardList[i].amountOwned <= 0) continue;
            _ttBattleCardList.Add(masterCardList[i]);
            _temporaryCardQuantityList.Add(masterCardList[i].amountOwned);

        }

        for (int i = 0; i < masterCardList.Length; i++)
        {
            ttCardListOfEliteCards.Add(masterCardList[i]);
            if (masterCardList[i].cardsRarity != Card.cardRarity.Elite || masterCardList[i].amountOwned <= 0) continue;
            _ttBattleCardList.Add(masterCardList[i]);
            _temporaryCardQuantityList.Add(masterCardList[i].amountOwned);

        }
    }

    public void AddCardToInventory(int cardId)
    {
        for (int i = 0; i < masterCardList.Length; i++)
        {
            if (masterCardList[i].cardID == cardId)
            {
                masterCardList[i].amountOwned++;
                break;
            } 
        }
    }

    public void RemoveCardFromInventory(int cardId)
    {
        for (int i = 0; i < masterCardList.Length; i++)
        {
            if (masterCardList[i].cardID == cardId)
            {
                masterCardList[i].amountOwned--;
                break;
            } 
        }
    }





    public int RetrieveNumberOfCardsOnPage()
    {//other functions use this to get the private variables
        return _numberOfCardsOnPage;
    }

    public int RetrieveTotalNumberOfCards()
    {//other functions use this to get the private variables
        return _totalNumberOfCards;
    }

    public int RetrieveTotalNumberOfPages()
    {//other functions use this to get the private variables
        return _totalNumberOfPages;
    }
    public int RetrieveTotalNumberOfCardsOnLastPage()
    {//other functions use this to get the private variables
        return _totalNumberOfCardsOnLastPage;
    }

    public int RetrieveTotalNumberOfCardsInMasterList()
    {
        return masterCardList.Length;
    }

    public List<Card> RetrieveBattleCardList()
    {
        return _ttBattleCardList;
    }

    public List<int> RetrieveBattleQuantityList()
    {
        return _temporaryCardQuantityList;
    }

    public int RetrieveTotalNumberOfBattleCards()
    {
        return _ttBattleCardList.Count;
    }

}
