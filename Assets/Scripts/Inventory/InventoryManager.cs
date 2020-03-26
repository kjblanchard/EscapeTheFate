using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;
using UnityEngine.UI;
using static Input;

public class InventoryManager : MonoBehaviour
{
    public static InventoryManager instance;

    

    public enum InventoryStates
    {
        DISABLED,
        OPTIONS_PANEL,
        EQUIPMENT_PLAYER_SELECT,
        EQUIPMENT_PANEL,
        EQUIPMENT_TYPE_SELECT,
        EQUIPMENT_WEAPON_SELECT,
        INVENTORY_PANEL,
        INVENTORY_USE,
        INVENTORY_ORGANIZE,
        MATERIA_PLAYER_SELECT,
        MATERIA_PANEL,
        MATERIA_EDIT_SELECTION,
        MATERIA_SELECTION,

    }

    public InventoryStates inventoryState;

    [Header("Fingers")]
    public GameObject horizontalFinger;
    public GameObject verticalFinger;
    public GameObject blinkingFinger;
    public GameObject blinkingVerticalFinger;

    [Header("Finger Locations")]

    public List<GameObject> optionsList = new List<GameObject>();
    public int currentOption = 0;

    public List<GameObject> inventoryOptionsList = new List<GameObject>();
    public int inventoryCurrentOption = 0;

    public List<GameObject> playerOptionsList = new List<GameObject>();
    public int playerCurrentOption = 0;

    public List<GameObject> weaponOptionsList = new List<GameObject>();
    public int weaponCurrentOption = 0;

    public List<GameObject> equipmentTypeList = new List<GameObject>();
    private int equipmentTypeCurrentOption = 0;

    public List<GameObject> materiaEquipmentList = new List<GameObject>();
    private int materiaEquipCurrentOption = 0;

    public List<GameObject> weaponMateriaList = new List<GameObject>();
    private int weaponMateriaSelect = 0;

    public List<GameObject> armorMateriaList = new List<GameObject>();
    private int armorMateriaSelect = 0;

    public List<GameObject> accMateriaList = new List<GameObject>();
    private int accMateriaSelect = 0;

    public List<GameObject> materiaSelectList = new List<GameObject>();
    public int materiaSelectCurrentOption = 0;



    [Header("Panels")]
    public GameObject equipmentPanel;
    public GameObject optionsPanel;
    public GameObject inventoryPanel;
    public GameObject equipControlPanel;
    public GameObject player1Panel;
    public GameObject player2Panel;
    public GameObject player3Panel;
    public GameObject charSelectedPanel;
    public GameObject charInfoPanel;
    public GameObject currencyPanel;
    public GameObject materiaPanel;
    public CanvasGroup charSelectCanvasGroup;
    public CanvasGroup statModifierCanvasGroup;
    public CanvasGroup[] charPanelCanvasGroup;
    

    private int selectedItem = -1;

    public ScrollRect itemScrollRect;
    public ScrollRect weaponScrollRect;
    public ScrollRect materiaScrollRect;

    private int fingerCounter = 1;
    private int weaponFingerCounter = 1;
    public int materiaFingerCounter = 1;

    [Header("Moving Windows")]

    public GameObject charOneSelectedPanel;
    public CanvasGroup charOneCanvasGroup;
    public Animation charOneAnimation;

    public bool initializingFields;

    [Header("What to Search For")]

    private Equipment.Types typeToSearchFor;
    private Equipment.UsableBy usableToSearchFor;

    [Header("Materia Bools")]
    public bool materiaWeapon = false;
    public bool materiaArmor = false;
    public bool materiaAcc = false;





    private void Awake()
    {
        instance = this;

        int inventoryManagerCount = FindObjectsOfType<InventoryManager>().Length;
        if (inventoryManagerCount > 1)
        {
            Destroy(gameObject);
        }
        else
        {
            DontDestroyOnLoad(gameObject);
        }
    }

    // Start is called before the first frame update
    void Start()
    {

        GameMenus.instance.ClearCharInfoPanel();
        //anim = gameObject.GetComponent<Animation>();
    }

    // Update is called once per frame
    void Update()
    {


        switch (inventoryState)
        {
            case InventoryStates.DISABLED:
                Disabled();
                break;
            case InventoryStates.OPTIONS_PANEL:

                OptionsSelect();
                break;
            case InventoryStates.INVENTORY_PANEL:
                InventorySelect();
                break;

            case InventoryStates.INVENTORY_USE:
                UseAndOrder();
                break;

            case InventoryStates.INVENTORY_ORGANIZE:
                Inventory.instance.OrganizeItems();
                inventoryState = InventoryStates.INVENTORY_PANEL;
                break;
            case InventoryStates.EQUIPMENT_PANEL:
                EquipScreen();

                break;
            case InventoryStates.EQUIPMENT_PLAYER_SELECT:
                PlayerSelect();
                break;

            case InventoryStates.EQUIPMENT_TYPE_SELECT:
                EquipTypeSelect();
                break;

            case InventoryStates.EQUIPMENT_WEAPON_SELECT:
                WeaponSelect();
                break;

            case InventoryStates.MATERIA_PLAYER_SELECT:
                MatPlayerSelect();
                break;

            case InventoryStates.MATERIA_PANEL:
                MatPanelSelect();
                break;

            case InventoryStates.MATERIA_EDIT_SELECTION:
                MatEditSelect();
                break;

            case InventoryStates.MATERIA_SELECTION:
                MatSelection();
                break;

        }
    }


    void Disabled()
    {
        if (!GameManager.instance.shouldCloseMenu) return;
        GameManager.instance.shouldCloseMenu = false;
        horizontalFinger.SetActive(false);
        verticalFinger.SetActive(false);
        blinkingFinger.SetActive(false);
        blinkingVerticalFinger.SetActive(false);

        inventoryPanel.SetActive(false);
        equipmentPanel.SetActive(false);
        optionsPanel.SetActive(false);
        InventoryUI.instance.timerUpdaterIsRunning = false;
    }
    void OptionsSelect()
    {
        if (initializingFields)
        {

            initializingFields = false;
            GameMenus.instance.UpdateMainMenuCharacterPanels();
            playerCurrentOption = 0;

            horizontalFinger.SetActive(false);
            verticalFinger.SetActive(true);
            blinkingFinger.SetActive(false);
            blinkingVerticalFinger.SetActive(false);

            inventoryPanel.SetActive(false);
            equipmentPanel.SetActive(true);
            optionsPanel.SetActive(true);
            charInfoPanel.SetActive(true);
            currencyPanel.SetActive(true);
            GameManager.instance.canMoveCursor = true;
            materiaPanel.SetActive(false);
            




        }




        if ((GetKeyDown(KeyCode.A) || (GetButtonDown("left"))) && currentOption > 0 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            currentOption--;
            SoundManager.instance.PlaySFX(0);

        }
        else if ((GetKeyDown(KeyCode.D) || (GetButtonDown("right"))) && currentOption < optionsList.Count - 1 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            currentOption++;
            SoundManager.instance.PlaySFX(0);
        }

        if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && currentOption == 0 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            inventoryState = InventoryStates.EQUIPMENT_PLAYER_SELECT;
            initializingFields = true;
            SoundManager.instance.PlaySFX(0);
        }
        else if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && currentOption == 1 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            
            inventoryState = InventoryStates.INVENTORY_PANEL;
            initializingFields = true;
            SoundManager.instance.PlaySFX(0);
        }
        else if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && currentOption == 2 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            inventoryState = InventoryStates.MATERIA_PLAYER_SELECT;
            initializingFields = true;
            SoundManager.instance.PlaySFX(0);
        }
        else if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && currentOption == 3 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            GameManager.instance.canMoveCursor = false;
            StartCoroutine(UIFade.instance.MenuCloserFade());

        }
        else if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && currentOption == 4 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            SoundManager.instance.PlaySFX(3);
        }
        verticalFinger.transform.position = optionsList[currentOption].transform.position;
        if((GetKeyDown(KeyCode.C) || (GetButtonDown("Fire2"))) && currentOption != 3 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            SoundManager.instance.PlaySFX(1);
            currentOption = 3;
        }
    }

    public void OptionsMoveCursorToPosition(int futureFingerPosition)
    {
        if (currentOption != futureFingerPosition) currentOption = futureFingerPosition;
    }

    public void OptionsButtonClick()
    {
        if (currentOption == 1)
        {
            SoundManager.instance.PlaySFX(0);
            StartCoroutine(InventoryUI.instance.FadeIn(InventoryStates.INVENTORY_PANEL));
        }
        else if (currentOption != 1)
        {
            SoundManager.instance.PlaySFX(3);
        }
    }


    private void InventorySelect()
    {
        if(initializingFields)
        {
            initializingFields = false;
            horizontalFinger.SetActive(true);
            verticalFinger.SetActive(false);
            blinkingFinger.SetActive(false);
            blinkingVerticalFinger.SetActive(false);

            inventoryPanel.SetActive(true);
            equipmentPanel.SetActive(true);//maybe should switch this to gain fps
            optionsPanel.SetActive(false);
            inventoryCurrentOption = 0;
            fingerCounter = 1;
            itemScrollRect.verticalNormalizedPosition += (float)1 / (InventoryUI.instance.itemHolderList.Count - 5);
            GameManager.instance.isLoading = true;
            StartCoroutine(GameMenus.instance.TurnOnItemPanelAnimation(true));
            Debug.Log("just did this");
        }


        if ((GetKeyDown(KeyCode.A) || (GetButtonDown("left"))) && inventoryCurrentOption > 0 &&
            GameManager.instance.isLoading == false)
        {
            inventoryCurrentOption--;
            SoundManager.instance.PlaySFX(0);
        }
        else if ((GetKeyDown(KeyCode.D) || (GetButtonDown("right"))) && inventoryCurrentOption < inventoryOptionsList.Count - 1 && GameManager.instance.isLoading == false)
        {
            inventoryCurrentOption++;
            SoundManager.instance.PlaySFX(0);
        }
        if ((GetKeyDown(KeyCode.C) || (GetButtonDown("Fire2"))) && GameManager.instance.isLoading == false)
        {
            SoundManager.instance.PlaySFX(1);
            StartCoroutine(GameMenus.instance.TurnOnItemPanelAnimation(false)) ;
            
            /*
            inventoryState = InventoryStates.OPTIONS_PANEL;
            initializingFields = true;
            */
        }
        else if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && inventoryCurrentOption == 0 && GameManager.instance.isLoading == false)
        {
            SoundManager.instance.PlaySFX(0);
            inventoryState = InventoryStates.INVENTORY_USE;
            initializingFields = true;
            InventoryUI.instance.descriptionTextField.text = Inventory.instance.inventoryList[inventoryCurrentOption].item.description;
            fingerCounter = 1;
        }
        else if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && inventoryCurrentOption == 1 && GameManager.instance.isLoading == false)
        {
            SoundManager.instance.PlaySFX(0);
            inventoryState = InventoryStates.INVENTORY_ORGANIZE;
        }
        horizontalFinger.transform.position = inventoryOptionsList[inventoryCurrentOption].transform.position;
    }

    void UseAndOrder()
    {
        if(initializingFields)
        {
            initializingFields = false;

        }

        if ((GetKeyDown(KeyCode.C) || (GetButtonDown("Fire2"))) && GameManager.instance.isLoading == false)
        {
            horizontalFinger.SetActive(false);
            inventoryCurrentOption = 0;
            fingerCounter = 1;
            itemScrollRect.verticalNormalizedPosition += (float)1 / (InventoryUI.instance.itemHolderList.Count - 5);
            SoundManager.instance.PlaySFX(1);
            inventoryState = InventoryStates.INVENTORY_PANEL;
            initializingFields = true;

        }

        if (InventoryUI.instance.itemHolderList.Count >= 1)
        {
            if ((GetKeyDown(KeyCode.W) || (GetButtonDown("up"))) && inventoryCurrentOption > 0 && GameManager.instance.isLoading == false)
            {
                inventoryCurrentOption--;

                if (fingerCounter > 1)
                {
                    fingerCounter--;
                }
                else if (fingerCounter == 1)
                {
                    itemScrollRect.verticalNormalizedPosition += (float)1 / (InventoryUI.instance.itemHolderList.Count - 5);
                }

                SoundManager.instance.PlaySFX(0);
                InventoryUI.instance.descriptionTextField.text = Inventory.instance.inventoryList[inventoryCurrentOption].item.description;
            }

            else if ((GetKeyDown(KeyCode.S) || (GetButtonDown("down"))) && inventoryCurrentOption < InventoryUI.instance.itemHolderList.Count - 1 && GameManager.instance.isLoading == false)
            {
                inventoryCurrentOption++;

                if (fingerCounter < 5)
                {
                    fingerCounter++;
                }
                else if (fingerCounter == 5)
                {
                    itemScrollRect.verticalNormalizedPosition -= (float)1 / (InventoryUI.instance.itemHolderList.Count - 5);
                }


                SoundManager.instance.PlaySFX(0);
                InventoryUI.instance.descriptionTextField.text = Inventory.instance.inventoryList[inventoryCurrentOption].item.description;
            }


        }

        if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && selectedItem == -1)
        {
            blinkingFinger.SetActive(true);
            blinkingFinger.transform.position = horizontalFinger.transform.position;
            selectedItem = inventoryCurrentOption;
        }
        else if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && (selectedItem != -1) && (selectedItem != inventoryCurrentOption))
        {
            //Rearrange ITEMS
            Inventory.instance.SwapItems(selectedItem, inventoryCurrentOption);
            selectedItem = -1;
            blinkingFinger.SetActive(false);
        }

        horizontalFinger.transform.position = InventoryUI.instance.itemHolderList[inventoryCurrentOption].GetComponent<ItemHolder>().fingerPoint.transform.position;
    }

    void PlayerSelect()
    {

        if (initializingFields == true)
        {
            initializingFields = false;
            horizontalFinger.SetActive(false);// moved this to bottom if can move cursor
            verticalFinger.SetActive(false);
            blinkingFinger.SetActive(false);
            blinkingVerticalFinger.SetActive(true);

            inventoryPanel.SetActive(false);
            equipmentPanel.SetActive(true);
            optionsPanel.SetActive(true);

            player1Panel.SetActive(true);
            player2Panel.SetActive(true);
            player3Panel.SetActive(true);

            equipControlPanel.SetActive(false);
            GameMenus.instance.UpdateCharInfoPanel(playerCurrentOption);
            GameMenus.instance.DisplaySelectedItemInInfoPanel(true, playerCurrentOption);
            weaponCurrentOption = 0;

        }


        if ((GetKeyDown(KeyCode.W) || (GetButtonDown("up"))) && playerCurrentOption > 0 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor )
        {
            playerCurrentOption--;
            GameMenus.instance.UpdateCharInfoPanel(playerCurrentOption);
            SoundManager.instance.PlaySFX(0);
            GameMenus.instance.DisplaySelectedItemInInfoPanel(true, playerCurrentOption);
        }

        else if ((GetKeyDown(KeyCode.S) || (GetButtonDown("down"))) && playerCurrentOption < playerOptionsList.Count - 1 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            playerCurrentOption++;
            GameMenus.instance.UpdateCharInfoPanel(playerCurrentOption);
            GameMenus.instance.DisplaySelectedItemInInfoPanel(true, playerCurrentOption);
            SoundManager.instance.PlaySFX(0);
        }
        else if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) /*&& playerCurrentOption == 0*/ && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            GameMenus.instance.CharSelected(playerCurrentOption);
            //GameMenus.instance.charSelectedStatsAnimator.SetBool("isSelected", true);
            StartCoroutine(GameMenus.instance.ShowCharStats(true));
            usableToSearchFor = GameManager.instance.playerGameObjects[playerCurrentOption].GetComponent<CharStatsHolder>().currentUseBy;
            inventoryState = InventoryStates.EQUIPMENT_PANEL;
            initializingFields = true;
            GameMenus.instance.characterSelected = playerCurrentOption;
        }

        if ((GetKeyDown(KeyCode.C) || (GetButtonDown("Fire2"))) && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            inventoryState = InventoryStates.OPTIONS_PANEL;
            initializingFields = true;
            SoundManager.instance.PlaySFX(1);
        }

        if (GameManager.instance.canMoveCursor) { horizontalFinger.SetActive(true); }
        blinkingVerticalFinger.transform.position = optionsList[currentOption].transform.position;
        horizontalFinger.transform.position = playerOptionsList[playerCurrentOption].transform.position;
    }

    void EquipScreen()
    {

        if(initializingFields == true)
        {
            initializingFields = false;
            horizontalFinger.SetActive(false);
            verticalFinger.SetActive(false);
            blinkingFinger.SetActive(false);
            blinkingVerticalFinger.SetActive(false);

            inventoryPanel.SetActive(false);
            equipmentPanel.SetActive(true);
            optionsPanel.SetActive(false);
            equipControlPanel.SetActive(true);
            equipmentTypeCurrentOption = 0;
            typeToSearchFor = Equipment.Types.WEAPON;
            InventoryUI.instance.WeaponRecreateList();
        }





        if ((GetKeyDown(KeyCode.D) || (GetButtonDown("right"))) && weaponCurrentOption < weaponOptionsList.Count -1 && GameManager.instance.canMoveCursor)
        {
            weaponCurrentOption++;
            SoundManager.instance.PlaySFX(0);
        }
        else if ((GetKeyDown(KeyCode.A) || (GetButtonDown("left"))) && weaponCurrentOption > 0 && GameManager.instance.canMoveCursor)
        {
            weaponCurrentOption--;
            SoundManager.instance.PlaySFX(0);
        }
        else if (( (GetKeyDown(KeyCode.C)) || (GetButtonDown("Fire2"))) && GameManager.instance.canMoveCursor)
        {
            GameMenus.instance.CharDeSelected();
            StartCoroutine(GameMenus.instance.ShowCharStats(false));
            inventoryState = InventoryStates.EQUIPMENT_PLAYER_SELECT;
            initializingFields = true;
            SoundManager.instance.PlaySFX(1);
            GameMenus.instance.TurnOffStats();
            
        }

        if((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && weaponCurrentOption == 0 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            typeToSearchFor = Equipment.Types.WEAPON;
            inventoryState = InventoryStates.EQUIPMENT_TYPE_SELECT;
            initializingFields = true;
            SoundManager.instance.PlaySFX(0);

        }
        if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && weaponCurrentOption == 2 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            GameMenus.instance.CharDeSelected();
            StartCoroutine(GameMenus.instance.ShowCharStats(false));
            inventoryState = InventoryStates.EQUIPMENT_PLAYER_SELECT;
            initializingFields = true;
            SoundManager.instance.PlaySFX(1);
            GameMenus.instance.TurnOffStats();

        }

        if (GameManager.instance.canMoveCursor) { horizontalFinger.SetActive(true); }
        horizontalFinger.transform.position = weaponOptionsList[weaponCurrentOption].transform.position;
    }



    void EquipTypeSelect()
    {

        if(initializingFields == true)
        {
            GameMenus.instance.ClearCharInfoPanel();
            initializingFields = false;
            weaponCurrentOption = 0;
            horizontalFinger.SetActive(true);
            verticalFinger.SetActive(false);
            blinkingFinger.SetActive(false);
            blinkingVerticalFinger.SetActive(false);

            inventoryPanel.SetActive(false);
            equipmentPanel.SetActive(true);
            optionsPanel.SetActive(false);
            equipControlPanel.SetActive(false);

            //InventoryUI.instance.FillWeaponList();
            GameMenus.instance.DisplaySelectedItemInInfoPanel(true, equipmentTypeCurrentOption);
            InventoryUI.instance.CustomEquipmentList(typeToSearchFor, usableToSearchFor);
        }

        if((GetKeyDown(KeyCode.W) || (GetButtonDown("up"))) && GameManager.instance.isLoading == false && equipmentTypeCurrentOption > 0)
        {
            equipmentTypeCurrentOption--;
            //InventoryUI.instance.CustomEquipmentList(Equipment.Types.WEAPON, usableToSearchFor);
            SoundManager.instance.PlaySFX(0);
            GameMenus.instance.DisplaySelectedItemInInfoPanel(true, equipmentTypeCurrentOption);
            if (equipmentTypeCurrentOption == 0)
            {
                InventoryUI.instance.CustomEquipmentList(Equipment.Types.WEAPON, usableToSearchFor);
            }
            else if(equipmentTypeCurrentOption == 1)
                {
                InventoryUI.instance.CustomEquipmentList(Equipment.Types.ARMOR, usableToSearchFor);
            }
            else if(equipmentTypeCurrentOption == 2)
            {
                InventoryUI.instance.CustomEquipmentList(Equipment.Types.ACCESSORY, usableToSearchFor);
            }
        }
        else if((GetKeyDown(KeyCode.S) || (GetButtonDown("down"))) && GameManager.instance.isLoading == false && equipmentTypeCurrentOption < equipmentTypeList.Count -1)
        {
            equipmentTypeCurrentOption++;
            SoundManager.instance.PlaySFX(0);
            GameMenus.instance.DisplaySelectedItemInInfoPanel(true, equipmentTypeCurrentOption);
            if (equipmentTypeCurrentOption == 0)
            {
                InventoryUI.instance.CustomEquipmentList(Equipment.Types.WEAPON, usableToSearchFor);
            }
            else if (equipmentTypeCurrentOption == 1)
            {
                InventoryUI.instance.CustomEquipmentList(Equipment.Types.ARMOR, usableToSearchFor);
            }
            else if (equipmentTypeCurrentOption == 2)
            {
                InventoryUI.instance.CustomEquipmentList(Equipment.Types.ACCESSORY, usableToSearchFor);
            }
        }
        else if ((GetKeyDown(KeyCode.C) || (GetButtonDown("Fire2"))) && GameManager.instance.isLoading == false)
        {
            inventoryState = InventoryStates.EQUIPMENT_PANEL;
            initializingFields = true;
            SoundManager.instance.PlaySFX(1);
           
        }


        if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && GameManager.instance.isLoading == false && equipmentTypeCurrentOption == 0)
        {
            typeToSearchFor = Equipment.Types.WEAPON;
            inventoryState = InventoryStates.EQUIPMENT_WEAPON_SELECT;
            initializingFields = true;
            SoundManager.instance.PlaySFX(0);
            weaponFingerCounter = 1;
        }
        else if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && GameManager.instance.isLoading == false && equipmentTypeCurrentOption == 1)
        {
            typeToSearchFor = Equipment.Types.ARMOR;
            inventoryState = InventoryStates.EQUIPMENT_WEAPON_SELECT;
            initializingFields = true;
            SoundManager.instance.PlaySFX(0);
            weaponFingerCounter = 1;
        }
        else if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && GameManager.instance.isLoading == false && equipmentTypeCurrentOption == 2)
        {
            typeToSearchFor = Equipment.Types.ACCESSORY;
            inventoryState = InventoryStates.EQUIPMENT_WEAPON_SELECT;
            initializingFields = true;
            SoundManager.instance.PlaySFX(0);
            weaponFingerCounter = 1;
        }


        horizontalFinger.transform.position = equipmentTypeList[equipmentTypeCurrentOption].transform.position;
    }

    void WeaponSelect()
    {

        if(initializingFields == true)
        {
            initializingFields = false;
            equipmentPanel.SetActive(true);
            GameMenus.instance.DisplaySelectedItemInInfoPanel(false,weaponCurrentOption);
            GameMenus.instance.DisplayStatVariance(weaponCurrentOption,equipmentTypeCurrentOption);
            weaponCurrentOption = 0;
            weaponFingerCounter = 1;
        }
    


        if ((GetKeyDown(KeyCode.C) || (GetButtonDown("Fire2"))) && GameManager.instance.isLoading == false)
        {
            horizontalFinger.SetActive(false);
            weaponCurrentOption = 0;
            weaponFingerCounter = 1;
            weaponScrollRect.verticalNormalizedPosition += (float)1 / (InventoryUI.instance.weaponHolderList.Count - 3);
            SoundManager.instance.PlaySFX(1);
            GameMenus.instance.ResetStatModifierWindow();
            inventoryState = InventoryStates.EQUIPMENT_TYPE_SELECT;
            initializingFields = true;


        }

        if (InventoryUI.instance.weaponHolderList.Count >= 1)
        {
            if ((GetKeyDown(KeyCode.W) || (GetButtonDown("up"))) && weaponCurrentOption > 0 && GameManager.instance.isLoading == false)
            {
                weaponCurrentOption--;
                GameMenus.instance.DisplayStatVariance(weaponCurrentOption,equipmentTypeCurrentOption);

                if (weaponFingerCounter > 1)
                {
                    weaponFingerCounter--;
                }
                else if (weaponFingerCounter == 1)
                {
                    weaponScrollRect.verticalNormalizedPosition += (float)1 / (InventoryUI.instance.weaponHolderList.Count - 3);
                }

                SoundManager.instance.PlaySFX(0);
               GameMenus.instance.DisplaySelectedItemInInfoPanel(false,weaponCurrentOption);

            }

            else if ((GetKeyDown(KeyCode.S) || (GetButtonDown("down"))) && weaponCurrentOption < InventoryUI.instance.weaponHolderList.Count - 1 && GameManager.instance.isLoading == false)
            {
                weaponCurrentOption++;
                GameMenus.instance.DisplayStatVariance(weaponCurrentOption,equipmentTypeCurrentOption);


                if (weaponFingerCounter < 3)
                {
                    weaponFingerCounter++;
                }
                else if (weaponFingerCounter == 3)
                {
                    weaponScrollRect.verticalNormalizedPosition -= (float)1 / (InventoryUI.instance.weaponHolderList.Count - 3);
                }


                SoundManager.instance.PlaySFX(0);
                GameMenus.instance.DisplaySelectedItemInInfoPanel(false,weaponCurrentOption);
            }

            if (((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && GameManager.instance.isLoading == false))
            {
                inventoryState = InventoryStates.EQUIPMENT_TYPE_SELECT;
                initializingFields = true;
                GameMenus.instance.EquipWeapon(weaponCurrentOption,equipmentTypeCurrentOption);
                //InventoryUI.instance.CustomEquipmentList(Equipment.Types.WEAPON, Equipment.UsableBy.KEVIN);
                weaponScrollRect.verticalNormalizedPosition += (float)1 / (InventoryUI.instance.weaponHolderList.Count - 3);

            }


        }

       /* if (Input.GetKeyDown(KeyCode.Y) && selectedItem == -1)
        {

        }
        else if (Input.GetKeyDown(KeyCode.Y) && (selectedItem != -1) && (selectedItem != inventoryCurrentOption))
        {

        }
        */

        horizontalFinger.transform.position = InventoryUI.instance.weaponHolderList[weaponCurrentOption].GetComponent<ItemHolder>().fingerPoint.transform.position;
    }

    private void MatPlayerSelect()
    {
        if (initializingFields == true)
        {
            GameMenus.instance.ClearMateriaScreen();
            initializingFields = false;
            horizontalFinger.SetActive(false);// moved this to bottom if can move cursor
            verticalFinger.SetActive(false);
            blinkingFinger.SetActive(false);
            blinkingVerticalFinger.SetActive(true);

            inventoryPanel.SetActive(false);
            equipmentPanel.SetActive(true);
            optionsPanel.SetActive(false);

            player1Panel.SetActive(true);
            player2Panel.SetActive(true);
            player3Panel.SetActive(true);
            materiaPanel.SetActive(true);

            equipControlPanel.SetActive(false);
            GameMenus.instance.UpdateCharInfoPanel(playerCurrentOption);
            blinkingVerticalFinger.transform.position = optionsList[currentOption].transform.position;
            GameMenus.instance.UpdateMateriaEquipmentPanels(playerCurrentOption);
            GameMenus.instance.DisplaySelectedItemInInfoPanel(true, playerCurrentOption);
            
            materiaFingerCounter = 1;
            materiaSelectCurrentOption = 0;
            materiaScrollRect.verticalNormalizedPosition = 1f;


        }


        if ((GetKeyDown(KeyCode.W) || (GetButtonDown("up"))) && playerCurrentOption > 0 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            playerCurrentOption--;
            GameMenus.instance.UpdateCharInfoPanel(playerCurrentOption);
            SoundManager.instance.PlaySFX(0);
            GameMenus.instance.UpdateMateriaEquipmentPanels(playerCurrentOption);
            GameMenus.instance.DisplaySelectedItemInInfoPanel(true, playerCurrentOption);
        }

        else if ((GetKeyDown(KeyCode.S) || (GetButtonDown("down"))) && playerCurrentOption < playerOptionsList.Count - 1 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            playerCurrentOption++;
            GameMenus.instance.UpdateCharInfoPanel(playerCurrentOption);
            SoundManager.instance.PlaySFX(0);
            GameMenus.instance.UpdateMateriaEquipmentPanels(playerCurrentOption);
            GameMenus.instance.DisplaySelectedItemInInfoPanel(true, playerCurrentOption);
        }
        else if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) /*&& playerCurrentOption == 0*/ && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            StartCoroutine(GameMenus.instance.MateriaCharSelected(playerCurrentOption,true));
            //usableToSearchFor = GameManager.instance.playerGameObjects[playerCurrentOption].GetComponent<CharStatsHolder>().currentUseBy;
            GameMenus.instance.characterSelected = playerCurrentOption;
            inventoryState = InventoryStates.MATERIA_PANEL;
            StartCoroutine(GameMenus.instance.TurnOnMateriaPanelAnimation(true));

            initializingFields = true;
        }

        if ((GetKeyDown(KeyCode.C) || (GetButtonDown("Fire2"))) && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            inventoryState = InventoryStates.OPTIONS_PANEL;
            initializingFields = true;
            SoundManager.instance.PlaySFX(1);

        }

        if (GameManager.instance.canMoveCursor) { horizontalFinger.SetActive(true); }
        if (GameManager.instance.canMoveCursor) { optionsPanel.SetActive(true); }

        horizontalFinger.transform.position = playerOptionsList[playerCurrentOption].transform.position;
    }



    private void MatPanelSelect()
    {
        if (initializingFields == true)
        {
           // GameMenus.instance.HackToGetMateriaCorrect();
            initializingFields = false;
            horizontalFinger.SetActive(false);// moved this to bottom if can move cursor
            verticalFinger.SetActive(false);
            blinkingFinger.SetActive(false);
            blinkingVerticalFinger.SetActive(false);
            optionsPanel.SetActive(false);
            weaponMateriaSelect = 0;

            materiaWeapon = false;
            materiaArmor = false;
            materiaAcc = false;


        }

        if ((GetKeyDown(KeyCode.W) || (GetButtonDown("up"))) && materiaEquipCurrentOption > 0 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            materiaEquipCurrentOption--;
            SoundManager.instance.PlaySFX(0);
        }

        else if ((GetKeyDown(KeyCode.S) || (GetButtonDown("down"))) && materiaEquipCurrentOption < materiaEquipmentList.Count - 1 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            materiaEquipCurrentOption++;
            SoundManager.instance.PlaySFX(0);
        }
        else if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            if (materiaEquipCurrentOption == 0 && GameMenus.instance.charStatHolders[playerCurrentOption].equippedWeapon.intNoOfSlots > 0)
            {


                materiaWeapon = true;
                materiaArmor = false;
                materiaAcc = false;
                SoundManager.instance.PlaySFX(0);
                inventoryState = InventoryStates.MATERIA_EDIT_SELECTION;
                initializingFields = true;
            }
            else if (materiaEquipCurrentOption == 1 && GameMenus.instance.charStatHolders[playerCurrentOption].equippedArmor.intNoOfSlots > 0)
            {
                materiaWeapon = false;
                materiaArmor = true;
                materiaAcc = false;
                SoundManager.instance.PlaySFX(0);
                inventoryState = InventoryStates.MATERIA_EDIT_SELECTION;
                initializingFields = true;
            }

            else if (materiaEquipCurrentOption == 2 && GameMenus.instance.charStatHolders[playerCurrentOption].equippedAccessory.intNoOfSlots > 0)
            {
                materiaWeapon = false;
                materiaArmor = false;
                materiaAcc = true;
                SoundManager.instance.PlaySFX(0);
                inventoryState = InventoryStates.MATERIA_EDIT_SELECTION;
                initializingFields = true;
            }
            else
            {
                SoundManager.instance.PlaySFX(3);
            }


        }

        if ((GetKeyDown(KeyCode.C) || (GetButtonDown("Fire2"))) && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            StartCoroutine(GameMenus.instance.MateriaCharSelected(playerCurrentOption, false));
           StartCoroutine(GameMenus.instance.TurnOnMateriaPanelAnimation(false));
            SoundManager.instance.PlaySFX(1);

        }


        if (GameManager.instance.canMoveCursor) { horizontalFinger.SetActive(true); }
        horizontalFinger.transform.position = materiaEquipmentList[materiaEquipCurrentOption].transform.position;


    }

    private void MatEditSelect()
    {
        if (initializingFields == true)
        {
            initializingFields = false;
            horizontalFinger.SetActive(true);
            verticalFinger.SetActive(false);
            blinkingFinger.SetActive(false);
            blinkingVerticalFinger.SetActive(true);
            optionsPanel.SetActive(false);

            //weaponMateriaSelect = 0;
            //armorMateriaSelect = 0;
            //accMateriaSelect = 0;
            if (materiaWeapon)
            {
                GameMenus.instance.DisplayMateriaStats(GameMenus.instance.charStatHolders[playerCurrentOption].weaponEquippedMateria[weaponMateriaSelect]);
            }
            else if (materiaArmor)
            {
                GameMenus.instance.DisplayMateriaStats(GameMenus.instance.charStatHolders[playerCurrentOption].armorEquippedMateria[weaponMateriaSelect]);
            }
            else if (materiaAcc)
            {
                GameMenus.instance.DisplayMateriaStats(GameMenus.instance.charStatHolders[playerCurrentOption].accEquippedMateria[weaponMateriaSelect]);
            }

        }


        if ((GetKeyDown(KeyCode.A) || (GetButtonDown("left"))) && weaponMateriaSelect > 0 && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            weaponMateriaSelect--;
            SoundManager.instance.PlaySFX(0);
            if (materiaWeapon)
            {
                GameMenus.instance.DisplayMateriaStats(GameMenus.instance.charStatHolders[playerCurrentOption].weaponEquippedMateria[weaponMateriaSelect]);
            }
            else if (materiaArmor)
            {
                GameMenus.instance.DisplayMateriaStats(GameMenus.instance.charStatHolders[playerCurrentOption].armorEquippedMateria[weaponMateriaSelect]);
            }
            else if (materiaAcc)
            {
                GameMenus.instance.DisplayMateriaStats(GameMenus.instance.charStatHolders[playerCurrentOption].accEquippedMateria[weaponMateriaSelect]);
            }

        }

        else if ((GetKeyDown(KeyCode.D) || (GetButtonDown("right")))  && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            if(materiaWeapon && weaponMateriaSelect < GameMenus.instance.charStatHolders[playerCurrentOption].equippedWeapon.intNoOfSlots - 1)
            {
                weaponMateriaSelect++;
                GameMenus.instance.DisplayMateriaStats(GameMenus.instance.charStatHolders[playerCurrentOption].weaponEquippedMateria[weaponMateriaSelect]);
                SoundManager.instance.PlaySFX(0);
            }


            else if (materiaArmor && weaponMateriaSelect < GameMenus.instance.charStatHolders[playerCurrentOption].equippedArmor.intNoOfSlots - 1)

            {
                weaponMateriaSelect++;
                GameMenus.instance.DisplayMateriaStats(GameMenus.instance.charStatHolders[playerCurrentOption].armorEquippedMateria[weaponMateriaSelect]);
                SoundManager.instance.PlaySFX(0);
            }

            else if (materiaAcc && weaponMateriaSelect < GameMenus.instance.charStatHolders[playerCurrentOption].equippedAccessory.intNoOfSlots - 1)
            {
                weaponMateriaSelect++;
                GameMenus.instance.DisplayMateriaStats(GameMenus.instance.charStatHolders[playerCurrentOption].accEquippedMateria[weaponMateriaSelect]);
                SoundManager.instance.PlaySFX(0);
            }


        }

        if ((GetKeyDown(KeyCode.C) || (GetButtonDown("Fire2"))) && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            inventoryState = InventoryStates.MATERIA_PANEL;
            initializingFields = true;
            SoundManager.instance.PlaySFX(1);

        }

        if ((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            inventoryState = InventoryStates.MATERIA_SELECTION;
            initializingFields = true;
            SoundManager.instance.PlaySFX(0);

        }
        else if ( (GetKeyDown(KeyCode.X) || GetButtonDown("Fire3")) && GameManager.instance.isLoading == false && GameManager.instance.canMoveCursor)
        {
            SoundManager.instance.PlaySFX(10);
            UnequipMateria(true,0,0);
        }



        if (materiaWeapon)
        {
            blinkingVerticalFinger.transform.position = weaponMateriaList[weaponMateriaSelect].transform.position;
        }
        else if (materiaArmor)
        {
            blinkingVerticalFinger.transform.position = armorMateriaList[weaponMateriaSelect].transform.position;
        }
        else if (materiaAcc)
        {
            blinkingVerticalFinger.transform.position = accMateriaList[weaponMateriaSelect].transform.position;
        }


    }

    private void MatSelection()
    {
        if (initializingFields == true)
        {
            initializingFields = false;
            blinkingFinger.SetActive(true);
            horizontalFinger.SetActive(true);
            GameMenus.instance.DisplayMateriaStats(Inventory.instance.materiaList[materiaSelectCurrentOption]);
            /*
            materiaFingerCounter = 1;
            materiaSelectCurrentOption = 0;
            */

        }



        if ((GetKeyDown(KeyCode.C) || (GetButtonDown("Fire2"))) && GameManager.instance.isLoading == false)
        {
            SoundManager.instance.PlaySFX(1);
            inventoryState = InventoryStates.MATERIA_EDIT_SELECTION;
            initializingFields = true;



        }
        if ((GetKeyDown(KeyCode.D) || (GetButtonDown("right"))) && GameManager.instance.isLoading == false)
        {
            SoundManager.instance.PlaySFX(1);
            //InventoryUI.instance.FillSmallMateriaList(1);



        }

        if (InventoryUI.instance.materiaHolderList.Count >= 1)
        {
            if ((GetKeyDown(KeyCode.W) || (GetButtonDown("up"))) && materiaSelectCurrentOption > 0 && GameManager.instance.isLoading == false)
            {
                materiaSelectCurrentOption--;
               // 

                if (materiaFingerCounter > 1)
                {
                    materiaFingerCounter--;
                }
                else if (materiaFingerCounter == 1)
                {
                    materiaScrollRect.verticalNormalizedPosition += (float)1 / (InventoryUI.instance.materiaHolderList.Count - 5);
                }

                SoundManager.instance.PlaySFX(0);
                GameMenus.instance.DisplayMateriaStats(Inventory.instance.materiaList[materiaSelectCurrentOption]);

            }

            else if ((GetKeyDown(KeyCode.S) || (GetButtonDown("down"))) && materiaSelectCurrentOption < InventoryUI.instance.materiaHolderList.Count - 1 && GameManager.instance.isLoading == false)
            {
                materiaSelectCurrentOption++;
                

                if (materiaFingerCounter < 5)
                {
                    materiaFingerCounter++;
                }
                else if (materiaFingerCounter == 5)
                {
                    materiaScrollRect.verticalNormalizedPosition -= (float)1 / (InventoryUI.instance.materiaHolderList.Count - 5);
                }

                GameMenus.instance.DisplayMateriaStats(Inventory.instance.materiaList[materiaSelectCurrentOption]);
                SoundManager.instance.PlaySFX(0);
            }

            else if (((GetKeyDown(KeyCode.Space) || (GetButtonDown("Fire1"))) && GameManager.instance.isLoading == false))
            {
                SoundManager.instance.PlaySFX(10);
                inventoryState = InventoryStates.MATERIA_EDIT_SELECTION;
                initializingFields = true;
                GameMenus.instance.EquipMateria(materiaEquipCurrentOption, weaponMateriaSelect,materiaSelectCurrentOption);
                GameMenus.instance.UpdateMateriaEquipmentPanels(playerCurrentOption);

            }



        }

        blinkingFinger.transform.position = InventoryUI.instance.materiaHolderList[materiaSelectCurrentOption].GetComponent<ItemHolder>().fingerPoint.transform.position;
    }



    public void UnequipMateria(bool comingFromMateriaScreen, int slotToRemove, int equipmentSelection)
    {// used when pressing X and when equipping weapon with less slots than you have equipped

        if (comingFromMateriaScreen)
        {
            if (materiaWeapon && GameMenus.instance.charStatHolders[playerCurrentOption].weaponEquippedMateria[weaponMateriaSelect].name != "Empty")
            {
                Inventory.instance.tempMateriaHolder = GameMenus.instance.charStatHolders[playerCurrentOption].weaponEquippedMateria[weaponMateriaSelect];
                GameMenus.instance.charStatHolders[playerCurrentOption].weaponEquippedMateria[weaponMateriaSelect] = MasterMateriaList.instance.totalMateriaList[0];
                GameManager.instance.playerClasses[playerCurrentOption].weaponEquippedMateria[weaponMateriaSelect] = MasterMateriaList.instance.totalMateriaList[0];
                GameMenus.instance.DisplayMateriaStats(GameMenus.instance.charStatHolders[playerCurrentOption].weaponEquippedMateria[weaponMateriaSelect]);
                
            }
            else if (materiaArmor && GameMenus.instance.charStatHolders[playerCurrentOption].armorEquippedMateria[weaponMateriaSelect].name != "Empty")
            {
                Inventory.instance.tempMateriaHolder = GameMenus.instance.charStatHolders[playerCurrentOption].armorEquippedMateria[weaponMateriaSelect];
                GameMenus.instance.charStatHolders[playerCurrentOption].armorEquippedMateria[weaponMateriaSelect] = MasterMateriaList.instance.totalMateriaList[0];
                GameManager.instance.playerClasses[playerCurrentOption].armorEquippedMateria[weaponMateriaSelect] = MasterMateriaList.instance.totalMateriaList[0];
                GameMenus.instance.DisplayMateriaStats(GameMenus.instance.charStatHolders[playerCurrentOption].armorEquippedMateria[weaponMateriaSelect]);
            }
            else if (materiaAcc && GameMenus.instance.charStatHolders[playerCurrentOption].accEquippedMateria[weaponMateriaSelect].name != "Empty")
            {
                Inventory.instance.tempMateriaHolder = GameMenus.instance.charStatHolders[playerCurrentOption].accEquippedMateria[weaponMateriaSelect];
                GameMenus.instance.charStatHolders[playerCurrentOption].accEquippedMateria[weaponMateriaSelect] = MasterMateriaList.instance.totalMateriaList[0];
                GameManager.instance.playerClasses[playerCurrentOption].accEquippedMateria[weaponMateriaSelect] = MasterMateriaList.instance.totalMateriaList[0];
                GameMenus.instance.DisplayMateriaStats(GameMenus.instance.charStatHolders[playerCurrentOption].accEquippedMateria[weaponMateriaSelect]);
            }
            else
            {
                
                return;
            }
            
            
            Inventory.instance.AddMateria(Inventory.instance.tempMateriaHolder);
            InventoryUI.instance.RemoveMateriaFromEquipment(Inventory.instance.tempMateriaHolder);

            //InventoryUI.instance.FillSmallMateriaList(0);
            //InventoryUI.instance.FillMateriaList();
            
            GameMenus.instance.UpdateMateriaEquipmentPanels(playerCurrentOption);

    


        }
        else if (!comingFromMateriaScreen)
        {
            
            if (equipmentSelection == 0 && GameMenus.instance.charStatHolders[playerCurrentOption].weaponEquippedMateria[weaponMateriaSelect].name != "Empty")
            {
                Inventory.instance.tempMateriaHolder = GameMenus.instance.charStatHolders[playerCurrentOption].weaponEquippedMateria[slotToRemove];
                GameMenus.instance.charStatHolders[playerCurrentOption].weaponEquippedMateria[slotToRemove] = MasterMateriaList.instance.totalMateriaList[0];
                GameManager.instance.playerClasses[playerCurrentOption].weaponEquippedMateria[slotToRemove] = MasterMateriaList.instance.totalMateriaList[0];

            }
            else if (equipmentSelection == 1 && GameMenus.instance.charStatHolders[playerCurrentOption].armorEquippedMateria[weaponMateriaSelect].name != "Empty")
            {
                Inventory.instance.tempMateriaHolder = GameMenus.instance.charStatHolders[playerCurrentOption].armorEquippedMateria[slotToRemove];
                GameMenus.instance.charStatHolders[playerCurrentOption].armorEquippedMateria[slotToRemove] = MasterMateriaList.instance.totalMateriaList[0];
                GameManager.instance.playerClasses[playerCurrentOption].armorEquippedMateria[slotToRemove] = MasterMateriaList.instance.totalMateriaList[0];
            }
            else if (equipmentSelection == 2 && GameMenus.instance.charStatHolders[playerCurrentOption].accEquippedMateria[weaponMateriaSelect].name != "Empty")
            {
                Inventory.instance.tempMateriaHolder = GameMenus.instance.charStatHolders[playerCurrentOption].accEquippedMateria[slotToRemove];
                GameMenus.instance.charStatHolders[playerCurrentOption].accEquippedMateria[slotToRemove] = MasterMateriaList.instance.totalMateriaList[0];
                GameManager.instance.playerClasses[playerCurrentOption].accEquippedMateria[slotToRemove] = MasterMateriaList.instance.totalMateriaList[0];
            }
            Inventory.instance.AddMateria(Inventory.instance.tempMateriaHolder);
            
            
        }

    }
}
