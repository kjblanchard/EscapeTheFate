using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// notes about returning to this - something is funky with the AI, it generates things wrong in the potential moves sometimes? could be placing or could be potential moves - seems to be when you place a card in position 4 it thinks it can flip it when it cant
/// there is a lot of extra board refreshes, I think a lot of them can be removed.. I solved the issue when I did a check at the end of the game on active cards that are active, and removing soring order, etc
/// </summary>
public class TTManager : MonoBehaviour

{


    [Header("Total Game Reference")]
    //public Card[] allGameCards;
    //public int[] ownedCardQuantity;
    //public Sprite defaultCardBack;
    public static TTManager instance;
    public TTCardSelectionProcessor ttSelectionController;
    public float[] waitTime;
    public bool playerTurn;
    public bool enemyTurn;
    public int cardsOnBoard;
    public bool endingStarted;
    public bool turnNotChosen = true;
    public GameObject defaultFingerPosition;
    [SerializeField] int timesToBlink;
    bool spinning;
    public bool initializingFields;
    [SerializeField] string[] defaultTexts;
    [SerializeField] bool aiOn;
    [SerializeField] bool closedEnemyHand;
    [SerializeField] bool randomEnemyHand;

    [Header("Startup")]
    [SerializeField] GameObject backgroundIMG;
    [SerializeField] GameObject board;
    [SerializeField] GameObject cardSelect;
    [SerializeField] GameObject score;
    [SerializeField] GameObject fingers;
    public bool startGame = false;


    [Header("Initialize Field Bools")]
    [SerializeField] bool[] chooseCardBool;
    [SerializeField] bool[] confirmCardBool;
    [SerializeField] bool[] displayEnemyCardBool;
    [SerializeField] bool[] turnSelectionBool;
    [SerializeField] bool[] cardSelectBool;
    [SerializeField] bool[] enemySelectBool;
    [SerializeField] bool[] enemySelectBoolAI;
    [SerializeField] bool[] locationSelectionBool;
    [SerializeField] bool[] enemyLocationSelectionBool;
    [SerializeField] bool[] endGameBool;

    [Header("Text Controller")]
    [SerializeField] string[] pleaseWaitText;
    [SerializeField] string[] winTitleText;
    public int pleaseWaitTextSelected = 0;
    public  int turnSelectionTextSelected = 0;
    [SerializeField] string[] rematchText;
    [SerializeField] string[] turnSelectionText;

    [Header("Scoring")]
    public int[] scoreValues;
    public Text playerScore;
    public int currentPlayerScore;
    public Text enemyScore;
    public int currentEnemyScore;

    [Header("Card Modifying")]
    public int translateAmount;
    public VerticalLayoutGroup myHandVLG;
    public List<Canvas> myHandCanvas = new List<Canvas>();
    public GameObject myHandGameObject;
    public VerticalLayoutGroup enemyHandVLG;
    public List<Canvas> enemyHandCanvas = new List<Canvas>();
        public GameObject enemyHandGameObject;


    [Header("UI Frame References")]
    public GameObject cardSelectionScreen = null;
    public GameObject cardConfirmScreen = null;
    public Image cardSelectionImage = null;
    public GameObject helpInfoPanel = null;
    public GameObject thinkingPanel = null;
    public Text thinkingPleaseWait = null;
    public Text throatPunchTextUI = null;
    public GameObject rematchPanel = null;
    [SerializeField] GameObject myScorePanel;
    [SerializeField] GameObject enemyScorePanel;

    [Header("UI Card in Hand")]
    public List<GameObject> myHand = new List<GameObject>();
    public List<GameObject> enemyHand = new List<GameObject>();

    [Header("Hand Reference Lists")]
    private List<GameObject> referenceMyHand = new List<GameObject>();
    private List<GameObject> referenceEnemyHand = new List<GameObject>();
    private List<GameObject> referenceHandLocations = new List<GameObject>();
    private List<GameObject> referenceEnemyHandLocations = new List<GameObject>();
    private List<Canvas> referenceMyHandCanvasList = new List<Canvas>();
    private List<Canvas> referenceEnemyHandCanvasList = new List<Canvas>();


    [Header("Hand Choosing References")]
    public Text[] availableCardName;
    public Text[] availableCardQuantity;
    public int[] tempAvailableCardQuantity;
    public Text[] availableCardDescription;
    public Sprite currentCardImage;
    public int[] prevChosenCardLocation;
    public int currentHandSize;
    public CardSelectHolder[] cardHolders;

    [Header("Card Choosing References")]
    public Vector3 previousLocation;
    public bool justStarted = true;
    public bool movingUp = false;
    public bool movingDown = false;
    public int transformedCardLocation;
    public int enemyTransformedCardLocation;

    [Header("BoardChoosing References")]

    public BoardLocation[] boardLocationHolder;
    public GameObject[] boardGameObjects;


    [Header("Current Hand List")]
    public List<Card> myHandList = new List<Card>();
    public List<Card> enemyHandList = new List<Card>();

    [Header("Finger Location")]
    public int chooseCardLocation = 0;
    public int cardConfirmLocation = 0;
    public int cardSelectLocation = 0;
    public int boardSelectLocation = 0;
    public int enemySelectLocation = 0;
    public int rematchSelectLocation = 0;

    [Header("Finger Locations")]
    public List<GameObject> chooseCardList = new List<GameObject>();
    public List<GameObject> cardConfirmList = new List<GameObject>();
    public List<GameObject> cardSelectList = new List<GameObject>();
    public List<GameObject> boardSelectList = new List<GameObject>();
    public List<GameObject> enemySelectList = new List<GameObject>();
    public List<GameObject> rematchSelectList = new List<GameObject>();

    public Image[] boardImages;

    [Header("Current Card Reference")]
    public Text cardName;
    public string currentCardName;
    public Sprite currentCardSprite;

    [Header("Finger UI reference")]
    public GameObject horizontalFinger;
    public GameObject verticalFinger;
    public GameObject flippedFinger;
    public GameObject blinkingFinger;
    public GameObject blinkingVerticalFinger;

    [Header("AI references")]
    [SerializeField] List<int> potentialBoardLocation = new List<int>();
    [SerializeField] List<int> potentialCardLocation = new List<int>();

    public enum GameStates
    {
        DISABLED,
        CHOOSE_CARDS,
        CONFIRM_CARDS,
        DISPLAY_ENEMY_CARDS,
        TURN_SELECTION,
        CARD_SELECTION,
        LOCATION_SELECTION,
        ENEMY_SELECTION,
        ENEMY_LOCATION_SELECTION,
        END,
    }

    public GameStates gameState;


    private void Awake()
    {
        int essentialsLoaderCount = FindObjectsOfType<TTManager>().Length;
        if (essentialsLoaderCount > 1)
        {
            Destroy(gameObject);
        }
        else
        {
            instance = this;
            DontDestroyOnLoad(gameObject);
        }
    }

void Start()
    {


    }



    void Update()
    {
        switch (gameState)
        {
            case GameStates.DISABLED:
                Disabled();
                break;
            case GameStates.CHOOSE_CARDS:
                //if (initializingFields == true) { initializingFields = false; ttSelectionController.InitializeBattleCardAlbum(); }
                //ttSelectionController.UpdateTTBattleInputs();
                ChooseCards();
                break;
            case GameStates.CONFIRM_CARDS:
                ConfirmCards();
                break;
            case GameStates.DISPLAY_ENEMY_CARDS:
                DisplayEnemyCards();
                break;
            case GameStates.TURN_SELECTION:
                if (turnNotChosen == true)
                {
                    StartCoroutine(TurnSelection());
                }
                break;
            case GameStates.CARD_SELECTION:

                CardSelection();
                break;
            case GameStates.LOCATION_SELECTION:
                LocationSelection();
                break;
            case GameStates.ENEMY_SELECTION:
                EnemySelection();
                break;
            case GameStates.ENEMY_LOCATION_SELECTION:
                EnemySelectLocation();
                break;
            case GameStates.END:

                    End();

                break;
        }
    }

    /// <summary>
 //////////////////////////////////Start code
    /// </summary>

    public void Disabled()
    {
        if(startGame == true)
        {
            startGame = false;

            //RestartGame();
            initializingFields = true;
            PlayerController.instance.canMove = false;
            TurnOnElements(true);
            GameManager.instance.ttActive = true;

            gameState = GameStates.CHOOSE_CARDS;
            
        }
    }
    public void ChooseCards()
    {
        if (initializingFields == true)
        {
            //ttSelectionController.InitializeBattleCardAlbum();

            InitializeFields(chooseCardBool);
            InitializeCardInformations();
            EnemyHandGenerator();
            RememberHandUI();


        }

        if ((Input.GetKeyDown(KeyCode.S)|| (Input.GetButtonDown("down"))) && chooseCardLocation < chooseCardList.Count - 1)
        {
            chooseCardLocation++;
            SoundManager.instance.PlaySFX(0);
        }

        if ((Input.GetKeyDown(KeyCode.W)||(Input.GetButtonDown("up"))) && chooseCardLocation > 0)
        {

            chooseCardLocation--;
            SoundManager.instance.PlaySFX(0);
        }
        if ((Input.GetKeyDown(KeyCode.Space)||(Input.GetButtonDown("Fire1"))) && myHandList.Count < 5)

        {
            if (tempAvailableCardQuantity[chooseCardLocation] > 0)
            {
                tempAvailableCardQuantity[chooseCardLocation]--;
                currentCardName = cardHolders[chooseCardLocation].cardHeld.cardName;
                CardChosen();
                availableCardQuantity[chooseCardLocation].text = tempAvailableCardQuantity[chooseCardLocation].ToString();
                prevChosenCardLocation[myHandList.Count - 1] = chooseCardLocation;
                UpdateCurrentHandSize();

                StartCoroutine(RotateCard(myHand[currentHandSize -1]));
                SoundManager.instance.PlaySFX(4);
            }
        }

        if ((Input.GetKeyDown(KeyCode.C)||(Input.GetButtonDown("Fire2"))) && currentHandSize > 0)
        {
            tempAvailableCardQuantity[prevChosenCardLocation[currentHandSize - 1]]++;
            myHandList.RemoveAt(currentHandSize - 1);
            UpdateCurrentHandSize();
            myHand[currentHandSize].SetActive(false);
            UpdateQuantityDisplay();
            SoundManager.instance.PlaySFX(1);
        }

        if (myHandList.Count == 5)
        {
            gameState = GameStates.CONFIRM_CARDS;
            initializingFields = true;
        }

        currentCardImage = cardHolders[chooseCardLocation].cardHeld.AImage;
        cardSelectionImage.sprite = currentCardImage;
        horizontalFinger.transform.position = chooseCardList[chooseCardLocation].transform.position;
    }

    public void ConfirmCards()
    {
        if(initializingFields == true)
        {
            InitializeFields(confirmCardBool);
        }


        if ((Input.GetKeyDown(KeyCode.D)||(Input.GetButtonDown("right"))) && cardConfirmLocation < cardConfirmList.Count - 1)
        {
            cardConfirmLocation++;
            SoundManager.instance.PlaySFX(0);
        }

        if ((Input.GetKeyDown(KeyCode.A)||(Input.GetButtonDown("left"))) && cardConfirmLocation > 0)
        {

            cardConfirmLocation--;
            SoundManager.instance.PlaySFX(0);
        }
        if ((Input.GetKeyDown(KeyCode.Space))||(Input.GetButtonDown("Fire1")))
        {
            if (cardConfirmLocation == 0)
            {
                gameState = GameStates.DISPLAY_ENEMY_CARDS;
                initializingFields = true;
                StartCoroutine(PickingCards());

            }
            else if (cardConfirmLocation == 1)
            {
                SoundManager.instance.PlaySFX(1);
                myHandList.Clear();
                ClearCardDisplay();
                InitializeCardInformations();
                gameState = GameStates.CHOOSE_CARDS;
                initializingFields = true;
                cardConfirmLocation = 0;
            }

        }

        horizontalFinger.transform.position = cardConfirmList[cardConfirmLocation].transform.position;
    }

    public void CardSelection()
    {

        if (cardsOnBoard > 8)
        {
            horizontalFinger.SetActive(false);
            verticalFinger.SetActive(false);
            blinkingFinger.SetActive(false);
            flippedFinger.SetActive(false);
            initializingFields = true;
            gameState = GameStates.END;

        }
        else //continue game
        {



            if (initializingFields == true)
            {
                InitializeFields(cardSelectBool);
            }



            if ((Input.GetKeyDown(KeyCode.S)||(Input.GetButtonDown("down"))) && cardSelectLocation < myHandList.Count - 1)
            {
                movingDown = true;
                SoundManager.instance.PlaySFX(0);
            }

            if ((Input.GetKeyDown(KeyCode.W)||(Input.GetButtonDown("up"))) && cardSelectLocation > 0)
            {
                movingUp = true;
                SoundManager.instance.PlaySFX(0);


            }
            if ((Input.GetKeyDown(KeyCode.Space))||(Input.GetButtonDown("Fire1")))
            {
                gameState = GameStates.LOCATION_SELECTION;
                initializingFields = true;
                SoundManager.instance.PlaySFX(0);
            }






            if (movingDown == true)
            {
                cardSelectLocation++;
                myHand[transformedCardLocation].transform.Translate(-translateAmount, 0, 0);
                myHandCanvas[transformedCardLocation].sortingOrder = 0;
                myHandCanvas[transformedCardLocation].overrideSorting = false;
                transformedCardLocation++;
                myHandVLG.enabled = false;
                myHand[cardSelectLocation].transform.Translate(translateAmount, 0, 0);
                myHandCanvas[cardSelectLocation].overrideSorting = true;
                myHandCanvas[cardSelectLocation].sortingOrder = 1;
                movingDown = false;


            }

            if (movingUp == true)
            {
                cardSelectLocation--;
                myHand[transformedCardLocation].transform.Translate(-translateAmount, 0, 0);
                myHandCanvas[transformedCardLocation].sortingOrder = 0;
                myHandCanvas[transformedCardLocation].overrideSorting = false;
                transformedCardLocation--;
                myHandVLG.enabled = false;
                myHand[cardSelectLocation].transform.Translate(translateAmount, 0, 0);
                myHandCanvas[cardSelectLocation].overrideSorting = true;
                myHandCanvas[cardSelectLocation].sortingOrder = 1;
                movingUp = false;

            }

            currentCardName = myHandList[cardSelectLocation].cardName;
            cardName.text = currentCardName;
            flippedFinger.transform.position = cardSelectList[cardSelectLocation].transform.position;

            if (justStarted == true)
            {
                justStarted = false;
                Debug.Log("just did the thing");
                transformedCardLocation = 0;
                transformedCardLocation = cardSelectLocation;
                myHandVLG.enabled = false;
                myHand[cardSelectLocation].transform.Translate(translateAmount, 0, 0);
                myHandCanvas[cardSelectLocation].overrideSorting = true;
                myHandCanvas[cardSelectLocation].sortingOrder = 1;

                Debug.Log("just finished the thing");
            }
        }

    }



    public void LocationSelection()
    {
        if (initializingFields == true)
        {
            InitializeFields(locationSelectionBool);
        }


        if ((Input.GetKeyDown(KeyCode.A)||(Input.GetButtonDown("left"))) && boardSelectLocation > 0 && boardSelectLocation != 3 && boardSelectLocation != 6)
        {
            boardSelectLocation--;
            SoundManager.instance.PlaySFX(0);
        }

        if ((Input.GetKeyDown(KeyCode.S)||(Input.GetButtonDown("down"))) && boardSelectLocation < 6)
        {
            boardSelectLocation = boardSelectLocation + 3;
            SoundManager.instance.PlaySFX(0);
        }

        if ((Input.GetKeyDown(KeyCode.D)||(Input.GetButtonDown("right"))) && boardSelectLocation != 2 && boardSelectLocation != 5 && boardSelectLocation < boardSelectList.Count - 1)
        {
            boardSelectLocation++;
            SoundManager.instance.PlaySFX(0);
        }

        if ((Input.GetKeyDown(KeyCode.W) || (Input.GetButtonDown("up"))) && boardSelectLocation > 2)
        {
            boardSelectLocation = boardSelectLocation - 3;
            SoundManager.instance.PlaySFX(0);
        }

        if ((Input.GetKeyDown(KeyCode.C)) || (Input.GetButtonDown("Fire2")))
        {
            DisableBlinkingFinger();
            if (playerTurn)
            {


                gameState = GameStates.CARD_SELECTION;
                initializingFields = true;
            }

            SoundManager.instance.PlaySFX(0);
            boardSelectLocation = 0;
        }
        if ((Input.GetKeyDown(KeyCode.Space)) || (Input.GetButtonDown("Fire1")))
        {

            PlaceCardOnBoard();

        }


        blinkingVerticalFinger.transform.position = boardSelectList[boardSelectLocation].transform.position;
        if(boardLocationHolder[boardSelectLocation].spotTaken == false)
        {
            currentCardName = "";
            cardName.text = currentCardName;
        }
        else if(boardLocationHolder[boardSelectLocation].spotTaken == true)
        {
            currentCardName = boardLocationHolder[boardSelectLocation].cardHeld.cardName;
            cardName.text = currentCardName;
        }

    }

    public void EnemySelection()
    {

        if (cardsOnBoard > 8)
        {
            horizontalFinger.SetActive(false);
            verticalFinger.SetActive(false);
            blinkingFinger.SetActive(false);
            flippedFinger.SetActive(false);
            initializingFields = true;
            gameState = GameStates.END;
            
        }
        else //continue game
        {

            if (aiOn == false)
            {
                if(initializingFields == true)
                {
                    InitializeFields(enemySelectBool);
                }


                if (Input.GetKeyDown(KeyCode.S) && enemySelectLocation < enemySelectList.Count - 1)
                {
                    movingDown = true;
                    SoundManager.instance.PlaySFX(0);
                }

                if (Input.GetKeyDown(KeyCode.W) && enemySelectLocation > 0)
                {
                    movingUp = true;
                    SoundManager.instance.PlaySFX(0);
                }
                if (Input.GetKeyDown(KeyCode.Space))
                {
                    gameState = GameStates.ENEMY_LOCATION_SELECTION;
                    initializingFields = true;
                    SoundManager.instance.PlaySFX(0);
                }
                if (movingDown == true)
                {
                    enemySelectLocation++;
                    enemyHand[enemyTransformedCardLocation].transform.Translate(translateAmount, 0, 0);
                    enemyHandCanvas[enemyTransformedCardLocation].sortingOrder = 0;
                    enemyHandCanvas[enemyTransformedCardLocation].overrideSorting = false;
                    enemyTransformedCardLocation++;
                    enemyHandVLG.enabled = false;
                    enemyHand[enemySelectLocation].transform.Translate(-translateAmount, 0, 0);
                    enemyHandCanvas[enemySelectLocation].overrideSorting = true;
                    enemyHandCanvas[enemySelectLocation].sortingOrder = 1;
                    movingDown = false;


                }

                if (movingUp == true)
                {
                    enemySelectLocation--;
                    enemyHand[enemyTransformedCardLocation].transform.Translate(translateAmount, 0, 0);
                    enemyHandCanvas[enemyTransformedCardLocation].sortingOrder = 0;
                    enemyHandCanvas[enemyTransformedCardLocation].overrideSorting = false;
                    enemyTransformedCardLocation--;
                    enemyHandVLG.enabled = false;
                    enemyHand[enemySelectLocation].transform.Translate(-translateAmount, 0, 0);
                    enemyHandCanvas[enemySelectLocation].overrideSorting = true;
                    enemyHandCanvas[enemySelectLocation].sortingOrder = 1;
                    movingUp = false;

                }

                if (justStarted == true)
                {
                    justStarted = false;
                    enemyTransformedCardLocation = 0;
                    enemyTransformedCardLocation = enemySelectLocation;
                    enemyHandVLG.enabled = false;
                    enemyHand[enemySelectLocation].transform.Translate(-translateAmount, 0, 0);
                    enemyHandCanvas[enemySelectLocation].overrideSorting = true;
                    enemyHandCanvas[enemySelectLocation].sortingOrder = 1;

                }

                currentCardName = enemyHandList[enemySelectLocation].cardName;
                cardName.text = currentCardName;

                horizontalFinger.transform.position = enemySelectList[enemySelectLocation].transform.position;

            }
            else if(aiOn == true)
            {
                if (initializingFields == true)
                {
                    InitializeFields(enemySelectBoolAI);
                    if (closedEnemyHand == true)
                    {
                        helpInfoPanel.SetActive(false);
                    }
                    else if (closedEnemyHand == false)
                    {
                        helpInfoPanel.SetActive(true);
                    }
                }
                currentCardName = enemyHandList[enemySelectLocation].cardName;
                cardName.text = currentCardName;

                if (movingDown == true)
                {
                    enemySelectLocation++;
                    enemyHand[enemyTransformedCardLocation].transform.Translate(translateAmount, 0, 0);
                    enemyHandCanvas[enemyTransformedCardLocation].sortingOrder = 0;
                    enemyHandCanvas[enemyTransformedCardLocation].overrideSorting = false;
                    enemyTransformedCardLocation++;
                    enemyHandVLG.enabled = false;
                    enemyHand[enemySelectLocation].transform.Translate(-translateAmount, 0, 0);
                    enemyHandCanvas[enemySelectLocation].overrideSorting = true;
                    enemyHandCanvas[enemySelectLocation].sortingOrder = 1;
                    movingDown = false;


                }

                if (movingUp == true)
                {
                    enemySelectLocation--;
                    enemyHand[enemyTransformedCardLocation].transform.Translate(translateAmount, 0, 0);
                    enemyHandCanvas[enemyTransformedCardLocation].sortingOrder = 0;
                    enemyHandCanvas[enemyTransformedCardLocation].overrideSorting = false;
                    enemyTransformedCardLocation--;
                    enemyHandVLG.enabled = false;
                    enemyHand[enemySelectLocation].transform.Translate(-translateAmount, 0, 0);
                    enemyHandCanvas[enemySelectLocation].overrideSorting = true;
                    enemyHandCanvas[enemySelectLocation].sortingOrder = 1;
                    movingUp = false;

                }

                if (justStarted == true)
                {
                    justStarted = false;
                    enemyTransformedCardLocation = 0;
                    enemyTransformedCardLocation = enemySelectLocation;
                    enemyHandVLG.enabled = false;
                    enemyHand[enemySelectLocation].transform.Translate(-translateAmount, 0, 0);
                    enemyHandCanvas[enemySelectLocation].overrideSorting = true;
                    enemyHandCanvas[enemySelectLocation].sortingOrder = 1;
                    StartCoroutine(BeginnerCreatePotentialMoves());

                }


            }

        }
    }

    public void EnemySelectLocation()
    {
        if(initializingFields == true)
        {
            InitializeFields(enemyLocationSelectionBool);
        }



        if (Input.GetKeyDown(KeyCode.A) && boardSelectLocation > 0 && boardSelectLocation != 3 && boardSelectLocation != 6)
        {
            boardSelectLocation--;
            SoundManager.instance.PlaySFX(0);
        }

        if (Input.GetKeyDown(KeyCode.S) && boardSelectLocation < 6)
        {
            boardSelectLocation = boardSelectLocation + 3;
            SoundManager.instance.PlaySFX(0);
        }

        if (Input.GetKeyDown(KeyCode.D) && boardSelectLocation != 2 && boardSelectLocation != 5 && boardSelectLocation < boardSelectList.Count - 1)
        {
            boardSelectLocation++;
            SoundManager.instance.PlaySFX(0);
        }

        if (Input.GetKeyDown(KeyCode.W) && boardSelectLocation > 2)
        {
            boardSelectLocation = boardSelectLocation - 3;
            SoundManager.instance.PlaySFX(0);
        }

        if (Input.GetKeyDown(KeyCode.C))
        {
            DisableBlinkingFinger();
            gameState = GameStates.ENEMY_SELECTION;
            initializingFields = true;

            SoundManager.instance.PlaySFX(0);
            boardSelectLocation = 0;
        }
        if (Input.GetKeyDown(KeyCode.Space))
        {
            PlaceCardOnBoard();

        }


        blinkingVerticalFinger.transform.position = boardSelectList[boardSelectLocation].transform.position;

    }

    public void DisableBlinkingFinger() //added in to stop the blinky delay
    {
        blinkingVerticalFinger.SetActive(false);
        helpInfoPanel.SetActive(false);
    }

    public void CardChosen()
    {
        //myCardList.Add(currentCardName);
        //Debug.Log("I CHOOSE YOU" + currentCardName);
        for (int i = 0; i < CardInventory.instance.masterCardList.Length; i++)
        {
            if (CardInventory.instance.masterCardList[i].cardName == currentCardName)
            {
                myHandList.Add(CardInventory.instance.masterCardList[i]);
            }

        }
        UpdateCards();
    }

    public void UpdateCards()
    {

        for (int i = 0; i < myHandList.Count; i++)
        {
            myHand[i].SetActive(true);
            myHand[i].GetComponent<Image>().sprite = myHandList[i].AImage;
            Debug.Log("I just updated " + myHand[i] + " with" + myHandList[i]);
        }
    }

    public void ClearCardDisplay()
    {
        for (int i = 0; i < myHand.Count; i++)
        {
            myHand[i].SetActive(false);
        }
    }
    public void ClearEnemyCardDisplay()
    {
        for (int i = 0; i < enemyHand.Count; i++)
        {
            enemyHand[i].SetActive(false);
        }
    }

    private void InitializeCardInformations()
    {
        for (int j = 0; j < availableCardName.Length; j++)
        {
            string cardLookup = CardInventory.instance.cardList[j];

            for (int i = 0; i < CardInventory.instance.masterCardList.Length; i++)
            {
                if (CardInventory.instance.masterCardList[i].cardName == cardLookup)
                {
                    cardHolders[j].cardHeld = CardInventory.instance.masterCardList[i];
                }

            }
        }
        for (int i = 0; i < CardInventory.instance.cardList.Length; i++)
        {
            availableCardName[i].text = cardHolders[i].cardHeld.cardName;
            availableCardDescription[i].text = cardHolders[i].cardHeld.description;
            tempAvailableCardQuantity[i] = CardInventory.instance.cardAmount[i];
            availableCardQuantity[i].text = tempAvailableCardQuantity[i].ToString();
        }
    }

    public void UpdateCurrentHandSize()
    {
        currentHandSize = myHandList.Count;
    }

    private void UpdateQuantityDisplay()
    {
        for (int i = 0; i < availableCardQuantity.Length; i++)
        {
            availableCardQuantity[i].text = tempAvailableCardQuantity[i].ToString();

        }
    }

    private void PlaceCardOnBoard()
    {
        if (playerTurn == true && boardLocationHolder[boardSelectLocation].spotTaken == false)
        {
            boardLocationHolder[boardSelectLocation].spotTaken = true;
            boardLocationHolder[boardSelectLocation].playerOwned = true;
            boardLocationHolder[boardSelectLocation].checkIfFlipped = true;


            boardLocationHolder[boardSelectLocation].cardHeld = myHandList[cardSelectLocation]; // testing for card implementation
            myHandCanvas[cardSelectLocation].overrideSorting = true;
            myHandCanvas[cardSelectLocation].sortingOrder = 0; //added to replace reset canvas
            StartCoroutine(OneFrame());
            myHandCanvas[cardSelectLocation].overrideSorting = false;//added to replace reset canvas
            StartCoroutine(OneFrame());
            myHand[cardSelectLocation].SetActive(false);
            myHandList.RemoveAt(cardSelectLocation);
            cardSelectList.RemoveAt(cardSelectLocation);
            myHand.RemoveAt(cardSelectLocation);
            myHandCanvas.RemoveAt(cardSelectLocation);
            
            transformedCardLocation = 0;
            justStarted = true;
            SoundManager.instance.PlaySFX(6);
            DisableBlinkingFinger();
            
            cardSelectLocation = 0;

            cardsOnBoard++;
            initializingFields = true;
            gameState = GameStates.ENEMY_SELECTION;

                playerTurn = false;
                enemyTurn = true;

                boardSelectLocation = 0;
                myHandVLG.enabled = true;
        }

        else if (enemyTurn == true && boardLocationHolder[boardSelectLocation].spotTaken == false)
        {
            boardLocationHolder[boardSelectLocation].spotTaken = true;
            boardLocationHolder[boardSelectLocation].playerOwned = false;
            boardLocationHolder[boardSelectLocation].checkIfFlipped = true;
            boardLocationHolder[boardSelectLocation].cardHeld = enemyHandList[enemySelectLocation]; // testing for card implementation

            enemyHandCanvas[enemySelectLocation].overrideSorting = true;
            enemyHandCanvas[enemySelectLocation].sortingOrder = 0; //added to replace reset canvas
            enemyHandCanvas[enemySelectLocation].overrideSorting = false;//added to replace reset canvas

            enemyHand[enemySelectLocation].SetActive(false);
            enemyHandList.RemoveAt(enemySelectLocation);
            enemySelectList.RemoveAt(enemySelectLocation);
            enemyHand.RemoveAt(enemySelectLocation);
            enemyHandCanvas.RemoveAt(enemySelectLocation);
            
            enemyTransformedCardLocation = 0;
            justStarted = true;
            SoundManager.instance.PlaySFX(6);
            DisableBlinkingFinger();
            
            enemySelectLocation = 0;
            cardsOnBoard++;
            initializingFields = true;
            gameState = GameStates.CARD_SELECTION;
  
                playerTurn = true;
                enemyTurn = false;
                boardSelectLocation = 0;
                enemyHandVLG.enabled = true;
        }
        else if (boardLocationHolder[boardSelectLocation].spotTaken == true)
        {
            SoundManager.instance.PlaySFX(3);
        }
    }

    public void EnemyHandGenerator()
    {
        enemyHandList.Clear();
        enemyHandVLG.enabled = true;
        if (randomEnemyHand == false)
        {


            for (int i = 0; i < CardInventory.instance.enemyCards.Length; i++)
            {
                string cardLookup = CardInventory.instance.enemyCards[i];
                for (int j = 0; j < CardInventory.instance.masterCardList.Length; j++)
                {
                    if (cardLookup == CardInventory.instance.masterCardList[j].cardName)
                    {
                        enemyHandList.Add(CardInventory.instance.masterCardList[j]);
                    }
                }
            }
            for (int i = 0; i < enemyHandList.Count; i++)
            {
                enemyHandCanvas[i].overrideSorting = false;
                enemyHandCanvas[i].sortingOrder = 0;
            }
        }

        if(randomEnemyHand == true)
        {
            int cardsLeftToChoose = 5;
            for (int i = 0; i < cardsLeftToChoose; i++)
            {
                int cardToAdd = Random.Range(0, CardInventory.instance.masterCardList.Length);
                enemyHandList.Add(CardInventory.instance.masterCardList[cardToAdd]);
            }
        }

    }

    IEnumerator DisplayEnemyHand()
    {
        ResetCanvas();
        if (closedEnemyHand == false)
        {
            enemyHandVLG.enabled = true;
        for (int i = 0; i < enemyHandList.Count; i++)
        {
            int index = Random.Range(0, 5);
                enemyHandCanvas[i].overrideSorting = false;
                StartCoroutine(OneFrame()); // needed for sorting properly
                enemyHand[i].SetActive(true);
            SoundManager.instance.PlaySFX(4);
            enemyHand[i].GetComponent<Image>().sprite = enemyHandList[i].BImage;
            StartCoroutine(RotateCard(enemyHand[i]));
            yield return new WaitForSeconds(waitTime[index]);
        }
        gameState = GameStates.TURN_SELECTION;
            initializingFields = true;
        }
        else if (closedEnemyHand == true)
        {
            enemyHandVLG.enabled = true;
            for (int i = 0; i < enemyHandList.Count; i++)
            {
                int index = Random.Range(0, 5);
                enemyHandCanvas[i].overrideSorting = false;
                enemyHand[i].SetActive(true);
                SoundManager.instance.PlaySFX(4);
                //enemyHand[i].GetComponent<Image>().sprite = defaultCardBack;
                enemyHand[i].GetComponent<Image>().sprite = CardInventory.instance.emptyCard.AImage;
                yield return new WaitForSeconds(waitTime[index]);
            }
            gameState = GameStates.TURN_SELECTION;
            initializingFields = true;
        }
    }
    IEnumerator PickingCards()
    {
        Debug.Log("started picking the cards");
        pleaseWaitTextSelected = 0;
        Image thinkImg = thinkingPanel.GetComponent<Image>();
        for (int i = 0; i < pleaseWaitText.Length; i++)

        {
            thinkingPleaseWait.text = pleaseWaitText[i];
            yield return new WaitForSeconds(0.5f);
            thinkImg.color = new Color(0.5f, 0.5f, 0.5f, 0.5f);
            yield return new WaitForSeconds(0.5f);
            thinkImg.color = new Color(1f, 1f, 1f, 1f);
            pleaseWaitTextSelected++;
            if(pleaseWaitTextSelected == 1)
            {
                StartCoroutine(DisplayEnemyHand());
            }

        }

    }
    private void DisplayEnemyCards()
    {
        if(initializingFields == true)
        {
            InitializeFields(displayEnemyCardBool);
        }


    }


    public void UpdateScore()
    {
        currentPlayerScore = myHandList.Count;
        currentEnemyScore = enemyHandList.Count;
        for(int i = 0; i < boardLocationHolder.Length; i++)
        {
            if(boardLocationHolder[i].playerOwned == true && boardLocationHolder[i].spotTaken)
            {
                currentPlayerScore++; 
            }
            else if(boardLocationHolder[i].playerOwned == false && boardLocationHolder[i].spotTaken)
            {
                currentEnemyScore++;
            }
        }
        
        playerScore.text = scoreValues[currentPlayerScore].ToString();
        enemyScore.text = scoreValues[currentEnemyScore].ToString();
    }


    public void End()
    {
        flippedFinger.SetActive(false);
        if(initializingFields == true)
        {
            initializingFields = false;
            for (int i = 0; i < myHand.Count; i++)
            {
                if(myHand[i].activeInHierarchy == true)
                {
                    var spotRef = myHand[i].GetComponent<Canvas>();
                    spotRef.sortingOrder = 0;
                    spotRef.overrideSorting = false;
                }
            }
            for (int i = 0; i < enemyHand.Count; i++)
            {
                if (enemyHand[i].activeInHierarchy == true)
                {
                    var spotRef = enemyHand[i].GetComponent<Canvas>();
                    spotRef.sortingOrder = 0;
                    spotRef.overrideSorting = false;
                }
            }
            StartCoroutine( StartEndCo() );
            Debug.Log("supposed to start the endco here");
            
        }


        if ((Input.GetKeyDown(KeyCode.D) || (Input.GetButtonDown("right"))) && rematchSelectLocation < rematchSelectList.Count - 1)
        {
            rematchSelectLocation++;
            SoundManager.instance.PlaySFX(0);
        }
        if ((Input.GetKeyDown(KeyCode.A) || (Input.GetButtonDown("left"))) && rematchSelectLocation > 0)
        {
            rematchSelectLocation--;
            SoundManager.instance.PlaySFX(0);
        }
        if ((Input.GetKeyDown(KeyCode.Space) || (Input.GetButtonDown("Fire1"))) && rematchSelectLocation == 0)
        {

            gameState = GameStates.CHOOSE_CARDS;

            RestartGame();


        }
        if ((Input.GetKeyDown(KeyCode.Space) || (Input.GetButtonDown("Fire1"))) && rematchSelectLocation == 1)
        {

            SoundManager.instance.PlaySFX(1);
            StartCoroutine(EndCardBattleFade());

        }

        horizontalFinger.transform.position = rematchSelectList[rematchSelectLocation].transform.position;
    } //plays when game has ended

    private IEnumerator StartEndCo()
    {
        Debug.Log("started the startendco");
        if (currentPlayerScore > currentEnemyScore)
        {
            //SoundManager.instance.StopBGM();
            SoundManager.instance.PlayIntroLoop(3);
            StartCoroutine(PlayMusicQuick());
            /*
            //yield return new WaitForSeconds(0.25f);
            thinkingPleaseWait.text = winTitleText[0];
            throatPunchTextUI.text = rematchText[0];
            InitializeFields(endGameBool);
            GameManager.instance.afterCardBattleStatus = 0;
            */

        }

        else if (currentPlayerScore == currentEnemyScore)
        {
            yield return new WaitForSeconds(0.25f);
            thinkingPleaseWait.text = winTitleText[1];
            throatPunchTextUI.text = rematchText[1];
            InitializeFields(endGameBool);
            GameManager.instance.afterCardBattleStatus = 1;
        }

        else if (currentPlayerScore < currentEnemyScore)
        {
            yield return new WaitForSeconds(0.25f);
            thinkingPleaseWait.text = winTitleText[2];
            throatPunchTextUI.text = rematchText[2];
            InitializeFields(endGameBool);
            GameManager.instance.afterCardBattleStatus = 2;
        }
        
    }

    private void RestartGame()
    {
        //SoundManager.instance.StopBGM();
        if(GameManager.instance.ttActive == true)
        { SoundManager.instance.PlayIntroLoop(1); }

        ClearEnemyCardDisplay();
        myHandList.Clear();
        ClearCardDisplay();
        InitializeCardInformations();
        gameState = GameStates.CHOOSE_CARDS;
        cardConfirmLocation = 0;
        chooseCardLocation = 0;
        currentHandSize = 0;
        turnNotChosen = true;
        pleaseWaitTextSelected = 0;
        turnSelectionTextSelected = 0;
        cardsOnBoard = 0;
        movingDown = false;
        movingUp = false;
        cardSelectLocation = 0;
        enemySelectLocation = 0;
        boardSelectLocation = 0;
        enemyHandList.Clear();
        playerTurn = false;
        enemyTurn = false;
        transformedCardLocation = -1;
        playerScore.text = defaultTexts[0];
        enemyScore.text = defaultTexts[0];
        throatPunchTextUI.text = defaultTexts[1];
        InitializeCardInformations();
        ClearBoardImages();
        ClearPreviousCardLocations();
        EnemyHandGenerator();
        RegenerateHandUI();
        ResetCanvas();
        potentialBoardLocation.Clear();
        potentialCardLocation.Clear();
        HideScorePanel();
        initializingFields = true;
    }

    IEnumerator TurnSelection()
    {
        ResetCanvas();//added
        turnSelectionTextSelected = 0;
        playerTurn = false;
        enemyTurn = false;

        if(initializingFields == true)
        {
            InitializeFields(turnSelectionBool);
        }

        turnNotChosen = false;
        yield return new WaitForSeconds(0);

        horizontalFinger.transform.position = defaultFingerPosition.transform.position;
        flippedFinger.transform.position = defaultFingerPosition.transform.position;

        SoundManager.instance.PlaySFX(5);

        int whosTurnIsItAnyway = Random.Range(0, 2);
        Debug.Log("random number is " + whosTurnIsItAnyway);

        for (int i = 0; i < timesToBlink; i++)
        {
            if( i % 2 == 0)
            {
                throatPunchTextUI.text = turnSelectionText[turnSelectionTextSelected];
                turnSelectionTextSelected++;
            }
            horizontalFinger.SetActive(true);
            yield return new WaitForSeconds(0.1f);
            horizontalFinger.SetActive(false);
            flippedFinger.SetActive(true);
            yield return new WaitForSeconds(0.1f);
            flippedFinger.SetActive(false);
        }

        if(whosTurnIsItAnyway == 0)
        {
            horizontalFinger.SetActive(false);
            flippedFinger.SetActive(true);
            playerTurn = true;
            enemyTurn = false;
            justStarted = true;
            ShowScorePanel();
            gameState = GameStates.CARD_SELECTION;
            initializingFields = true;

        }
        else if (whosTurnIsItAnyway == 1)
        {
            horizontalFinger.SetActive(true);
            flippedFinger.SetActive(false);
            playerTurn = false;
            enemyTurn = true;
            justStarted = true;
            ShowScorePanel();
            gameState = GameStates.ENEMY_SELECTION;
            initializingFields = true;
        }
        else
        {
            Debug.Log("borked");
        }

    } //used for initial turn selection

     public IEnumerator RotateCard(GameObject thingToRotate)
    {
            int timesToRotate = 20;
            int speed = 5;
            Sprite referenceSprite = thingToRotate.GetComponent<Image>().sprite;
            thingToRotate.GetComponent<Image>().sprite = CardInventory.instance.emptyCard.AImage;
            for (int i = 0; i < timesToRotate; i++)

            {
                thingToRotate.transform.Rotate(0, speed, 0);
                Debug.Log("I just did the rotation");
                yield return new WaitForSeconds(0.01f);
            }
            thingToRotate.GetComponent<Image>().sprite = referenceSprite;

       thingToRotate.transform.rotation = new Quaternion(0, 0, 0, 0);
       
    } //used for hand picking card flips

    private void ClearBoardImages()
    {
        for (int i = 0; i < boardLocationHolder.Length; i++)
        {
            boardLocationHolder[i].spotTaken = false;
            boardLocationHolder[i].cardHeld = null;
            boardImages[i].color = new Color(0f, 0f, 0f, 0f);

        }
    }
    private void ClearPreviousCardLocations()
    {
        for(int i = 0; i < prevChosenCardLocation.Length; i++)
        {
            prevChosenCardLocation[i] = 0;
        }
    }
    private void ResetCanvas()
    {
        myHandVLG.enabled = true;
        enemyHandVLG.enabled = true;
        for(int i = 0; i < myHandCanvas.Count ; i++)
        {
            myHandCanvas[i].overrideSorting = true;
            myHandCanvas[i].sortingOrder = 0;
            StartCoroutine(OneFrame()); // needed for sorting properlyr
            myHandCanvas[i].overrideSorting = false;
            
        }
        for(int i = 0; i< enemyHandCanvas.Count; i++)
        {
            enemyHandCanvas[i].overrideSorting = true;
            StartCoroutine(OneFrame()); // needed for sorting properly
            enemyHandCanvas[i].sortingOrder = 0;
            enemyHandCanvas[i].overrideSorting = false;
            
            
        }
    }

    public void RememberHandUI()
    {
        for(int i = 0; i < myHand.Count; i++)
        {
            referenceMyHand.Add(myHand[i]);
        }
        for(int i = 0; i < enemyHand.Count; i++)
        {
            referenceEnemyHand.Add(enemyHand[i]);
        }
        for(int i = 0; i < cardSelectList.Count; i++)
        {
            referenceHandLocations.Add(cardSelectList[i]);
        }
        for(int i = 0; i < enemySelectList.Count; i++)
        {
            referenceEnemyHandLocations.Add(enemySelectList[i]);
        }
        for(int i = 0; i < myHandCanvas.Count; i++)
        {
            referenceMyHandCanvasList.Add(myHandCanvas[i]);
        }
        for(int i = 0; i < enemyHandCanvas.Count; i++)
        {
            referenceEnemyHandCanvasList.Add(enemyHandCanvas[i]);
        }

    }
    private void RegenerateHandUI()
    {
        myHand.Clear();
        enemyHand.Clear();
        myHandList.Clear();
        cardSelectList.Clear();
        enemySelectList.Clear();
        myHandCanvas.Clear();
        enemyHandCanvas.Clear();

        for(int i = 0; i < referenceMyHand.Count; i++)
        {
            myHand.Add(referenceMyHand[i]);
        }
        for(int i = 0; i < referenceEnemyHand.Count; i++)
        {
            enemyHand.Add(referenceEnemyHand[i]);
        }
        for(int i = 0; i < referenceHandLocations.Count; i++)
        {
            cardSelectList.Add(referenceHandLocations[i]);
        }
        for(int i = 0; i < referenceEnemyHandLocations.Count; i++)
        {
            enemySelectList.Add(referenceEnemyHandLocations[i]);
        }
        for (int i = 0; i < referenceMyHandCanvasList.Count; i++)
        {
            myHandCanvas.Add(referenceMyHandCanvasList[i]);
        }
        for (int i = 0; i < referenceEnemyHandCanvasList.Count; i++)
        {
            enemyHandCanvas.Add(referenceEnemyHandCanvasList[i]);
            enemyHandCanvas[i].sortingOrder = 0;
            enemyHandCanvas[i].overrideSorting = false;
        }
        enemyHandVLG.enabled = true;
    }

    private IEnumerator BeginnerCreatePotentialMoves()
    {
        enemySelectLocation = 0;
        potentialBoardLocation.Clear();
        potentialCardLocation.Clear();

        for (int i = 0; i < boardLocationHolder.Length; i++)
        {
            if (boardLocationHolder[i].spotTaken == true && boardLocationHolder[i].playerOwned == true)
            {
                BoardLocation referenceSpot = boardLocationHolder[i];
                Card referenceCard = boardLocationHolder[i].cardHeld;
                for (int j = 0; j < referenceSpot.boardAdjacencies.Length; j++)
                {
                    if (boardLocationHolder[referenceSpot.boardAdjacencies[j]].spotTaken == false)
                    {
                        int maybePutCardHere = boardLocationHolder[referenceSpot.boardAdjacencies[j]].myLocationNumber;
                        int hisSideToCheck = referenceSpot.myValueToCheck[j]; //test
                        int sideToCheck = referenceSpot.adjacencyValueToCheck[j];
                        for (int q = 0; q < enemyHandList.Count; q++)
                        {
                            if (referenceCard.cardValues[hisSideToCheck] < enemyHandList[q].cardValues[sideToCheck])
                            {
                                potentialCardLocation.Add(q);
                                potentialBoardLocation.Add(maybePutCardHere);
                            }
                        }
                    }
                }
            }
        }
        int rando = Random.Range(1, waitTime.Length);
        yield return new WaitForSeconds(1);
        int timesToScroll = ( enemyHandList.Count -1);
        for (int i = 0; i < timesToScroll; i++)
        {
            movingDown = true;
            yield return new WaitForSeconds(waitTime[rando]);
        }
       
        movingDown = false;
        enemyHand[enemyTransformedCardLocation].transform.Translate(translateAmount, 0, 0);
        enemyHandCanvas[enemyTransformedCardLocation].sortingOrder = 0;
        enemyHandCanvas[enemyTransformedCardLocation].overrideSorting = false;
        StartCoroutine(BeginnerChooseFromMoves());
    }

private IEnumerator BeginnerChooseFromMoves()
    {

        if(potentialCardLocation.Count == 0)
        {
            for (int i = 0; i < boardLocationHolder.Length; i++)
            {
                if(boardLocationHolder[i].spotTaken == false)
                {
                    potentialBoardLocation.Add(boardLocationHolder[i].myLocationNumber);
                }
            }

            int randoGuy = Random.Range(0, potentialBoardLocation.Count);
           boardSelectLocation = potentialBoardLocation[randoGuy];
            int randoGuy2 = Random.Range(0, enemyHandList.Count);
            enemySelectLocation = randoGuy2;
            enemyHandVLG.enabled = false;
            enemyHand[enemySelectLocation].transform.Translate(-translateAmount, 0, 0);
            enemyHandCanvas[enemySelectLocation].overrideSorting = true;
            enemyHandCanvas[enemySelectLocation].sortingOrder = 1;
            yield return new WaitForSeconds(1);
            PlaceCardOnBoard();
            Debug.Log("I had none in my list, so placed 'randomly' ");
        }
        else if(potentialCardLocation.Count == 1)
        {
            boardSelectLocation = potentialBoardLocation[0];
            enemySelectLocation = potentialCardLocation[0];
            enemyHandVLG.enabled = false;
            enemyHand[enemySelectLocation].transform.Translate(-translateAmount, 0, 0);
            enemyHandCanvas[enemySelectLocation].overrideSorting = true;
            enemyHandCanvas[enemySelectLocation].sortingOrder = 1;
            yield return new WaitForSeconds(1);
            PlaceCardOnBoard();
            Debug.Log("I had 1 in my list, so took it");
        }
        else
        {
            int randoGuy = Random.Range(0, potentialBoardLocation.Count);
            boardSelectLocation = potentialBoardLocation[randoGuy];
            enemySelectLocation = potentialCardLocation[randoGuy];
            Debug.Log("I had a lot in my list, so chose position 1 'randomly' ");
            enemyHandVLG.enabled = false;
            enemyHand[enemySelectLocation].transform.Translate(-translateAmount, 0, 0);
            enemyHandCanvas[enemySelectLocation].overrideSorting = true;
            enemyHandCanvas[enemySelectLocation].sortingOrder = 1;
            yield return new WaitForSeconds(1.5f);
            PlaceCardOnBoard();
        }
    }

    public bool CanFlipTwo()
    {
        if(potentialCardLocation.Count == 0 || potentialCardLocation.Count == 1)
        {
            return false;
        }
        else
        {
            for (int i = 0; i < potentialCardLocation.Count; i++)
            {

            }
        }
        return true;
    }

    public void ShowScorePanel()
    {
        myScorePanel.SetActive(true);
        enemyScorePanel.SetActive(true);
    }
    public void HideScorePanel()
    {
        myScorePanel.SetActive(false);
        enemyScorePanel.SetActive(false);
    }

    private IEnumerator OneFrame()
    {
        yield return null;
    }

    public void InitializeFields(bool[] listToCheck)
    {
        Debug.Log("just ran the initialze field, and the gamestate is" + gameState + "and the list was" + listToCheck);
        initializingFields = false;
        cardSelectionScreen.SetActive(listToCheck[0]);
        cardConfirmScreen.SetActive(listToCheck[1]);
        blinkingVerticalFinger.SetActive(listToCheck[2]);
        horizontalFinger.SetActive(listToCheck[3]);
        verticalFinger.SetActive(listToCheck[4]);
        blinkingFinger.SetActive(listToCheck[5]);
        flippedFinger.SetActive(listToCheck[6]);
        helpInfoPanel.SetActive(listToCheck[7]);
        thinkingPanel.SetActive(listToCheck[8]);
        rematchPanel.SetActive(listToCheck[9]);
    }

    public void InitializeCardSelect()
    {
        cardSelectionScreen.SetActive(true);
        //cardConfirmScreen.SetActive(listToCheck[1]);
        //blinkingVerticalFinger.SetActive(listToCheck[2]);
        horizontalFinger.SetActive(false);
        verticalFinger.SetActive(false);
        //blinkingFinger.SetActive(listToCheck[5]);
        flippedFinger.SetActive(false);
        helpInfoPanel.SetActive(false);
        thinkingPanel.SetActive(false);
        rematchPanel.SetActive(false);
    }
    private void TurnOnElements(bool variable)
    {
        if (variable == true)
        {
            backgroundIMG.SetActive(true);
            board.SetActive(true);
            cardSelect.SetActive(true);
            myHandGameObject.SetActive(true);
            enemyHandGameObject.SetActive(true);
            score.SetActive(true);
            fingers.SetActive(true);

        }
        else if (variable == false)
        {
            //SoundManager.instance.StopBGM();
            backgroundIMG.SetActive(false);
            board.SetActive(false);
            cardSelect.SetActive(false);
            myHandGameObject.SetActive(false);
            enemyHandGameObject.SetActive(false);
            score.SetActive(false);
            fingers.SetActive(false);
            rematchPanel.SetActive(false);
            thinkingPanel.SetActive(false);
            //SoundManager.instance.PlayIntroLoop(SoundManager.instance.bgmToPlay);
            //PlayerController.instance.canMove = true;
        }
        
    }

    public IEnumerator EndCardBattleFade()
    {
        //SoundManager.instance.StopBGM();
        SoundManager.instance.PlayIntroLoop(SoundManager.instance.bgmToPlay);
        TTfade.instance.FadeToBlack();
        yield return new WaitForSeconds(1f);
        TTfade.instance.FadeFromBlack();
        GameManager.instance.TurnOnRegularJoystick();
        GameManager.instance.ttActive = false;
        RestartGame();
        gameState = GameStates.DISABLED;
        TurnOnElements(false);
        GameManager.instance.dialogActive = false;
        //PlayerController.instance.canMove = false;
        yield return new WaitForSeconds(0.5f);
        GameManager.instance.afterCardBattle = true;
            
    }

    private IEnumerator PlayMusicQuick()
    {//made this to avoid the delay when winning

        yield return new WaitForSeconds(0.35f);
        thinkingPleaseWait.text = winTitleText[0];
        throatPunchTextUI.text = rematchText[0];
        InitializeFields(endGameBool);
        GameManager.instance.afterCardBattleStatus = 0;
    }















    //THIS IS ALL ADDITIONAL STUFF FOR ADDING IN THE CARD SELECTION



}
