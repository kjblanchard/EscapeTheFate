using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF
{
    public partial class TTDB
    {
        [Header("Card Selection Cursor Memory")] [SerializeField]
        int _fingerLocationOnCurrentPage;

        [SerializeField] int _currentPageNumber;
        [SerializeField] int _currentSpotInCardInventory;

        [Header("Card Selection References")]
        public List<LastItemChosen> currentHandSelectionsList = new List<LastItemChosen>();

        [SerializeField] private TripleTriadCard[] _myHandTripleTriadCards;

        public int RetrieveCardSelectionFingerLocationOnCurrentPage()
        {
            return _fingerLocationOnCurrentPage;
        }

        public int RetrieveCardSelectionCurrentPageNumber()
        {
            return _currentPageNumber;
        }

        public int RetrieveCardSelectionCurrentSpotInInventory()
        {
            return _currentSpotInCardInventory;
        }

        public void InitializeCardSelectionValuesInDB()
        {
            _fingerLocationOnCurrentPage = 0;
            _currentPageNumber = 0;
            _currentSpotInCardInventory = 0;
        }

        public void UpdateCardSelectionFingerPositionForScrollingInDb(
            CardSelectionState.WhichUpDownDirection whichUpDownDirection)
        {
            if (whichUpDownDirection == CardSelectionState.WhichUpDownDirection.IsMovingDown)
            {
                _fingerLocationOnCurrentPage++;
                _currentSpotInCardInventory++;
            }
            else if (whichUpDownDirection == CardSelectionState.WhichUpDownDirection.IsMovingUp)
            {
                _fingerLocationOnCurrentPage--;
                _currentSpotInCardInventory--;
            }
        }

        public void UpdateCardSelectionFingerPositionForPageScrollingInDb(
            CardSelectionState.PageScrollDirections whichPageScrollDirection)
        {
            if (whichPageScrollDirection == CardSelectionState.PageScrollDirections.GoingForward)
            {
                _currentPageNumber++;
                _currentSpotInCardInventory += 10;
            }
            else if (whichPageScrollDirection == CardSelectionState.PageScrollDirections.GoingBackward)
            {
                _currentPageNumber--;
                _currentSpotInCardInventory -= 10;
            }
        }

        public void UpdateCardSelectionFingerPositionForLoopingInDb(
            CardSelectionState.WhichUpDownDirection whichUpDownDirection)
        {
            switch (whichUpDownDirection)
            {
                case CardSelectionState.WhichUpDownDirection.IsMovingDown:
                    _fingerLocationOnCurrentPage = 0;
                    _currentSpotInCardInventory -= ttMan.ttUi.RetrieveNumberOfCardsActiveOnPage(_currentPageNumber) - 1;
                    break;
                case CardSelectionState.WhichUpDownDirection.IsMovingUp:
                    _fingerLocationOnCurrentPage = ttMan.ttUi.RetrieveNumberOfCardsActiveOnPage(_currentPageNumber) - 1;
                    _currentSpotInCardInventory = _currentSpotInCardInventory +
                        ttMan.ttUi.RetrieveNumberOfCardsActiveOnPage(_currentPageNumber) - 1;
                    break;
            }
        }

        public void UpdateCardSelectionInfoOnScrollingPastEndOfPage()
        {
            _fingerLocationOnCurrentPage = ttMan.ttUi.listOfPages[_currentPageNumber].currentActiveCardsOnPage - 1;
            _currentSpotInCardInventory = RetrieveTotalNumberOfBattleCards() - 1;
        }

        public void UpdateMyHandTripleTriadCardsWithMySelectionList()
        {
            for (int i = 0; i < currentHandSelectionsList.Count; i++)
            {
                _myHandTripleTriadCards[i].whatCardIAm =
                    currentBattleSelectableCards[currentHandSelectionsList[i].spotInCardInv];
                _myHandTripleTriadCards[i].SetTtCardToHaveNotPlayed();
            }
        }

        public int RetrieveTripleTriadCardLength()
        {
            return _myHandTripleTriadCards.Length;
        }


    }
}

