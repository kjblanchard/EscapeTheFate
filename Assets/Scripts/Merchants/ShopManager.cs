using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using static  Input;

public class ShopManager : MonoBehaviour
{
    public static ShopManager instance;


    
    [SerializeField] public TypeText shopDialog;
    
    
    public enum ShopStates
    {
        DISABLED,
        TITLE_SCREEN,
        BUYING,
        BUY_CONFIRM,
        SELLING,
        SELLING_CONFIRM,

    }
    
    public ShopStates shopState;
    
    [Header("Fingers")]
    public GameObject finger;
    public GameObject blinkingFinger;

    private int equipmentFingerCounter;
    
    [Header("Finger Locations")]

    public List<GameObject> titleScreenList = new List<GameObject>();
    public int titleScreenCurrentOption;
    public List<GameObject> buyingScreenList = new List<GameObject>();
    public int buyingScreenCurrentOption;
    public List<GameObject> buyConfirmList = new List<GameObject>();
    public int buyConfirmCurrentOption;
    public List<GameObject> sellingScreenList = new List<GameObject>();
    public int sellingScreenCurrentOption;
    public List<GameObject> sellConfirmList = new List<GameObject>();
    public int sellConfirmCurrentOption;
    
    
    [Header("Panels")]
    public GameObject alertGameObject;
    [SerializeField] GameObject shopGameObject;
    [SerializeField] GameObject shopBackgroundColor;
    [SerializeField] private GameObject titleScreen;
    [SerializeField] private GameObject weaponBuyScreen;
    [SerializeField] private GameObject weaponConfirmBuyScreen;



    [Header("ScrollRects")]

    [SerializeField] private ScrollRect equipmentScrollRect;

    /*
    [Header("ScrollRects")]
    
    public ScrollRect buyingScrollRect;
    public ScrollRect sellingScrollRect;
 

    private int buyingFingerCounter = 1;
    private int sellingFingerCounter = 1;
       */

    [Header("State Controls")]
    public bool initializingFields;
    public bool shopOpen = false;
    private bool turnOnFinger = false;

    [Header("Animators")] 
    [SerializeField] private Animator titleScreenTopHalfAnimator;
    [SerializeField] private Animator titleScreenBottomHalfAnimator;

    [SerializeField] private Animator weaponBuyScreenTop;
    [SerializeField] private Animator weaponBuyScreenRight;
    [SerializeField] private Animator weaponBuyScreenBottom;
    [SerializeField] private Animator weaponBuyScreenLeft;





    void Awake()
    {

        {
            instance = this;
            int shopMenusCount = FindObjectsOfType<GameMenus>().Length;
            if (shopMenusCount > 1)
            {
                Destroy(gameObject);
            }
            else
            {
                DontDestroyOnLoad(gameObject);
            }
        }

    }

    private void Update()
    {
        switch (shopState)
        {
            case ShopStates.DISABLED:
                Disabled();
                break;
            case ShopStates.TITLE_SCREEN:
                TitleScreen();
                break;
            case ShopStates.BUYING:
                Buying();
                break;
            case ShopStates.BUY_CONFIRM:
                BuyingConfirm();
                break;
            case ShopStates.SELLING:
                Selling();
                break;
            case ShopStates.SELLING_CONFIRM:
                SellingConfirm();
                break;



        }
    }

    public void TurnOn()
    {
        shopBackgroundColor.SetActive(true);
        shopGameObject.SetActive(true);
        shopOpen = true;
        ShopUI.instance.EquipmentShopRecreateList();
        shopState = ShopStates.TITLE_SCREEN;
        initializingFields = true;
    }

    public void Turnoff()
    {
        shopBackgroundColor.SetActive(false);
        shopGameObject.SetActive(false);
        shopOpen = false;
    }

    private void Disabled()
    {
        if (!GameManager.instance.shouldCloseShop) return;
        GameManager.instance.shouldCloseShop = false;
        StopAllCoroutines();
        shopDialog.ChangeTextGoodbye();
        StartCoroutine( UIFade.instance.ShopCloserFade());
        GameManager.instance.TurnOnRegularJoystick();//could possibly take this out?
        Debug.Log("i'm doing the close");
    }
    private void TitleScreen()
    {
        if (initializingFields)
        {
            initializingFields = false;
            ShopUI.instance.UpdateGilUI();

            shopBackgroundColor.SetActive(true);
            shopGameObject.SetActive(true);
            titleScreen.SetActive(true);
            weaponBuyScreen.SetActive(false);
            weaponConfirmBuyScreen.SetActive(false);
            finger.SetActive(false);
            TitleScreenAnimators(true);

        }
        if ((GetKeyDown(KeyCode.A) || (GetButtonDown("left"))) && titleScreenCurrentOption > 0 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            titleScreenCurrentOption--;
            SoundManager.instance.PlaySFX(0);

        }
        else if ((GetKeyDown(KeyCode.D) || (GetButtonDown("right"))) && titleScreenCurrentOption < titleScreenList.Count - 1 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            titleScreenCurrentOption++;
            SoundManager.instance.PlaySFX(0);
        }
        else if (GetKeyDown(KeyCode.C) && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            shopState = ShopStates.DISABLED;
            GameManager.instance.shouldCloseShop = true;
            
        }
        if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && titleScreenCurrentOption == 0 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            TitleScreenAnimators(false);
            shopState = ShopStates.BUYING;
            initializingFields = true;
            SoundManager.instance.PlaySFX(0);
            StartCoroutine(BuyingScreenDelay(true));



        }
        else if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && titleScreenCurrentOption == 1 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            
            shopState = ShopStates.SELLING;
            initializingFields = true;
            SoundManager.instance.PlaySFX(0);
        }

        blinkingFinger.transform.position = titleScreenList[titleScreenCurrentOption].transform.position;
    }

    private void Buying()
    {
        if (initializingFields)
        {
            initializingFields = false;
            //blinkingFinger.SetActive(false);
            buyConfirmCurrentOption = 0;

            //equipmentFingerCounter = 1;
            //equipmentScrollRect.verticalNormalizedPosition += (float)1 / (ShopUI.instance.equipmentHolderList.Count - 5);

            alertGameObject.SetActive(false);
            if (turnOnFinger)
            {
                turnOnFinger = false;
                finger.transform.position = ShopUI.instance.equipmentHolderList[buyingScreenCurrentOption].GetComponent<ItemHolder>().fingerPoint.transform.position;
                finger.gameObject.SetActive(true);
            }
            ShopUI.instance.EquipmentChecker(ShopUI.instance.shopEquipmentList[buyingScreenCurrentOption]);
            ShopUI.instance.charHolderReference.EquipmentComparison(ShopUI.instance.shopEquipmentList[buyingScreenCurrentOption]);




        }

        if (InventoryUI.instance.itemHolderList.Count >= 1)
        {
            if ((GetKeyDown(KeyCode.W) || (GetButtonDown("up"))) && buyingScreenCurrentOption > 0 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
            {
                buyingScreenCurrentOption--;

                if (equipmentFingerCounter > 1)
                {
                    equipmentFingerCounter--;
                }
                else if (equipmentFingerCounter == 1)
                {
                   equipmentScrollRect.verticalNormalizedPosition += (float)1 / (ShopUI.instance.equipmentHolderList.Count - 5);
                }
                ShopUI.instance.EquipmentChecker(ShopUI.instance.shopEquipmentList[buyingScreenCurrentOption]);
                ShopUI.instance.charHolderReference.EquipmentComparison(ShopUI.instance.shopEquipmentList[buyingScreenCurrentOption]);
                SoundManager.instance.PlaySFX(0);
                //InventoryUI.instance.descriptionTextField.text = Inventory.instance.inventoryList[inventoryCurrentOption].item.description;
            }

            else if ((GetKeyDown(KeyCode.S) || (GetButtonDown("down"))) && buyingScreenCurrentOption < ShopUI.instance.equipmentHolderList.Count - 1 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
            {
                buyingScreenCurrentOption++;

                if (equipmentFingerCounter < 5)
                {
                    equipmentFingerCounter++;
                }
                else if (equipmentFingerCounter == 5)
                {
                    equipmentScrollRect.verticalNormalizedPosition -= (float)1 / (ShopUI.instance.equipmentHolderList.Count - 5);
                }

                ShopUI.instance.EquipmentChecker(ShopUI.instance.shopEquipmentList[buyingScreenCurrentOption]);
                ShopUI.instance.charHolderReference.EquipmentComparison(ShopUI.instance.shopEquipmentList[buyingScreenCurrentOption]);
                SoundManager.instance.PlaySFX(0);
               // InventoryUI.instance.descriptionTextField.text = Inventory.instance.inventoryList[inventoryCurrentOption].item.description;
            }


        }
        if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            ShopUI.instance.UpdateEquipmentConfirmScreen(ShopUI.instance.shopEquipmentList[buyingScreenCurrentOption]);
            shopState = ShopStates.BUY_CONFIRM;
            initializingFields = true;
            SoundManager.instance.PlaySFX(0);
        }
        else if (GetKeyDown(KeyCode.C) && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            finger.SetActive(false);
            buyingScreenCurrentOption = 0;
            equipmentFingerCounter = 1;
            shopState = ShopStates.TITLE_SCREEN;
            initializingFields = true;
            WeaponBuyAnimators(false);
            ShopManager.instance.shopDialog.dialogText.alpha = 1;
            StartCoroutine(BuyingScreenDelay(false));

        }

        finger.transform.position = ShopUI.instance.equipmentHolderList[buyingScreenCurrentOption].GetComponent<ItemHolder>().fingerPoint.transform.position;

        //finger.transform.position = buyingScreenList[buyingScreenCurrentOption].transform.position;
    }

    private void BuyingConfirm()
    {
        if (initializingFields)
        {
            initializingFields = false;
            titleScreen.SetActive(false);
            weaponBuyScreen.SetActive(true);
            weaponConfirmBuyScreen.SetActive(true);
            blinkingFinger.SetActive(false);
            finger.SetActive(true);
        }
        
        if ((GetKeyDown(KeyCode.A) || (GetButtonDown("left"))) && buyConfirmCurrentOption > 0 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            buyConfirmCurrentOption--;
            SoundManager.instance.PlaySFX(0);

        }
        else if ((GetKeyDown(KeyCode.D) || (GetButtonDown("right"))) && buyConfirmCurrentOption < buyConfirmList.Count - 1 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            buyConfirmCurrentOption++;
            SoundManager.instance.PlaySFX(0);

        }
        else if ((GetKeyDown(KeyCode.C) || (GetButtonDown("Fire2")))&& GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            finger.SetActive(false);
            turnOnFinger = true;
            initializingFields = true;
            shopState = ShopStates.BUYING;
            weaponConfirmBuyScreen.SetActive(false);
            SoundManager.instance.PlaySFX(1);


        }
        else if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor && buyConfirmCurrentOption == 0)
        {
            ShopUI.instance.EquipmentBuyAttempt(buyingScreenCurrentOption);
        }

            finger.transform.position = buyConfirmList[buyConfirmCurrentOption].transform.position;

    }

    private void Selling()
    {
        if (initializingFields)
        {
            //load the right panels
            initializingFields = false;
        }
    }

    private void SellingConfirm()
    {
        if (initializingFields)
        {
            //load the right panels
            initializingFields = false;
        }
    }

    private IEnumerator BuyingScreenDelay(bool fromTitleScreen)
    {
        if (fromTitleScreen)
        {
            equipmentScrollRect.verticalNormalizedPosition += (float)1 / (InventoryUI.instance.itemHolderList.Count - 5);

            equipmentFingerCounter = 1;
            GameManager.instance.canMoveCursor = false;
            blinkingFinger.SetActive(false);
            finger.SetActive(false);
            buyingScreenCurrentOption = 0;
            yield return new WaitForSeconds(0.25f);
            titleScreen.SetActive(false);
            weaponBuyScreen.SetActive(true);
            weaponConfirmBuyScreen.SetActive(false);
            //blinkingFinger.SetActive(false);
            WeaponBuyAnimators(true);
            yield return new WaitForSeconds(0.4f);
            finger.SetActive(true);
            GameManager.instance.canMoveCursor = true;
            
        }
        else if (!fromTitleScreen)
        {
            finger.SetActive(false);
            GameManager.instance.canMoveCursor = true;
            yield return new WaitForSeconds(0.25f);
            /*
            titleScreen.SetActive(true);
            weaponBuyScreen.SetActive(false);
            weaponConfirmBuyScreen.SetActive(false);
            blinkingFinger.SetActive(false);
            finger.SetActive(true);
            */
            TitleScreenAnimators(true);
            yield return new WaitForSeconds(0.25f);
            if(shopState == ShopStates.TITLE_SCREEN)
            { blinkingFinger.SetActive(true); }
            
            GameManager.instance.canMoveCursor = true;
        }
    }

    private void TitleScreenAnimators(bool opening)
    {
        if (opening)
        {
            titleScreenTopHalfAnimator.SetBool("isOpen",true);
            titleScreenBottomHalfAnimator.SetBool("isOpen",true);
        }

        else if (!opening)
        {
            titleScreenTopHalfAnimator.SetBool("isOpen",false);
            titleScreenBottomHalfAnimator.SetBool("isOpen",false);
        }
    }

    private void WeaponBuyAnimators(bool opening)
    {
        if (opening)
        {
            weaponBuyScreenTop.SetBool("isOpen",true);
            weaponBuyScreenRight.SetBool("isOpen",true);
            weaponBuyScreenBottom.SetBool("isOpen",true);
            weaponBuyScreenLeft.SetBool("isOpen",true);

        }
        else if (!opening)
        {
            weaponBuyScreenTop.SetBool("isOpen",false);
            weaponBuyScreenRight.SetBool("isOpen",false);
            weaponBuyScreenBottom.SetBool("isOpen",false);
            weaponBuyScreenLeft.SetBool("isOpen",false);
        }
    }
}
