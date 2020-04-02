using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Serialization;

namespace ETF.TripleTriad
{
    public partial class TTDB : MonoBehaviour
    {
        [Header("Card Battle References")] private int _numberofBattleCardsOnPage = 10;
        private int _totalNumberOfBattleCards;
        private int _totalNumberOfBattlePages;
        private int _totalNumberOfCardsOnLastBattlePage;
        public List<Card> currentBattleSelectableCards = new List<Card>();
        public List<int> currentBattleQuantityForCards = new List<int>();


        [SerializeField] TripleTriadManager ttMan;

        public void CalculateTtBattleNumbers()
        {
            //looks at your master card list, and figures out how many pages and remaining cards are needed
            _totalNumberOfBattleCards = CardInventory.instance.RetrieveTotalNumberOfCardsInMasterList();
            var notRoundedUpBattlePageCount = ((float) _totalNumberOfBattleCards) / _numberofBattleCardsOnPage;
            _totalNumberOfBattlePages = (int) Mathf.Ceil(notRoundedUpBattlePageCount);
            _totalNumberOfCardsOnLastBattlePage = (int) _totalNumberOfBattleCards % _numberofBattleCardsOnPage;
        }

        public int RetrieveTotalNumberOfCardsOnPage()
        {
            //returns the total number of battle pages
            return _numberofBattleCardsOnPage;
        }

        public int RetrieveTotalNumberOfBattleCards()
        {
            //returns the total number of battle pages
            return _totalNumberOfBattleCards;
        }

        public int RetrieveTotalNumberOfBattlePages()
        {
            //returns the total number of battle pages
            return _totalNumberOfBattlePages;
        }

        public int RetrieveTotalNumberOfCardsOnLastBattlePage()
        {
            //returns the total number of battle pages
            return _totalNumberOfCardsOnLastBattlePage;
        }

        public void ClearBattleSelectionsList()
        {
            //clears the current card choices List
            currentHandSelectionsList.Clear();
        }

        public void BringInUsableBattleCards()
        {
            //this will copy the current list of battle cards to our list of cards that can be used
            currentBattleSelectableCards = CardInventory.instance.RetrieveBattleCardList();
            currentBattleQuantityForCards = CardInventory.instance.RetrieveBattleQuantityList();
        }



    }
}
