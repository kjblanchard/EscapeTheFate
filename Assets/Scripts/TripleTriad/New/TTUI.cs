using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using UnityEngine;
using UnityEngine.Experimental.PlayerLoop;
using UnityEngine.UI;

public class TTUI : MonoBehaviour
{

    [Header("Script Reference")] [SerializeField]
    TTDB ttdb;

    [SerializeField] TripleTriadManager ttMan;

    [Header("State Controls")] public bool isLoading = false;

    [Header("Scroll Rect Stuff")] [SerializeField]
    ScrollRect imTheScrollRect;

    [SerializeField] Transform battleScrollRectSpacer;
    [Range(0, 10f)] [SerializeField] float[] scrollSpeed;

    [Header("Current List of Pages")] 
    public List<TTCardPage> listOfPages = new List<TTCardPage>();

    [Header("Prefab References")] [SerializeField]
    GameObject fingerPrefab;

    [SerializeField] GameObject battlePagePrefab;

    [Header("Card Select - UI references")] [SerializeField]
    Text cardDescription;
    [SerializeField] Image cardImage;
    [SerializeField] Text cardRarity;
    [SerializeField] Text pageNumText;



    [Header("Card Confirm - UI References")]
    [SerializeField] Transform[] cardConfirmFingerLocations;
    [SerializeField] GameObject CardConfirmCanvas;

    [Header("Animation")] [SerializeField] Animator cardDisplayAnimator;
    [SerializeField] Animator[] myHandCardAnimators;
    [SerializeField] Animator cardConfirmAnimator;

    [Header("My Hand Images")] [SerializeField]
    Image[] myHandCardImages;




    public void GeneratePages()
    {
        //run calculations in the db, and then generate the amount of pages based on the number of cards in your inventory
        ttdb.CalculateTtBattleNumbers();
        var totalOfCardsOnLastPage = ttdb.RetrieveTotalNumberOfCardsOnLastBattlePage();
        var totalPages = ttdb.RetrieveTotalNumberOfBattlePages();
        listOfPages.Clear();

        for (int i = 0; i < totalPages; i++)
        {
            GameObject newBattlePage = Instantiate(battlePagePrefab, battleScrollRectSpacer, false);
            listOfPages.Add(newBattlePage.GetComponent<TTCardPage>());

        }

        ModifyLastPagesActiveCards(totalOfCardsOnLastPage);
    }

    private void ModifyLastPagesActiveCards(int totalOfCardsOnLastPage)
    {
        //Activates or deactivates the cards on the last page, based on how many cards are on it
        if (totalOfCardsOnLastPage != 0)
        {
            var pageToModify = listOfPages[listOfPages.Count - 1];
            pageToModify.currentActiveCardsOnPage = 0;
            for (int i = 0; i < pageToModify.cardGameObjects.Length; i++)
            {
                if (totalOfCardsOnLastPage > i)
                {
                    pageToModify.cardGameObjects[i].SetActive(true);
                    pageToModify.currentActiveCardsOnPage++;
                }
                else
                {
                    pageToModify.cardGameObjects[i].SetActive(false);
                }
            }
        }
    }

    public void InitializeUiFromTtBattleList()
    {
        //take your renumbered list, and populate them onto the pages that your cursor points to, based on if you own the card, if you've lost the card, or if you've never owned the card
        var numberOfCardsOnPage = ttdb.RetrieveTotalNumberOfCardsOnPage();
        for (int i = 0; i < ttdb.currentBattleSelectableCards.Count; i++)
        {
            var pageToPutThisOn = i / numberOfCardsOnPage;
            var positionToPutThisOn = i - (pageToPutThisOn * 10);
            listOfPages[pageToPutThisOn].cardQuantityLocations[positionToPutThisOn].text =
                ttdb.currentBattleSelectableCards[i].amountOwned.ToString();
            listOfPages[pageToPutThisOn].cardNameLocations[positionToPutThisOn].text =
                ttdb.currentBattleSelectableCards[i].cardName;
        }
    }

    public void ResetScrollRectPosition()
    {
        imTheScrollRect.horizontalNormalizedPosition = 0;
    }

    public IEnumerator MoveForwardScrollAnimation(int pagesCount)
    {
        //these have a weird speed issue, so have to use a scroll speed array that has different values for different amount of pages
        var initialValue = imTheScrollRect.horizontalNormalizedPosition;
        var target = initialValue + (float) 1 / (pagesCount - 1);

        isLoading = true;
        while (initialValue != target)
        {

            initialValue = Mathf.MoveTowards(initialValue, target, scrollSpeed[pagesCount - 1] * Time.deltaTime);
            imTheScrollRect.horizontalNormalizedPosition = initialValue;
            yield return null;
        }

        isLoading = false;

    }

    public IEnumerator MoveBackwardScrollAnimation(int pagesCount)
    {
        //these have a weird speed issue, so have to use a scroll speed array that has different values for different amount of pages
        var initialValue = imTheScrollRect.horizontalNormalizedPosition;
        var target = initialValue - (float) 1 / (pagesCount - 1);

        isLoading = true;
        while (initialValue != target)
        {
            initialValue = Mathf.MoveTowards(initialValue, target, scrollSpeed[pagesCount - 1] * Time.deltaTime);
            imTheScrollRect.horizontalNormalizedPosition = initialValue;
            yield return null;
        }

        isLoading = false;

    }

    public void KeepFingerOnProperLocationInCardSelection(int pageNum, int fingerPos)
    {
        //finger is turned on and off when in update method by the isloading bool
        if (isLoading == false)
        {
            if (!fingerPrefab.gameObject.activeInHierarchy)
            {
                fingerPrefab.gameObject.SetActive(true);
            }

            fingerPrefab.transform.position = listOfPages[pageNum - 1].fingerLocations[fingerPos].transform.position;
        }
        else
        {
            fingerPrefab.gameObject.SetActive(false);
        }
        

    }

    public void UpdateCardDisplayUI(int cardToLookup)
    {
        //This updates various field on the screen, depending on if you own the card, if you've lost the card, or if you've never owned the card
        cardDescription.text = ttdb.currentBattleSelectableCards[cardToLookup].description;
        cardRarity.text = ttdb.currentBattleSelectableCards[cardToLookup].cardsRarity.ToString();
        cardImage.sprite = ttdb.currentBattleSelectableCards[cardToLookup].AImage;
        PlayDisplayCardAnimation();

    }

    public void UpdatePageNum(int currentPageNum)
    {
        pageNumText.text = currentPageNum.ToString();
    }

    private void PlayDisplayCardAnimation()
    {
        cardDisplayAnimator.Play("Rest");
    }

    public void PlayMyHandAnimation(int cardToDisplay)
    {
        myHandCardAnimators[cardToDisplay].Play("MyHandSelect");
    }

    public void PlayRestAnimation(int cardToDisplay)
    {
        myHandCardAnimators[cardToDisplay].Play("Rest");

    }

    public void UpdateMyHandImage(int positionToUpdate, int cardNumToLookup)
    {
        myHandCardImages[positionToUpdate].sprite = ttdb.currentBattleSelectableCards[cardNumToLookup].AImage;
    }

    public void UpdateQuantityAfterSelecting(int currentPage, int currentPosition, int currentSpotInCardInventory)
    {
        listOfPages[currentPage - 1].cardQuantityLocations[currentPosition].text =
            ttdb.currentBattleQuantityForCards[currentSpotInCardInventory].ToString();
    }

    public bool IsTheUiLoading()
    {
        return isLoading;
    }

    public void UpdateColorOfCardSelectionText(int quantityAmount, int currentPage, int currentPositionOnCardPage)
    {
        if (quantityAmount > 0)
        {
            listOfPages[currentPage - 1].cardNameLocations[currentPositionOnCardPage].color = Color.white;
            listOfPages[currentPage - 1].cardQuantityLocations[currentPositionOnCardPage].color = Color.white;
        }
        else
        {
            listOfPages[currentPage - 1].cardNameLocations[currentPositionOnCardPage].color = Color.gray;
            listOfPages[currentPage - 1].cardQuantityLocations[currentPositionOnCardPage].color = Color.gray;
        }
    }

    public void KeepFingerOnProperLocationInCardConfirmation(int fingerPos)
    {
        //finger is turned on and off when in update method by the isloading bool
        if (isLoading == false)
        {
            if (!fingerPrefab.gameObject.activeInHierarchy)
            {
                fingerPrefab.gameObject.SetActive(true);
            }

            fingerPrefab.transform.position = cardConfirmFingerLocations[fingerPos].transform.position;
        }
        else
        {
            fingerPrefab.gameObject.SetActive(false);
        }
    }

    public void InitializeCardConfirmScreen()
    {
        isLoading = true;   
        CardConfirmCanvas.SetActive(true);
        StartCoroutine(PlayCardConfirmAnimator());
    }

    public void InitializeCardSelectionScreen()
    {
        CardConfirmCanvas.SetActive(false);
    }

    private IEnumerator PlayCardConfirmAnimator()
    {
        cardConfirmAnimator.Play("Displaying");
        yield return new WaitForSeconds(0.750f);
        isLoading = false;
    }

    public int ReturnCardConfirmFingerPosCount()
    {
        return cardConfirmFingerLocations.Length;
    }
}

