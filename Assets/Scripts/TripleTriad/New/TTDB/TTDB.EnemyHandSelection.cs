﻿using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

namespace ETF.TripleTriad
{
    public partial class TTDB
    {
        [Header("EnemyHandReferences")] private EnemyCardHand.WhatTypeOfCardPlayerAmI _typeOfEnemyPlayer;
        private List<Card> _currentEnemyRareCards;

        private List<Card> currentEnemyCardSelectionList = new List<Card>();
        public List<TripleTriadCardInHand> currentEnemyTripleTriadCardsInHand = new List<TripleTriadCardInHand>(); 
        public TripleTriadCardInHand[] fullEnemyTripleTriadCards;
        private string _whatToSayWhenTurnIsBeingSelected;
        private string _whatToSayWhenPlayerWins;
        private string _whatToSayWhenPlayerLoses;
        private string _whatToSayWhenChoosingCards;
        private string _npcName;
        private int _randomNumber;
        private int _chanceToUse;


        public void BringInEnemyCardInformation(EnemyCardHand.WhatTypeOfCardPlayerAmI typeOfEnemyPlayer,
            List<Card> currentEnemyRareCards, string turnSelectionText, string playerWinText, string playerLoseText,
            string choosingCardText, string npcName)
        {
            //this is used to get information from the npc, it is called by enemy hand script, which will be on every npc..  Gets his rare cards, type of difficulty, and Information
            _typeOfEnemyPlayer = typeOfEnemyPlayer;
            _currentEnemyRareCards = currentEnemyRareCards;
            _whatToSayWhenTurnIsBeingSelected = turnSelectionText;
            _whatToSayWhenPlayerWins = playerWinText;
            _whatToSayWhenPlayerLoses = playerLoseText;
            _whatToSayWhenChoosingCards = choosingCardText;
            _npcName = npcName;
        }


        public void GenerateEnemyCardHand()
        {
            //this is the switch for each type of enemy, currently only have beginner choices
            switch (_typeOfEnemyPlayer)
            {
                case EnemyCardHand.WhatTypeOfCardPlayerAmI.Beginner:
                    BeginnerChooseCards();
                    break;
                case EnemyCardHand.WhatTypeOfCardPlayerAmI.Moderate:
                    ModerateChooseCards();
                    break;
            }
        }

        private void BeginnerChooseCards()
        {
            //chooses cards for the beginner ruleset
            if (_currentEnemyRareCards.Count > 0)
            {
                DetermineIfRareCardsShouldBeAddedToHand();
            }

            BeginnerAddCardsToHandSelectionsUntilFull();
            AddEnemySelectedHandToTheEnemyCardHandGameObjects();
            AddEnemySelectedCardsToHandList();
        }
        private void ModerateChooseCards()
        {
            //chooses cards for the beginner ruleset
            if (_currentEnemyRareCards.Count > 0)
            {
                DetermineIfRareCardsShouldBeAddedToHand();
            }
            ModerateAddCardsToHandSelectionsUntilFull();
            AddEnemySelectedHandToTheEnemyCardHandGameObjects();
            AddEnemySelectedCardsToHandList();
        }


        private void DetermineIfRareCardsShouldBeAddedToHand()
        {
            //this determines if the enemy will use his rare card in his hand or not.
            for (var i = 0; i < _currentEnemyRareCards.Count; i++)
            {
                _randomNumber = Random.Range(0, 100);
                _chanceToUse = _currentEnemyRareCards[i].percentChanceForEnemyToUseInBattle;
                if (_randomNumber <= _chanceToUse)
                {
                    currentEnemyCardSelectionList.Add(_currentEnemyRareCards[i]);
                }
                
            }
        }

        private void BeginnerAddCardsToHandSelectionsUntilFull()
        {
            //this will choose a random card from the list of common cards until the enemy's hand is full
            while (currentEnemyCardSelectionList.Count < 5)
            {
                var randomNumber = Random.Range(0, CardInventory.instance.ttCardListOfCommonCards.Count);
                currentEnemyCardSelectionList.Add(CardInventory.instance.ttCardListOfCommonCards[randomNumber]);
            }
        }
        private void ModerateAddCardsToHandSelectionsUntilFull()
        {
            //this will choose a random card from the list of common cards until the enemy's hand is full
            while (currentEnemyCardSelectionList.Count < 5)
            {
                var randomNumber = Random.Range(0, CardInventory.instance.ttCardListOfUncommonCards.Count);
                currentEnemyCardSelectionList.Add(CardInventory.instance.ttCardListOfUncommonCards[randomNumber]);
            }
        }

        private void AddEnemySelectedHandToTheEnemyCardHandGameObjects()
        {
            //uses the information in the DB to set the card gameobject so that is has a reference to the card list that was chosen
            for (int i = 0; i < fullEnemyTripleTriadCards.Length; i++)
            {
                fullEnemyTripleTriadCards[i].SetMyCurrentCard(currentEnemyCardSelectionList[i]);
            }
        }

        private void AddEnemySelectedCardsToHandList()
        {
            for (int i = 0; i < fullEnemyTripleTriadCards.Length; i++)
            {
                currentEnemyTripleTriadCardsInHand.Add(fullEnemyTripleTriadCards[i]);
            }
        }




        public string WhatWillTheEnemySayWhenTurnSelection()
        {
            //returns data to the ui
            return _whatToSayWhenTurnIsBeingSelected;
        }

        public string WhatWillTheEnemySayWhenChoosingCards()
        {
            //returns data to the ui
            return _whatToSayWhenChoosingCards;
        }

        public string WhatIsTheEnemyNpcName()
        {
            //returns data to the ui
            return _npcName;
        }

        public EnemyCardHand.WhatTypeOfCardPlayerAmI RetrieveEnemyDifficulty()
        {
            return _typeOfEnemyPlayer;
        }
        
    }
}
