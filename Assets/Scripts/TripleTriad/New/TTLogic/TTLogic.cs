using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public partial class TTLogic : MonoBehaviour
{// this is the master class of ttlogic, as well as the card selection logic
    [SerializeField] TTUI ttUi;
    [SerializeField] TTDB ttDb;

    public bool CanIScrollOnCardSelect(bool isIncrementing, int currentPageNumber)
    {
        if (ttUi.isLoading) { return false; }
        if (isIncrementing)
        {
            return ttUi.listOfPages[currentPageNumber - 1].currentActiveCardsOnPage > 1;
        }
        return ttUi.listOfPages[currentPageNumber - 1].currentActiveCardsOnPage > 1;
    }

    public bool CanISwitchPages(bool isIncrementing, int currentPageNumber)
    {
        if (ttUi.isLoading) { return false; }
        if (isIncrementing)
        {
            return currentPageNumber < ttUi.listOfPages.Count;
        }
        return currentPageNumber > 1;
    }

    public bool CanSelectCardInCardSelection(int currentSpotInCardInv)
    {
        return ttDb.currentBattleQuantityForCards[currentSpotInCardInv] > 0;
    }

    public bool CanRemoveCardFromCardSelection()
    {
        if (ttDb.currentHandSelectionsList.Count > 0)
        {
            return true;
        }

        return false;
    }

    public bool AreYouGoingToLoopInCardSelection(int currentFingerPosition, int currentPageNumber,
        TTCardSelectionProcessor.WhichScrollDirection whichWayToCheck)
    {
        switch (whichWayToCheck)
        {
            case TTCardSelectionProcessor.WhichScrollDirection.IsIncrementing:
                if (currentFingerPosition >= ttUi.listOfPages[currentPageNumber - 1].currentActiveCardsOnPage - 1)
                {
                    return true;
                }
                else
                {
                    return false;
                }

                break;
            case TTCardSelectionProcessor.WhichScrollDirection.IsDecrementing:
                if (currentFingerPosition == 0)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            default:
                throw new ArgumentOutOfRangeException(nameof(whichWayToCheck), whichWayToCheck, null);
        }
    }


}
