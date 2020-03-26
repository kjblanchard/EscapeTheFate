using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TTLogic : MonoBehaviour
{
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
    public bool CanIScrollOnCardConfirm(bool isIncrementing, int currentFingerPos)
    {
        if (ttUi.isLoading) { return false; }

        if (isIncrementing)
        {
            if(currentFingerPos < ttUi.ReturnCardConfirmFingerPosCount()-1)
            {
                return true;
            }
        }
        else
        {
            if (currentFingerPos > 0)
            {
                return true;
            }
        }
        return false;

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
        if (ttDb.currentCardSelectionsList.Count > 0)
        {
            return true;
        }

        return false;
    }

    public bool AreYouGoingToLoopInCardConfirm(int currentFingerPosition, int currentPageNumber,
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
