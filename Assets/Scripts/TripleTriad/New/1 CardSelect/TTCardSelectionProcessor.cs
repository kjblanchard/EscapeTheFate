using System;
using System.Collections;
using System.Diagnostics;
using UnityEngine;

public class TTCardSelectionProcessor : MonoBehaviour
{

    [Header("CardUI Script Reference")]
    [SerializeField] TTUI ttUi;
    [SerializeField] private TTDB ttDb;
    [SerializeField] private TTLogic ttLogic;

    [Header("PageScrolling Logic")]
    private PageScrollDirections _currentPageScrollDirection;
    private enum PageScrollDirections
    {
        GoingForward,
        GoingBackward,
    }
    [Header("CoroutineHolderForReference")]
    Coroutine _continuousCoroutineReference;

    [Header("ScrollSpeedControllersForUp-Down")]
    [SerializeField] float autoScrollSpeed = 1.0f;
    [SerializeField] bool regularAutoScrollSpeed = true;
    [SerializeField] float fastAutoScrollSpeed = 0.10f;

    [Header("Cursor Memory")]
    [SerializeField] int fingerLocationOnCurrentPage;
    [SerializeField] int currentPageNumber;
    [SerializeField] int currentSpotInCardInventory;
    
    // [Header("Initializing and updating")]
    // public bool needsToInitialize;
    // public  bool needToUpdateCursor;

    private enum ModifyingCardQuantity
    {
        IsAddingCard,
        IsRemovingCard,
    }
    public enum WhichScrollDirection
    {
        IsIncrementing,
        IsDecrementing,
    }
    private enum WhichWayToLoop
    {
        LoopToTop,
        LoopToBottom,
    }

    public void TtCardSelectProcessor()
    {
        if (Input.GetKeyDown(KeyCode.D) || (Input.GetButtonDown("right")))
        {
            if (_continuousCoroutineReference != null) { StopTheCoroutineScrolling(); }
            _continuousCoroutineReference = StartCoroutine(PageScrollContinuously(PageScrollDirections.GoingForward));
        }
        else if (Input.GetKeyDown(KeyCode.A)|| (Input.GetButtonDown("left")))
        {
            if (_continuousCoroutineReference != null) { StopTheCoroutineScrolling(); }
            _continuousCoroutineReference = StartCoroutine(PageScrollContinuously(PageScrollDirections.GoingBackward));

        }
        else if (Input.GetKeyDown(KeyCode.W)|| (Input.GetButtonDown("up")))
        {
            if (_continuousCoroutineReference != null) { StopTheCoroutineScrolling(); }
            _continuousCoroutineReference = StartCoroutine(ScrollContinuously(WhichScrollDirection.IsDecrementing));


        }
        else if (Input.GetKeyDown(KeyCode.S)|| (Input.GetButtonDown("down")))
        {
            if (_continuousCoroutineReference != null) { StopTheCoroutineScrolling(); }
            _continuousCoroutineReference = StartCoroutine(ScrollContinuously(WhichScrollDirection.IsIncrementing));
        }
        else if (Input.GetKeyDown(KeyCode.Space)|| (Input.GetButtonDown("Fire1")))
        {
            CardSelection(currentSpotInCardInventory);
        }
        else if (Input.GetKeyDown(KeyCode.C)|| (Input.GetButtonDown("Fire2")))
        {
            CancelLastSelection();

        }

        if (Input.GetKeyUp(KeyCode.D) || (Input.GetButtonUp("right")))
        {
            StopCoroutine(_continuousCoroutineReference);
        }
        else if (Input.GetKeyUp(KeyCode.A)|| (Input.GetButtonUp("left")))
        {
            StopCoroutine(_continuousCoroutineReference);
        }
        else if (Input.GetKeyUp(KeyCode.W)|| (Input.GetButtonUp("up")))
        {
            StopCoroutine(_continuousCoroutineReference);
        }
        else if (Input.GetKeyUp(KeyCode.S)|| (Input.GetButtonUp("down")))
        {
            StopCoroutine(_continuousCoroutineReference);
        }

        ttUi.KeepFingerOnProperLocationInCardSelection(currentPageNumber, fingerLocationOnCurrentPage);

    }

    public void InitializeCardSelectionFromRulesScreen()
    {//initializes the cardselectionscreen from the main screen
        ttUi.ResetScrollRectPosition();
        ttUi.GeneratePages();
        CardInventory.instance.CreateUsableBattleCardLists();//this only needs to be ran once, it generates all of the lists for card usage
        ttDb.BringInUsableBattleCards();
        ttUi.InitializeUiFromTtBattleList();
        currentPageNumber = 1;
        ttUi.UpdatePageNum(currentPageNumber);
        currentSpotInCardInventory = 0;
        fingerLocationOnCurrentPage = 0;
        ttUi.UpdateCardDisplayUI(0);
        ttDb.ClearBattleSelectionsList();
    }
    private void ChangePage(PageScrollDirections whichWayToScroll)
    {//uses ttlogic to figure out if you can page switch, and if you can then move the cursor and start the animation and sound
        bool canPageSwitch;
        var totalNumberOfPages = ttDb.RetrieveTotalNumberOfBattlePages();

        switch (whichWayToScroll)
        {
            case PageScrollDirections.GoingForward:
                
                canPageSwitch =ttLogic.CanISwitchPages(true,currentPageNumber);
                 if (canPageSwitch)
                 {
                    StartCoroutine(ttUi.MoveForwardScrollAnimation(totalNumberOfPages));
                    MoveCursorToNextPage(whichWayToScroll);
                    SoundManager.instance.PlaySFX(6);

                 }
                break;
            case PageScrollDirections.GoingBackward:
                canPageSwitch = ttLogic.CanISwitchPages(false,currentPageNumber);
                if (canPageSwitch)
                {
                    StartCoroutine(ttUi.MoveBackwardScrollAnimation(totalNumberOfPages));
                    MoveCursorToNextPage(whichWayToScroll);
                    SoundManager.instance.PlaySFX(6);
                }
                break;
            default:
                throw new ArgumentOutOfRangeException(nameof(whichWayToScroll), whichWayToScroll, null);
        }
    }
    private void MoveCursorUpDownInMenu(WhichScrollDirection whichWayToScroll)
    {
        bool canScroll;
        switch (whichWayToScroll)
        {
            case WhichScrollDirection.IsIncrementing:
                canScroll = ttLogic.CanIScrollOnCardSelect(true, currentPageNumber);
                if (!canScroll) return;
                var isGoingToLoopInMenu =
                    ttLogic.AreYouGoingToLoopInCardSelection(fingerLocationOnCurrentPage, currentPageNumber,
                        whichWayToScroll);
                if (isGoingToLoopInMenu)
                {
                    LoopInCurrentMenu(WhichWayToLoop.LoopToTop);
                }
                else
                {
                    MoveCursorOneSpotInInventory(whichWayToScroll);
                }

                SoundManager.instance.PlaySFX(0);
                ttUi.UpdateCardDisplayUI(currentSpotInCardInventory);
                
                break;
            case WhichScrollDirection.IsDecrementing:

                canScroll = ttLogic.CanIScrollOnCardSelect(false, currentPageNumber);
                if (!canScroll) return;
                if (fingerLocationOnCurrentPage == 0)
                {
                    LoopInCurrentMenu(WhichWayToLoop.LoopToBottom);
                }
                else
                {
                    MoveCursorOneSpotInInventory(whichWayToScroll);
                }

                SoundManager.instance.PlaySFX(0);
                ttUi.UpdateCardDisplayUI(currentSpotInCardInventory);
                break;

            default:
                throw new ArgumentOutOfRangeException(nameof(whichWayToScroll), whichWayToScroll, null);
        }
    }

    private void MoveCursorOneSpotInInventory(WhichScrollDirection whichWayAreYouGoing)
    {
        switch (whichWayAreYouGoing)
        {
            case WhichScrollDirection.IsIncrementing:
                fingerLocationOnCurrentPage++;
                currentSpotInCardInventory++;
                break;
            case WhichScrollDirection.IsDecrementing:
                fingerLocationOnCurrentPage--;
                currentSpotInCardInventory--;
                break;
        }

    }

    private void LoopInCurrentMenu(WhichWayToLoop whichLoopToDo)
    {
        switch (whichLoopToDo)
        {
            case WhichWayToLoop.LoopToTop:
                fingerLocationOnCurrentPage = 0;
                currentSpotInCardInventory = currentSpotInCardInventory -
                    ttUi.listOfPages[currentPageNumber - 1].currentActiveCardsOnPage + 1;
                break;
            case WhichWayToLoop.LoopToBottom:
                fingerLocationOnCurrentPage = ttUi.listOfPages[currentPageNumber - 1].currentActiveCardsOnPage - 1;
                currentSpotInCardInventory = currentSpotInCardInventory + ttUi.listOfPages[currentPageNumber - 1].currentActiveCardsOnPage - 1;
                break;
        }

    }

    private void MoveCursorToNextPage(PageScrollDirections scrollDirection)
    {
        switch (scrollDirection)
        {
            case PageScrollDirections.GoingForward:
                currentPageNumber++;
                ttUi.UpdatePageNum(currentPageNumber);
                currentSpotInCardInventory = currentSpotInCardInventory + 10;
                if (currentSpotInCardInventory < ttDb.RetrieveTotalNumberOfBattleCards())
                {
                    ttUi.UpdateCardDisplayUI(currentSpotInCardInventory);
                }
                else
                {
                    BounceCursorUpOnPageMove();
                }

                break;
            case PageScrollDirections.GoingBackward:
                currentPageNumber--;
                ttUi.UpdatePageNum(currentPageNumber);
                currentSpotInCardInventory = currentSpotInCardInventory - 10;
                ttUi.UpdateCardDisplayUI(currentSpotInCardInventory);
                break;
        }
    }

    private void BounceCursorUpOnPageMove()
    {
        fingerLocationOnCurrentPage = ttUi.listOfPages[currentPageNumber - 1].currentActiveCardsOnPage - 1;
        currentSpotInCardInventory = ttDb.RetrieveTotalNumberOfBattleCards() - 1;
        ttUi.UpdateCardDisplayUI(currentSpotInCardInventory);
    }


    private IEnumerator ScrollContinuously(WhichScrollDirection scrollDirection)
    {
        var numberToIncrement = 0;
        regularAutoScrollSpeed = true;
        while (true)
        {
            if (regularAutoScrollSpeed)
            {
                MoveCursorUpDownInMenu(scrollDirection);
                numberToIncrement++;
                if (numberToIncrement >= 2) { regularAutoScrollSpeed = false; }
                yield return new WaitForSeconds(autoScrollSpeed);
            }
            else if (!regularAutoScrollSpeed)
            {
                MoveCursorUpDownInMenu(scrollDirection);
                yield return new WaitForSeconds(fastAutoScrollSpeed);
            }
        }
        // ReSharper disable once IteratorNeverReturns
    }

    private IEnumerator PageScrollContinuously(PageScrollDirections whichWayToScroll)
    {
        while (true)
        {
            ChangePage(whichWayToScroll);
            yield return new WaitForSeconds(autoScrollSpeed * .75f);
        }
        // ReSharper disable once IteratorNeverReturns
    }

    private void CardSelection(int cardToSelect)
    {
        if (!ttLogic.CanSelectCardInCardSelection(cardToSelect)) {SoundManager.instance.PlaySFX(3); return;}
        ttUi.UpdateMyHandImage(ttDb.currentHandSelectionsList.Count, cardToSelect);
        ttUi.PlayMyHandAnimation(ttDb.currentHandSelectionsList.Count);
        AddChoiceToCardSelectionList(cardToSelect);
        ModifyCardQuantity(ModifyingCardQuantity.IsAddingCard);
        SoundManager.instance.PlaySFX(6);
        if (ttDb.currentHandSelectionsList.Count == 5)
        {
            TripleTriadManager.instance.ChangeToCardConfirmGameState();
        }
    }

    private void ModifyCardQuantity(ModifyingCardQuantity whatToDoWithQuantity)
    {//used to modify the card select quantity in screen and in the db
        switch (whatToDoWithQuantity)
        {
            case ModifyingCardQuantity.IsAddingCard:
                ttDb.currentBattleQuantityForCards[currentSpotInCardInventory]--;
                if (ttDb.currentBattleQuantityForCards[currentSpotInCardInventory] > 0)
                {
                    ttUi.UpdateQuantityAfterSelecting(currentPageNumber, fingerLocationOnCurrentPage,
                        currentSpotInCardInventory);
                }
                else
                {
                    ttUi.UpdateColorOfCardSelectionText(ttDb.currentBattleQuantityForCards[currentSpotInCardInventory],
                        currentPageNumber, fingerLocationOnCurrentPage);
                    ttUi.UpdateQuantityAfterSelecting(currentPageNumber, fingerLocationOnCurrentPage,
                        currentSpotInCardInventory);
                }

                break;
            case ModifyingCardQuantity.IsRemovingCard:
                var spotInSelectionsListToCheck = ttDb.currentHandSelectionsList.Count - 1;
                ttDb.currentBattleQuantityForCards[
                    ttDb.currentHandSelectionsList[spotInSelectionsListToCheck].spotInCardInv]++;
                ttUi.UpdateQuantityAfterSelecting(
                    ttDb.currentHandSelectionsList[spotInSelectionsListToCheck].pageNum,
                    ttDb.currentHandSelectionsList[spotInSelectionsListToCheck].spotOnPage,
                    ttDb.currentHandSelectionsList[spotInSelectionsListToCheck].spotInCardInv);
                ttUi.UpdateColorOfCardSelectionText(
                    ttDb.currentBattleQuantityForCards[
                        ttDb.currentHandSelectionsList[spotInSelectionsListToCheck].spotInCardInv],
                    ttDb.currentHandSelectionsList[spotInSelectionsListToCheck].pageNum,
                    ttDb.currentHandSelectionsList[spotInSelectionsListToCheck].spotOnPage);
                break;
        }
    }

    private void AddChoiceToCardSelectionList(int cardToSelect)
    {//used when you press select
        var cardChoice = new LastItemChosen(currentPageNumber, fingerLocationOnCurrentPage, cardToSelect);
        ttDb.currentHandSelectionsList.Add(cardChoice);
    }

    public void CancelLastSelection()
    {//used when you press cancel button
        if (!ttLogic.CanRemoveCardFromCardSelection()) return;
        ModifyCardQuantity(ModifyingCardQuantity.IsRemovingCard);
        ttUi.PlayRestAnimation(ttDb.currentHandSelectionsList.Count - 1);
        ttDb.currentHandSelectionsList.RemoveAt(ttDb.currentHandSelectionsList.Count - 1);
        SoundManager.instance.PlaySFX(1);
    }

    public void StopTheCoroutineScrolling()
    {
        StopCoroutine(_continuousCoroutineReference);
    }
}