using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF
{
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
        [TextArea(4, 5)] [SerializeField] private string whatToSayWhenTurnIsBeingSelected = "Who goes first..";
        [TextArea(4, 5)] [SerializeField] private string whatToSayWhenPlayerWins = "You win chump";
        [TextArea(4, 5)] [SerializeField] private string whatToSayWhenPlayerLoses = "You lose sucker";
        [TextArea(4, 5)] [SerializeField] private string whatToSayWhenChoosingCards = "I'm Choosing da cards";
        [SerializeField] private string npcName = "Enemy Player";


        private void Start()
        {
            TripleTriadManager.instance.ttDb.BringInEnemyCardInformation(_whatTypeOfPlayerAmI, _listOfRareCardsOwned,
                whatToSayWhenTurnIsBeingSelected, whatToSayWhenPlayerWins, whatToSayWhenPlayerLoses,
                whatToSayWhenChoosingCards, npcName);
        }
    }

}