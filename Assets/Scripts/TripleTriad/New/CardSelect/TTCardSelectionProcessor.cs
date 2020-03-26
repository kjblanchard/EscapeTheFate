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
        else if (Input.GetKeyUp(KeyCode.A)|| (Input.GetButtonUp("right")))
        {
            StopCoroutine(_continuousCoroutineReference);
        }
        else if (Input.GetKeyUp(KeyCode.W)|| (Input.GetButtonUp("right")))
        {
            StopCoroutine(_continuousCoroutineReference);
        }
        else if (Input.GetKeyUp(KeyCode.S)|| (Input.GetButtonUp("right")))
        {
            StopCoroutine(_continuousCoroutineReference);
        }

        ttUi.KeepFingerOnProperLocationInCardSelection(currentPageNumber, fingerLocationOnCurrentPage);

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
                    MoveCursorToNextPage(true);
                    SoundManager.instance.PlaySFX(6);

                }
                break;
            case PageScrollDirections.GoingBackward:
                canPageSwitch = ttLogic.CanISwitchPages(false,currentPageNumber);
                if (canPageSwitch)
                {
                    StartCoroutine(ttUi.MoveBackwardScrollAnimation(totalNumberOfPages));
                    MoveCursorToNextPage(false);
                    SoundManager.instance.PlaySFX(6);
                }
                break;
            default:
                throw new ArgumentOutOfRangeException(nameof(whichWayToScroll), whichWayToScroll, null);
        }
    }

    public void InitializeBattleCardAlbum()
    {
        ttUi.ResetScrollRectPosition();
        ttUi.GeneratePages();
        ttDb.BringInUsableBattleCards();
        ttUi.InitializeUiFromTtBattleList();
        currentPageNumber = 1;
        ttUi.UpdatePageNum(currentPageNumber);
        currentSpotInCardInventory = 0;
        fingerLocationOnCurrentPage = 0;
        ttUi.UpdateCardDisplayUI(0);
        ttDb.ClearBattleSelectionsList();
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
                    ttLogic.AreYouGoingToLoopInCardConfirm(fingerLocationOnCurrentPage, currentPageNumber,
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

    private void MoveCursorToNextPage(bool isIncrementing)
    {
        if (isIncrementing)
        {
            currentPageNumber++;
            ttUi.UpdatePageNum(currentPageNumber);
            currentSpotInCardInventory = currentSpotInCardInventory + 10;
            if (currentSpotInCardInventory < ttDb.RetrieveTotalNumberOfBattleCards())
            {
                ttUi.UpdateCardDisplayUI(currentSpotInCardInventory);
            }
            else
            { //this will bounce your cursor up
                fingerLocationOnCurrentPage = ttUi.listOfPages[currentPageNumber - 1].currentActiveCardsOnPage - 1;
                currentSpotInCardInventory = ttDb.RetrieveTotalNumberOfBattleCards() - 1;
                ttUi.UpdateCardDisplayUI(currentSpotInCardInventory);

            }
        }
        else
        {
            currentPageNumber--;
            ttUi.UpdatePageNum(currentPageNumber);
            currentSpotInCardInventory = currentSpotInCardInventory - 10;
            ttUi.UpdateCardDisplayUI(currentSpotInCardInventory);
        }
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
            yield return new WaitForSeconds(autoScrollSpeed*.75f);
        }
        // ReSharper disable once IteratorNeverReturns
    }

    // public void UpdateCursorInUI()
    // {
    //     ttUi.KeepFingerOnProperLocationInCardSelection(currentPageNumber, fingerLocationOnCurrentPage);;
    // }

    private void CardSelection(int cardToSelect)
    {
        if (!ttLogic.CanSelectCardInCardSelection(cardToSelect)) {SoundManager.instance.PlaySFX(3); return;}
        ttUi.UpdateMyHandImage(ttDb.currentCardSelectionsList.Count, cardToSelect);
        ttUi.PlayMyHandAnimation(ttDb.currentCardSelectionsList.Count);
        AddChoiceToCardSelectionList(cardToSelect);
        ModifyCardQuantity(ModifyingCardQuantity.IsAddingCard);
        SoundManager.instance.PlaySFX(6);
        if (ttDb.currentCardSelectionsList.Count == 5)
        {
            TripleTriadManager.instance.ChangeToCardConfirmGameState();
        }
    }

    private void ModifyCardQuantity(ModifyingCardQuantity whatToDoWithQuantity)
    {
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
                var spotInSelectionsListToCheck = ttDb.currentCardSelectionsList.Count - 1;
                ttDb.currentBattleQuantityForCards[
                    ttDb.currentCardSelectionsList[spotInSelectionsListToCheck].spotInCardInv]++;
                ttUi.UpdateQuantityAfterSelecting(
                    ttDb.currentCardSelectionsList[spotInSelectionsListToCheck].pageNum,
                    ttDb.currentCardSelectionsList[spotInSelectionsListToCheck].spotOnPage,
                    ttDb.currentCardSelectionsList[spotInSelectionsListToCheck].spotInCardInv);
                ttUi.UpdateColorOfCardSelectionText(
                    ttDb.currentBattleQuantityForCards[
                        ttDb.currentCardSelectionsList[spotInSelectionsListToCheck].spotInCardInv],
                    ttDb.currentCardSelectionsList[spotInSelectionsListToCheck].pageNum,
                    ttDb.currentCardSelectionsList[spotInSelectionsListToCheck].spotOnPage);
                break;
        }
    }

    private void AddChoiceToCardSelectionList(int cardToSelect)
    {
        var cardChoice = new LastItemChosen(currentPageNumber, fingerLocationOnCurrentPage, cardToSelect);
        ttDb.currentCardSelectionsList.Add(cardChoice);
    }

    public void CancelLastSelection()
    {
        if (ttLogic.CanRemoveCardFromCardSelection())
        {
            ModifyCardQuantity(ModifyingCardQuantity.IsRemovingCard);
            ttUi.PlayRestAnimation(ttDb.currentCardSelectionsList.Count - 1);
            ttDb.currentCardSelectionsList.RemoveAt(ttDb.currentCardSelectionsList.Count - 1);
            SoundManager.instance.PlaySFX(1);
        }
    }

    public void StopTheCoroutineScrolling()
    {
        StopCoroutine(_continuousCoroutineReference);
    }
}