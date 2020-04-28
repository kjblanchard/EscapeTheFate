﻿using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
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

        public enum WhatTypeOfWinReward
        {
            One,
            Flipped,
            Random,
            All
        }

        [SerializeField] private WhatTypeOfCardPlayerAmI _whatTypeOfPlayerAmI;

        [SerializeField] private Card[] _myAvailableCardsToUse;
        [SerializeField] List<Card> _listOfRareCardsOwned = new List<Card>();
        [TextArea(4, 5)] [SerializeField] private string whatToSayWhenTurnIsBeingSelected = "Who goes first..";
        [TextArea(4, 5)] [SerializeField] private string whatToSayWhenPlayerWins = "You win chump";
        [TextArea(4, 5)] [SerializeField] private string whatToSayWhenPlayerLoses = "You lose sucker";
        [TextArea(4, 5)] [SerializeField] private string whatToSayWhenPlayerTies = "You lose sucker";
        [TextArea(4, 5)] [SerializeField] private string whatToSayWhenChoosingCards = "I'm Choosing da cards";
        [TextArea(4, 5)] [SerializeField] private string opponentDescription;
        [SerializeField] private string npcName = "Enemy Player";
        [SerializeField] private bool isEnemyHandOpen;
        [SerializeField] private bool isRandomCardSelection;
        [SerializeField] private bool isPlus;
        [SerializeField] private bool isSame;
        [SerializeField] private WhatTypeOfWinReward _typeOfWinReward;





        private void Awake()
        {//this is here for testing, will be moved to loading into triple triad afterwards

        }

        public void SendInfoToTtDb()
        {
            TripleTriadManager.instance.ttDb.BringInEnemyCardInformation(_whatTypeOfPlayerAmI, _listOfRareCardsOwned,
                whatToSayWhenTurnIsBeingSelected, /*whatToSayWhenPlayerWins, whatToSayWhenPlayerLoses,whatToSayWhenPlayerTies,*/
                whatToSayWhenChoosingCards, npcName, isEnemyHandOpen, isRandomCardSelection, isPlus, isSame,
                _typeOfWinReward,opponentDescription);
        }

        public string RetrieveName()
        {
            return npcName;
        }
        public string RetrieveDescription()
        {
            return opponentDescription;
        }
        public string RetrieveDifficulty()
        {
            return _whatTypeOfPlayerAmI.ToString();
        }
        public string RetrieveRares()
        {
            return _listOfRareCardsOwned.Count.ToString();
        }
        public string RetrieveRandom()
        {
            return isRandomCardSelection.ToString();
        }
        public string RetrieveOpen()
        {
            return isEnemyHandOpen.ToString();
        }
    }
}