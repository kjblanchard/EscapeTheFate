using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyCardHand : MonoBehaviour
{
    
    public enum WhatTypeOfCardPlayerAmI
    {
        ChooseAllMyCards,
        Beginner,
        Moderate,
        Hard,
    }

    [SerializeField] private WhatTypeOfCardPlayerAmI _whatTypeOfPlayerAmI;
    
    [SerializeField] private Card[] _myAvailableCardsToUse;
    [SerializeField] List<Card> _listOfRareCardsOwned = new List<Card>();


    private void Start()
    {
        TripleTriadManager.instance.ttDb.BringInEnemyCardInformation(_whatTypeOfPlayerAmI,_listOfRareCardsOwned);
    }
}
