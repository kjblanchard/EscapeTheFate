using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GameMenus : MonoBehaviour
{

    public GameObject mapScreenBox;
    [SerializeField] Text mapScreenText;

    public bool shouldDisplayTransition;

    public static GameMenus instance;

    public Sprite invisiBro;


    [Header("Char Panel References")]

    public GameObject[] charPanelHolders;
    public Animator[] charPanelAnimators;
    public Animator charSelectedStatsAnimator;
    [SerializeField] float charStatsDelay;
    [SerializeField] MMCharSelectedHolder charSelectedMMguy;

    [Header("Character Info Panel")]

    public Image charInfoImage;
    public Animator charAnimator;
    public Text charWeaponText;
    public Text charArmorText;
    public Text charAccsryText;

    public GameObject equipDescriptionMaster;
    public GameObject equipEffectMaster;
    public GameObject equipSlotMaster;

    [Header("Character Info Panel")]

    public int characterSelected;
    public Text powerModifier;
    public Text armorModifier;
    public Text magAtkModifier;
    public Text magDefModifier;
    public Text critModifier;
    public Text dodgeModifier;
    public Text hitModifier;
    public Text speedModifier;
    public Text strengthModifier;
    public Text defModifier;
    public Text magModifier;
    public Text spiritModifier;

    [Header("Item Stat References")]

    private int itemPower;
    private int itemArmor;
    private int itemMagAtk;
    private int itemMagDef;
    private int itemCrit;
    private int itemDodge;
    private int itemHit;
    private int itemSpeed;

    [Header("Slot Controllers")]

    public SlotController equipScreenSlotController;
    [SerializeField] SlotController materiaWeaponSlotController;
    [SerializeField] SlotController materiaArmorSlotController;
    [SerializeField] SlotController materiaAccSlotController;

    [Header("MateriaEquipPanels")]
    [SerializeField] MateriaEquipmentPanel materiaWeaponPanel;
    [SerializeField] MateriaEquipmentPanel materiaArmorPanel;
    [SerializeField] MateriaEquipmentPanel materiaAccPanel;

    [Header("UI Screen Animators")]
    [SerializeField] Animator materiaSelectedInfoAnimator;
    [SerializeField] Animator materiaSelectionAnimator;
    [SerializeField] Animator characterInfoAnimator;
    [SerializeField] Animator currencyAnimator;
    [SerializeField] private Animator itemInfoAnimator;
    [SerializeField] private Animator itemItemsAnimator;
    [SerializeField] private Animator itemOptionsAnimator;
    [SerializeField] private Animator itemPanelAnimator;


    [Header("Reference To CharStatsHolders")]
    public CharStatsHolder[] charStatHolders;

    [Header("Materia Info Panel")]
    public Text materiaNameText;
    public Image[] materiaLevelSprites;
    public Text materiaCurrentAPText;
    public Text materiaApNextLevelText;
    public Text[] materiaSkillText;
    public Text[] materiaSkillDescriptionText;
    public Text[] materiaBonusStatText;

    [Header("Materia Sprites")]
    [SerializeField] Sprite materiaSupportEmptySprite;
    [SerializeField] Sprite materiaSupportFullSprite;
    [SerializeField] Sprite materiaMagicEmptySprite;
    [SerializeField] Sprite materiaMagicFullSprite;
    [SerializeField] Sprite materiaStandEmptySprite;
    [SerializeField] Sprite materiaStandFullSprite;
    [SerializeField] Sprite materiaSkillEmptySprite;
    [SerializeField] Sprite materiaSkillFullSprite;
    [SerializeField] Sprite materiaPersonalEmptySprite;
    [SerializeField] Sprite materiaPersonalFullSprite;








    void Awake()
    {

        {
            instance = this;
            int gameMenusCount = FindObjectsOfType<GameMenus>().Length;
            if (gameMenusCount > 1)
            {
                Destroy(gameObject);
            }
            else
            {
                DontDestroyOnLoad(gameObject);
            }
        }

    }

    private void Start()
    {
        DisplayTransitionBanner(); //in here for testing purposes so it works in the room
        charSelectedMMguy = FindObjectOfType<MMCharSelectedHolder>();
    }

    void Update()
    {

    }









    public IEnumerator ShowMapTransition()
    {
        GameManager.instance.mapTransitionTextDisplaying = true;
        mapScreenText.text = FindObjectOfType<MainCamera>().GetMapTransitionText();
        mapScreenBox.SetActive(true);
        yield return new WaitForSeconds(3);
        mapScreenBox.SetActive(false);
        //Debug.Log("ended the co");
        GameManager.instance.mapTransitionTextDisplaying = false;
    }

    public void DisplayTransitionBanner()
    {
        if (shouldDisplayTransition == true)
        {

            StartCoroutine(ShowMapTransition());
            //Debug.Log("Your display transition in gamemenu is true");
        }
        else if (shouldDisplayTransition == false)
        {
            // Debug.Log("Your display transition in gamemenu is false");
        }
    }

    public void UpdateMainMenuCharacterPanels()
    {
        for (int i = 0; i < charPanelHolders.Length; i++)
        {
            CharStatsHolder tempCharClass = GameManager.instance.playerGameObjects[i].GetComponent<CharStatsHolder>();
            if (tempCharClass.characterSpotTaken == true)
            {
                charPanelHolders[i].SetActive(true);

                MMCharPanelHolder tempHolder = charPanelHolders[i].GetComponent<MMCharPanelHolder>();
                //tempHolder.charImage.sprite = tempCharClass.characterAnimation;
                tempHolder.nameText.text = tempCharClass.characterName;
                tempHolder.currentHPText.text = tempCharClass.currentHP.ToString() + "  / ";
                tempHolder.maxHPText.text = tempCharClass.currentMaxHP.ToString();
                tempHolder.currentMPText.text = tempCharClass.currentMP.ToString() + "  / ";
                tempHolder.maxMPText.text = tempCharClass.currentMaxMP.ToString();
                tempHolder.currentXPText.text = tempCharClass.currentEXP.ToString() + "  / ";
                tempHolder.nextLevelXPText.text = tempCharClass.EXPtoNextLevel.ToString();
                tempHolder.limitSlider.value = tempCharClass.currentLimit;
                tempHolder.levelText.text = tempCharClass.currentLevel.ToString();
                //tempHolder.panelsAnimator.runtimeAnimatorController = tempCharClass.characterWalkAnimation;


            }
            else if (tempCharClass.characterSpotTaken == false)
            {
                //turn off the gameobject
                charPanelHolders[i].SetActive(false);
            }
        }
    }

    public void UpdateCharInfoPanel(int positionToUpdate)
    {
        CharStatsHolder tempCharClass = GameManager.instance.playerGameObjects[positionToUpdate].GetComponent<CharStatsHolder>();
        charInfoImage.sprite = tempCharClass.characterAnimation;
        charAnimator.runtimeAnimatorController = tempCharClass.characterWalkAnimation;
        if (tempCharClass.equippedWeapon == null)
        {
            charWeaponText.text = "Empty";
        }
        else
        {
            charWeaponText.text = tempCharClass.equippedWeapon.name;
        }
        if (tempCharClass.equippedArmor == null)
        {
            charArmorText.text = "Empty";
        }
        else
        {
            charArmorText.text = tempCharClass.equippedArmor.name;
        }
        if (tempCharClass.equippedAccessory == null)
        {
            charAccsryText.text = "Empty";
        }
        else
        {
            charAccsryText.text = tempCharClass.equippedAccessory.name;
        }


        UpdateCharStats(positionToUpdate);

        //Debug.Log("Just updated the char infos");
    }

    public void CharSelected(int selectedChar)
    {
        // Debug.Log("just did the char selected void");
        for (int i = 0; i < charPanelHolders.Length; i++)
        {
            if (selectedChar != i)
            {
                charPanelHolders[i].SetActive(false);
            }
            else if (selectedChar == i)
            {
                charPanelHolders[i].SetActive(true);
                charPanelAnimators[i].SetBool("isSelected", true);
            }
            else
            {
            }
        }
    }

    public IEnumerator MateriaCharSelected(int selectedChar, bool entering)
    {//used to animate the char panels and start moving the materia panels
        GameManager.instance.canMoveCursor = false;
        if (entering)
        {
            for (int i = 0; i < charPanelHolders.Length; i++)
            {
                charPanelAnimators[i].SetBool("materiaSelected", true);
            }
            yield return new WaitForSeconds(0.15f);

            for (int i = 0; i < charPanelHolders.Length; i++)
            {
                if (selectedChar != i)
                {
                    InventoryManager.instance.charPanelCanvasGroup[i].alpha = 0;
                }
                else if (selectedChar == i)
                {
                    charPanelHolders[i].SetActive(true);
                    StartCoroutine(MateriaScreenController.instance.OpenMateriaScreen(true));
                }
                else
                {
                }
            }


            yield return null;
        }
        if (!entering)
        {
            InventoryManager.instance.horizontalFinger.SetActive(false);
            StartCoroutine(MateriaScreenController.instance.OpenMateriaScreen(false));
            yield return new WaitForSeconds(0.5f);
            for (int i = 0; i < charPanelHolders.Length; i++)
            {
                InventoryManager.instance.charPanelCanvasGroup[i].alpha = 1;
            }
            //yield return new WaitForSeconds(1);
            for (int i = charPanelHolders.Length; i-- > 0;)
            //for (int i = 0; i < charPanelHolders.Length; i++)
            {

                charPanelAnimators[i].SetBool("materiaSelected", false);
                yield return new WaitForSeconds(0.15f);
            }
            //yield return new WaitForSeconds(1);


            InventoryManager.instance.inventoryState = InventoryManager.InventoryStates.MATERIA_PLAYER_SELECT;
            InventoryManager.instance.initializingFields = true;
            // yield return new WaitForSeconds(2);
            GameManager.instance.canMoveCursor = true;
        }

    }


    public void CharDeSelected()
    {
        for (int i = 0; i < charPanelHolders.Length; i++)
        {

            charPanelHolders[i].SetActive(true);
            charPanelAnimators[i].SetBool("isSelected", false);

            //Debug.Log("just hit the char deselected void");
        }
    }

    public IEnumerator ShowCharStats(bool show)
    {//used when a char is selected on the menu to start the animation and control cursor
        if (show == true)
        {
            GameManager.instance.canMoveCursor = false;
            StartCoroutine(DelayToShowStats(true));
            yield return new WaitForSeconds(charStatsDelay);
            GameManager.instance.canMoveCursor = true;
        }
        else if (show == false)
        {
            GameManager.instance.canMoveCursor = false;
            GameMenus.instance.charSelectedStatsAnimator.SetBool("isSelected", false);
            yield return new WaitForSeconds(charStatsDelay);
            GameManager.instance.canMoveCursor = true;
        }
        yield return null;

    }

    public void UpdateCharStats(int charSelected)
    {
        GameObject temp = InventoryManager.instance.charSelectedPanel;
        CharStatsHolder tempCharClass = GameManager.instance.playerGameObjects[charSelected].GetComponent<CharStatsHolder>();
        charSelectedMMguy.strengthText.text = tempCharClass.currentStr.ToString();
        charSelectedMMguy.defText.text = tempCharClass.currentDef.ToString();
        charSelectedMMguy.magText.text = tempCharClass.currentMag.ToString();
        charSelectedMMguy.sprText.text = tempCharClass.currentSpr.ToString();
        charSelectedMMguy.spdText.text = tempCharClass.currentSpeed.ToString();
        charSelectedMMguy.hitText.text = tempCharClass.currentHit.ToString();
        charSelectedMMguy.pwrText.text = tempCharClass.currentPower.ToString();
        charSelectedMMguy.armText.text = tempCharClass.currentArmor.ToString();
        charSelectedMMguy.magAtkText.text = tempCharClass.currentMagAtk.ToString();
        charSelectedMMguy.magDefText.text = tempCharClass.currentMagDef.ToString();
        charSelectedMMguy.critText.text = tempCharClass.currentCrit.ToString();
        charSelectedMMguy.dodgeText.text = tempCharClass.currentDodge.ToString();
    }

    public void TurnOffStats()
    {
        StopAllCoroutines();
        InventoryManager.instance.charSelectCanvasGroup.alpha = 0f;
    }

    private IEnumerator DelayToShowStats(bool go)
    {//shows the stats after a delay
        if (go == true)
        {
            yield return new WaitForSeconds(0.2f);
            GameMenus.instance.charSelectedStatsAnimator.SetBool("isSelected", true);
        }

        /*else if(go == false)
        {//probably can remove this, not in use
            yield return new WaitForSeconds(charStatsDelay);
            GameMenus.instance.charSelectedStatsAnimator.SetBool("isSelected", false);
        }
        */
    }

    public void DisplayStatVariance(int weaponNumber, int slotToCheck)
    {//this is used to determine what to display when mousing over a weapon in the stat screen
        InventoryManager.instance.statModifierCanvasGroup.alpha = 1f;
        CharStatsHolder tempCharStats = GameManager.instance.playerGameObjects[characterSelected].GetComponent<CharStatsHolder>();
        itemPower = Inventory.instance.weaponList[weaponNumber].equipment.power;
        itemArmor = Inventory.instance.weaponList[weaponNumber].equipment.armor;
        itemArmor = Inventory.instance.weaponList[weaponNumber].equipment.magicAttack;
        itemMagDef = Inventory.instance.weaponList[weaponNumber].equipment.magicDefense;
        itemCrit = Inventory.instance.weaponList[weaponNumber].equipment.crit;
        itemDodge = Inventory.instance.weaponList[weaponNumber].equipment.dodge;
        itemHit = Inventory.instance.weaponList[weaponNumber].equipment.hit;
        itemSpeed = Inventory.instance.weaponList[weaponNumber].equipment.speed;

        if (slotToCheck == 0)
        {
            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.power > tempCharStats.equippedWeapon.power)
            {
                powerModifier.color = new Color(0, 1, 0);

                powerModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.power - tempCharStats.equippedWeapon.power).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.power == tempCharStats.equippedWeapon.power)
            {
                powerModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.power < tempCharStats.equippedWeapon.power)
            {
                powerModifier.color = new Color(1, 0, 0);
                powerModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.power - tempCharStats.equippedWeapon.power).ToString();
            }
            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.armor > tempCharStats.equippedWeapon.armor)
            {
                armorModifier.color = new Color(0, 1, 0);

                armorModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.armor - tempCharStats.equippedWeapon.armor).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.armor == tempCharStats.equippedWeapon.armor)
            {
                armorModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.armor < tempCharStats.equippedWeapon.armor)
            {
                armorModifier.color = new Color(1, 0, 0);
                armorModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.armor - tempCharStats.equippedWeapon.armor).ToString();
            }
            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicAttack > tempCharStats.equippedWeapon.magicAttack)
            {
                magAtkModifier.color = new Color(0, 1, 0);

                magAtkModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicAttack - tempCharStats.equippedWeapon.magicAttack).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicAttack == tempCharStats.equippedWeapon.magicAttack)
            {
                magAtkModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicAttack < tempCharStats.equippedWeapon.magicAttack)
            {
                magAtkModifier.color = new Color(1, 0, 0);
                magAtkModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicAttack - tempCharStats.equippedWeapon.magicAttack).ToString();
            }

            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicDefense > tempCharStats.equippedWeapon.magicDefense)
            {
                magDefModifier.color = new Color(0, 1, 0);

                magDefModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicDefense - tempCharStats.equippedWeapon.magicDefense).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicDefense == tempCharStats.equippedWeapon.magicDefense)
            {
                magDefModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.power < tempCharStats.equippedWeapon.power)
            {
                magDefModifier.color = new Color(1, 0, 0);
                magDefModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicDefense - tempCharStats.equippedWeapon.magicDefense).ToString();
            }
            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.crit > tempCharStats.equippedWeapon.crit)
            {
                critModifier.color = new Color(0, 1, 0);

                critModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.crit - tempCharStats.equippedWeapon.crit).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.crit == tempCharStats.equippedWeapon.crit)
            {
                critModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.crit < tempCharStats.equippedWeapon.crit)
            {
                critModifier.color = new Color(1, 0, 0);
                critModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.crit - tempCharStats.equippedWeapon.crit).ToString();
            }

            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.dodge > tempCharStats.equippedWeapon.dodge)
            {
                dodgeModifier.color = new Color(0, 1, 0);

                dodgeModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.dodge - tempCharStats.equippedWeapon.dodge).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.dodge == tempCharStats.equippedWeapon.dodge)
            {
                dodgeModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.dodge < tempCharStats.equippedWeapon.dodge)
            {
                dodgeModifier.color = new Color(1, 0, 0);
                dodgeModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.dodge - tempCharStats.equippedWeapon.dodge).ToString();
            }

            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.hit > tempCharStats.equippedWeapon.hit)
            {
                hitModifier.color = new Color(0, 1, 0);

                hitModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.hit - tempCharStats.equippedWeapon.hit).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.hit == tempCharStats.equippedWeapon.hit)
            {
                hitModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.hit < tempCharStats.equippedWeapon.hit)
            {
                hitModifier.color = new Color(1, 0, 0);
                hitModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.hit - tempCharStats.equippedWeapon.hit).ToString();
            }

            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.speed > tempCharStats.equippedWeapon.speed)
            {
                speedModifier.color = new Color(0, 1, 0);

                speedModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.speed - tempCharStats.equippedWeapon.speed).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.speed == tempCharStats.equippedWeapon.speed)
            {
                speedModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.speed < tempCharStats.equippedWeapon.speed)
            {
                speedModifier.color = new Color(1, 0, 0);
                speedModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.speed - tempCharStats.equippedWeapon.speed).ToString();
            }
        }

        else if (slotToCheck == 1)
        {
            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.power > tempCharStats.equippedArmor.power)
            {
                powerModifier.color = new Color(0, 1, 0);

                powerModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.power - tempCharStats.equippedArmor.power).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.power == tempCharStats.equippedArmor.power)
            {
                powerModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.power < tempCharStats.equippedArmor.power)
            {
                powerModifier.color = new Color(1, 0, 0);
                powerModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.power - tempCharStats.equippedArmor.power).ToString();
            }
            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.armor > tempCharStats.equippedArmor.armor)
            {
                armorModifier.color = new Color(0, 1, 0);

                armorModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.armor - tempCharStats.equippedArmor.armor).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.armor == tempCharStats.equippedArmor.armor)
            {
                armorModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.armor < tempCharStats.equippedArmor.armor)
            {
                armorModifier.color = new Color(1, 0, 0);
                armorModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.armor - tempCharStats.equippedArmor.armor).ToString();
            }
            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicAttack > tempCharStats.equippedArmor.magicAttack)
            {
                magAtkModifier.color = new Color(0, 1, 0);

                magAtkModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicAttack - tempCharStats.equippedArmor.magicAttack).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicAttack == tempCharStats.equippedArmor.magicAttack)
            {
                magAtkModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicAttack < tempCharStats.equippedArmor.magicAttack)
            {
                magAtkModifier.color = new Color(1, 0, 0);
                magAtkModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicAttack - tempCharStats.equippedArmor.magicAttack).ToString();
            }

            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicDefense > tempCharStats.equippedArmor.magicDefense)
            {
                magDefModifier.color = new Color(0, 1, 0);

                magDefModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicDefense - tempCharStats.equippedArmor.magicDefense).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicDefense == tempCharStats.equippedArmor.magicDefense)
            {
                magDefModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.power < tempCharStats.equippedArmor.power)
            {
                magDefModifier.color = new Color(1, 0, 0);
                magDefModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicDefense - tempCharStats.equippedArmor.magicDefense).ToString();
            }
            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.crit > tempCharStats.equippedArmor.crit)
            {
                critModifier.color = new Color(0, 1, 0);

                critModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.crit - tempCharStats.equippedArmor.crit).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.crit == tempCharStats.equippedArmor.crit)
            {
                critModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.crit < tempCharStats.equippedArmor.crit)
            {
                critModifier.color = new Color(1, 0, 0);
                critModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.crit - tempCharStats.equippedArmor.crit).ToString();
            }

            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.dodge > tempCharStats.equippedArmor.dodge)
            {
                dodgeModifier.color = new Color(0, 1, 0);

                dodgeModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.dodge - tempCharStats.equippedArmor.dodge).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.dodge == tempCharStats.equippedArmor.dodge)
            {
                dodgeModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.dodge < tempCharStats.equippedArmor.dodge)
            {
                dodgeModifier.color = new Color(1, 0, 0);
                dodgeModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.dodge - tempCharStats.equippedArmor.dodge).ToString();
            }

            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.hit > tempCharStats.equippedArmor.hit)
            {
                hitModifier.color = new Color(0, 1, 0);

                hitModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.hit - tempCharStats.equippedArmor.hit).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.hit == tempCharStats.equippedArmor.hit)
            {
                hitModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.hit < tempCharStats.equippedArmor.hit)
            {
                hitModifier.color = new Color(1, 0, 0);
                hitModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.hit - tempCharStats.equippedArmor.hit).ToString();
            }

            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.speed > tempCharStats.equippedArmor.speed)
            {
                speedModifier.color = new Color(0, 1, 0);

                speedModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.speed - tempCharStats.equippedArmor.speed).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.speed == tempCharStats.equippedArmor.speed)
            {
                speedModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.speed < tempCharStats.equippedArmor.speed)
            {
                speedModifier.color = new Color(1, 0, 0);
                speedModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.speed - tempCharStats.equippedArmor.speed).ToString();
            }

        }

        else if (slotToCheck == 2)
        {
            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.power > tempCharStats.equippedAccessory.power)
            {
                powerModifier.color = new Color(0, 1, 0);

                powerModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.power - tempCharStats.equippedAccessory.power).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.power == tempCharStats.equippedAccessory.power)
            {
                powerModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.power < tempCharStats.equippedAccessory.power)
            {
                powerModifier.color = new Color(1, 0, 0);
                powerModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.power - tempCharStats.equippedAccessory.power).ToString();
            }
            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.armor > tempCharStats.equippedAccessory.armor)
            {
                armorModifier.color = new Color(0, 1, 0);

                armorModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.armor - tempCharStats.equippedAccessory.armor).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.armor == tempCharStats.equippedAccessory.armor)
            {
                armorModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.armor < tempCharStats.equippedAccessory.armor)
            {
                armorModifier.color = new Color(1, 0, 0);
                armorModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.armor - tempCharStats.equippedAccessory.armor).ToString();
            }
            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicAttack > tempCharStats.equippedAccessory.magicAttack)
            {
                magAtkModifier.color = new Color(0, 1, 0);

                magAtkModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicAttack - tempCharStats.equippedAccessory.magicAttack).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicAttack == tempCharStats.equippedAccessory.magicAttack)
            {
                magAtkModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicAttack < tempCharStats.equippedAccessory.magicAttack)
            {
                magAtkModifier.color = new Color(1, 0, 0);
                magAtkModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicAttack - tempCharStats.equippedAccessory.magicAttack).ToString();
            }

            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicDefense > tempCharStats.equippedAccessory.magicDefense)
            {
                magDefModifier.color = new Color(0, 1, 0);

                magDefModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicDefense - tempCharStats.equippedAccessory.magicDefense).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicDefense == tempCharStats.equippedAccessory.magicDefense)
            {
                magDefModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.power < tempCharStats.equippedAccessory.power)
            {
                magDefModifier.color = new Color(1, 0, 0);
                magDefModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.magicDefense - tempCharStats.equippedAccessory.magicDefense).ToString();
            }
            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.crit > tempCharStats.equippedAccessory.crit)
            {
                critModifier.color = new Color(0, 1, 0);

                critModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.crit - tempCharStats.equippedAccessory.crit).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.crit == tempCharStats.equippedAccessory.crit)
            {
                critModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.crit < tempCharStats.equippedAccessory.crit)
            {
                critModifier.color = new Color(1, 0, 0);
                critModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.crit - tempCharStats.equippedAccessory.crit).ToString();
            }

            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.dodge > tempCharStats.equippedAccessory.dodge)
            {
                dodgeModifier.color = new Color(0, 1, 0);

                dodgeModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.dodge - tempCharStats.equippedAccessory.dodge).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.dodge == tempCharStats.equippedAccessory.dodge)
            {
                dodgeModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.dodge < tempCharStats.equippedAccessory.dodge)
            {
                dodgeModifier.color = new Color(1, 0, 0);
                dodgeModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.dodge - tempCharStats.equippedAccessory.dodge).ToString();
            }

            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.hit > tempCharStats.equippedAccessory.hit)
            {
                hitModifier.color = new Color(0, 1, 0);

                hitModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.hit - tempCharStats.equippedAccessory.hit).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.hit == tempCharStats.equippedAccessory.hit)
            {
                hitModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.hit < tempCharStats.equippedAccessory.hit)
            {
                hitModifier.color = new Color(1, 0, 0);
                hitModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.hit - tempCharStats.equippedAccessory.hit).ToString();
            }

            if (Inventory.instance.tempWeaponList[weaponNumber].equipment.speed > tempCharStats.equippedAccessory.speed)
            {
                speedModifier.color = new Color(0, 1, 0);
                speedModifier.text = "+ " + (Inventory.instance.tempWeaponList[weaponNumber].equipment.speed - tempCharStats.equippedAccessory.speed).ToString();

            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.speed == tempCharStats.equippedAccessory.speed)
            {
                speedModifier.text = "";
            }
            else if (Inventory.instance.tempWeaponList[weaponNumber].equipment.speed < tempCharStats.equippedAccessory.speed)
            {
                speedModifier.color = new Color(1, 0, 0);
                speedModifier.text = (Inventory.instance.tempWeaponList[weaponNumber].equipment.speed - tempCharStats.equippedAccessory.speed).ToString();
            }
        }


    }


    public void EquipWeapon(int weaponNumber, int slotToEquip)
    {//used when equipping a weapon
        CharStatsHolder tempCharStats = GameManager.instance.playerGameObjects[characterSelected].GetComponent<CharStatsHolder>();
        InventoryManager.instance.horizontalFinger.SetActive(false);
        //Inventory.instance.tempWeaponHolder.currentAmount = 1;

        if (slotToEquip == 0)
        {
            if (tempCharStats.equippedWeapon == Inventory.instance.tempWeaponList[weaponNumber].equipment)
            {
                SoundManager.instance.PlaySFX(9);
                return;
            }
            else
            {


                Inventory.instance.tempWeaponHolder = tempCharStats.equippedWeapon;
                tempCharStats.equippedWeapon = Inventory.instance.tempWeaponList[weaponNumber].equipment;
                GameManager.instance.playerClasses[characterSelected].equippedWeapon = Inventory.instance.tempWeaponList[weaponNumber].equipment;
            }


        }
        else if (slotToEquip == 1)
        {
            if (tempCharStats.equippedArmor == Inventory.instance.tempWeaponList[weaponNumber].equipment)
            {
                SoundManager.instance.PlaySFX(9);
                Debug.Log("this is the samething");
                return;

            }
            else
            {


                Inventory.instance.tempWeaponHolder = tempCharStats.equippedArmor;
                tempCharStats.equippedArmor = Inventory.instance.tempWeaponList[weaponNumber].equipment;
                GameManager.instance.playerClasses[characterSelected].equippedArmor = Inventory.instance.tempWeaponList[weaponNumber].equipment;
            }

        }
        else if (slotToEquip == 2)
        {
            if (tempCharStats.equippedAccessory == Inventory.instance.tempWeaponList[weaponNumber].equipment)
            {
                SoundManager.instance.PlaySFX(9);
                return;
            }
            else
            {


                Inventory.instance.tempWeaponHolder = tempCharStats.equippedAccessory;
                tempCharStats.equippedAccessory = Inventory.instance.tempWeaponList[weaponNumber].equipment;
                GameManager.instance.playerClasses[characterSelected].equippedAccessory = Inventory.instance.tempWeaponList[weaponNumber].equipment;
            }




        }


        Inventory.instance.RemoveWeapon(Inventory.instance.tempWeaponList[weaponNumber].equipment);
        //if(Inventory.instance.weaponList[weaponNumber].currentAmount <= 0) { Inventory.instance.weaponList.RemoveAt(weaponNumber); }
        //Inventory.instance.weaponList.Add(Inventory.instance.tempWeaponHolder);
        Inventory.instance.AddWeapon(Inventory.instance.tempWeaponHolder);
        UpdateCharStats(characterSelected);
        GameManager.instance.UpdateStatsHolders();
        //InventoryUI.instance.WeaponRecreateList();
        SoundManager.instance.PlaySFX(9);
        GameMenus.instance.UpdateCharInfoPanel(characterSelected);
        ResetStatModifierWindow();
    }

    public void ResetStatModifierWindow()
    {
        InventoryManager.instance.statModifierCanvasGroup.alpha = 0f;
        powerModifier.text = "";
        armorModifier.text = "";
        magAtkModifier.text = "";
        magDefModifier.text = "";
        critModifier.text = "";
        dodgeModifier.text = "";
        hitModifier.text = "";
    }

    public void ClearCharInfoPanel()
    {
        InventoryUI.instance.weaponDescriptionTextField.text = "";
        InventoryUI.instance.weaponEffectTextField.text = "";
        equipScreenSlotController.HideAllSlots();
        //InventoryUI.instance.weaponSlotsTextField.text = "";
    }

    public void DisplaySelectedItemInInfoPanel(bool showEquipped, int slotToShow)
    {//used to change the description, effect, and slots in the menu when you mouseover things
        if (showEquipped)
        {
            CharStatsHolder tempCharStats = GameManager.instance.playerGameObjects[characterSelected].GetComponent<CharStatsHolder>();
            if (slotToShow == 0)
            {
                InventoryUI.instance.weaponDescriptionTextField.text = tempCharStats.equippedWeapon.description;
                InventoryUI.instance.weaponEffectTextField.text = tempCharStats.equippedWeapon.effect;
                DisplaySlots(tempCharStats.equippedWeapon, 0);



            }
            else if (slotToShow == 1)
            {
                InventoryUI.instance.weaponDescriptionTextField.text = tempCharStats.equippedArmor.description;
                InventoryUI.instance.weaponEffectTextField.text = tempCharStats.equippedArmor.effect;
                DisplaySlots(tempCharStats.equippedArmor, 0);

            }
            else if (slotToShow == 2)
            {
                InventoryUI.instance.weaponDescriptionTextField.text = tempCharStats.equippedAccessory.description;
                InventoryUI.instance.weaponEffectTextField.text = tempCharStats.equippedAccessory.effect;
                DisplaySlots(tempCharStats.equippedAccessory, 0);

            }


        }

        else if (!showEquipped)
        {
            InventoryUI.instance.weaponDescriptionTextField.text = Inventory.instance.tempWeaponList[InventoryManager.instance.weaponCurrentOption].equipment.description;
            InventoryUI.instance.weaponEffectTextField.text = Inventory.instance.tempWeaponList[InventoryManager.instance.weaponCurrentOption].equipment.effect;
            DisplaySlots(Inventory.instance.tempWeaponList[InventoryManager.instance.weaponCurrentOption].equipment, 0);
        }

    }

    public void DisplaySlots(Equipment equipmentToCheck, int slotControllerToSendTo)
    {
        if (equipmentToCheck.numberOfSlots == Equipment.Slots.NONE)
        {
            int[] singleSlotsToSend = { };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };

            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.ONE)
        {
            int[] singleSlotsToSend = { 0 };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };

            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.TWO)
        {
            int[] singleSlotsToSend = { 0, 1 };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.TWO_LINKED)
        {
            int[] singleSlotsToSend = { };
            int[] leftLinkSlotToSend = { 0 };
            int[] rightLinkSlotToSend = { 1 };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }

        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.THREE)
        {
            int[] singleSlotsToSend = { 0, 1, 2 };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.THREE_AND_TWOLINKED)
        {
            int[] singleSlotsToSend = { 2 };
            int[] leftLinkSlotToSend = { 0 };
            int[] rightLinkSlotToSend = { 1 };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.FOUR)
        {
            int[] singleSlotsToSend = { 0, 1, 2, 3 };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.FOUR_AND_TWOLINKED)
        {
            int[] singleSlotsToSend = { 2, 3 };
            int[] leftLinkSlotToSend = { 0 };
            int[] rightLinkSlotToSend = { 1 };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }

        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.FOUR_AND_TWO_TWOLINKED)
        {
            int[] singleSlotsToSend = { };
            int[] leftLinkSlotToSend = { 0, 2 };
            int[] rightLinkSlotToSend = { 1, 3 };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }

        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.FIVE)
        {
            int[] singleSlotsToSend = { 0, 1, 2, 3, 4 };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.FIVE_AND_TWOLINKED)
        {
            int[] singleSlotsToSend = { 2, 3, 4 };
            int[] leftLinkSlotToSend = { 0 };
            int[] rightLinkSlotToSend = { 1 };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.FIVE_AND_TWO_TWOLINKED)
        {
            int[] singleSlotsToSend = { 4 };
            int[] leftLinkSlotToSend = { 0, 2 };
            int[] rightLinkSlotToSend = { 1, 3 };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.SIX)
        {
            int[] singleSlotsToSend = { 0, 1, 2, 3, 4, 5 };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }

        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.SIX_AND_TWOLINKED)
        {
            int[] singleSlotsToSend = { 2, 3, 4, 5 };
            int[] leftLinkSlotToSend = { 0 };
            int[] rightLinkSlotToSend = { 1 };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.SIX_AND_TWO_TWOLINKED)
        {
            int[] singleSlotsToSend = { 4, 5 };
            int[] leftLinkSlotToSend = { 0, 2 };
            int[] rightLinkSlotToSend = { 1, 3 };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.SIX_AND_THREE_TWOLINKED)
        {
            int[] singleSlotsToSend = { };
            int[] leftLinkSlotToSend = { 0, 2, 4 };
            int[] rightLinkSlotToSend = { 1, 3, 5 };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }
        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.SEVEN)
        {
            int[] singleSlotsToSend = { 0, 1, 2, 3, 4, 5, 6 };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }
        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.SEVEN_AND_TWOLINKED)
        {
            int[] singleSlotsToSend = { 2, 3, 4, 5, 6 };
            int[] leftLinkSlotToSend = { 0 };
            int[] rightLinkSlotToSend = { 1 };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }
        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.SEVEN_AND_TWO_TWOLINKED)
        {
            int[] singleSlotsToSend = { 4, 5, 6 };
            int[] leftLinkSlotToSend = { 0, 2 };
            int[] rightLinkSlotToSend = { 1, 3 };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }
        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.SEVEN_AND_THREE_TWOLINKED)
        {
            int[] singleSlotsToSend = { 6 };
            int[] leftLinkSlotToSend = { 0, 2, 4 };
            int[] rightLinkSlotToSend = { 1, 3, 5 };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.EIGHT)
        {
            int[] singleSlotsToSend = { 0, 1, 2, 3, 4, 5, 6, 7 };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }
        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.EIGHT_AND_TWOLINKED)
        {
            int[] singleSlotsToSend = { 2, 3, 4, 5, 6, 7 };
            int[] leftLinkSlotToSend = { 0 };
            int[] rightLinkSlotToSend = { 1 };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.EIGHT_AND_TWO_TWOLINKED)
        {
            int[] singleSlotsToSend = { 4, 5, 6, 7 };
            int[] leftLinkSlotToSend = { 0, 2 };
            int[] rightLinkSlotToSend = { 1, 3 };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }
        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.EIGHT_AND_THREE_TWOLINKED)
        {
            int[] singleSlotsToSend = { 6, 7 };
            int[] leftLinkSlotToSend = { 0, 2, 4 };
            int[] rightLinkSlotToSend = { 1, 3, 5 };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.EIGHT_AND_FOUR_TWOLINKED)
        {
            int[] singleSlotsToSend = { };
            int[] leftLinkSlotToSend = { 0, 2, 4, 6 };
            int[] rightLinkSlotToSend = { 1, 3, 5, 7 };
            if (slotControllerToSendTo == 0) { equipScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); }
            else if (slotControllerToSendTo == 1) { materiaWeaponSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 1); }
            else if (slotControllerToSendTo == 2) { materiaArmorSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 2); }
            else if (slotControllerToSendTo == 3) { materiaAccSlotController.DisplaySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend, 3); }
        }

    }

    public void UpdateMateriaEquipmentPanels(int playerToCheck)
    {//this is used to update the equipment that will be displayed on the materia equipment panels

        CharStatsHolder playersStats = GameManager.instance.playerGameObjects[playerToCheck].GetComponent<CharStatsHolder>();
        materiaWeaponPanel.equipmentImage.sprite = playersStats.equippedWeapon.icon;
        materiaArmorPanel.equipmentImage.sprite = playersStats.equippedArmor.icon;
        materiaAccPanel.equipmentImage.sprite = playersStats.equippedAccessory.icon;

        materiaWeaponPanel.equipmentName.text = playersStats.equippedWeapon.name;
        materiaArmorPanel.equipmentName.text = playersStats.equippedArmor.name;
        materiaAccPanel.equipmentName.text = playersStats.equippedAccessory.name;

        DisplaySlots(playersStats.equippedWeapon, 1);
        DisplaySlots(playersStats.equippedArmor, 2);
        DisplaySlots(playersStats.equippedAccessory, 3);
    }

    public IEnumerator TurnOnMateriaPanelAnimation(bool isOpen)
    {
        if (isOpen)
        {
            characterInfoAnimator.SetBool("screenOpen", false);
            currencyAnimator.SetBool("screenOpen", false);
            materiaSelectionAnimator.SetBool("screenOpen", true);
            yield return new WaitForSeconds(0.25f);
            materiaSelectedInfoAnimator.SetBool("screenOpen", true);

        }

        if (!isOpen)
        {
            characterInfoAnimator.SetBool("screenOpen", true);
            currencyAnimator.SetBool("screenOpen", true);
            materiaSelectedInfoAnimator.SetBool("screenOpen", false);
            yield return new WaitForSeconds(0.25f);

            materiaSelectionAnimator.SetBool("screenOpen", false);

        }
    }

    public IEnumerator TurnOnItemPanelAnimation(bool isOpen)
        {
            if (isOpen)
            {
                itemPanelAnimator.SetBool("screenOpen",true);
                itemOptionsAnimator.SetBool("screenOpen",true);
                //yield return new WaitForSeconds(0.15f);
                itemInfoAnimator.SetBool("screenOpen",true);
                //yield return new WaitForSeconds((0.15f));
                itemItemsAnimator.SetBool("screenOpen",true);
                yield return new WaitForSeconds(0.2f);
                GameManager.instance.isLoading = false;

            }
            if (!isOpen)
            {
                //yield return new WaitForSeconds(0.15f);
                itemOptionsAnimator.SetBool("screenOpen",false);
                //yield return new WaitForSeconds(0.15f);
                itemInfoAnimator.SetBool("screenOpen",false);
                //yield return new WaitForSeconds((0.15f));
                itemItemsAnimator.SetBool("screenOpen",false);
                yield return new WaitForSeconds(0.1f);
                itemPanelAnimator.SetBool("screenOpen",false);
                yield return new WaitForSeconds(0.1f);
                InventoryManager.instance.inventoryState = InventoryManager.InventoryStates.OPTIONS_PANEL;
                InventoryManager.instance.initializingFields = true;
                GameManager.instance.isLoading = false;
            }
            
        }

    public void GrabCharacterStatsReference()
    {//added to the npcdefault menu opener script for now, this should be added when you FIRST open the menu only, every time


        //also should replace a lot of the temp gets I used before implementing this, as this saves gets since you can reference these

        for (int i = 0; i < charStatHolders.Length; i++)
        {
            charStatHolders[i] = null;
        }

        for (int i = 0; i < GameManager.instance.playerGameObjects.Length; i++)
        {
            charStatHolders[i] = GameManager.instance.playerGameObjects[i].GetComponent<CharStatsHolder>();
        }
        /*
        charStatHolders (GameManager.instance.playerGameObjects[0].GetComponent<CharStatsHolder>());
        charStatHolders.Add (GameManager.instance.playerGameObjects[1].GetComponent<CharStatsHolder>());
        charStatHolders.Add (GameManager.instance.playerGameObjects[2].GetComponent<CharStatsHolder>());
        */
    }

    public void DisplayMateriaStats(Materia materiaToDisplay)
    {//this is used to update the materia information screen with the stats from the materia, done when mousing over materias that are equipped or in the inv

        if (materiaToDisplay.name == "Empty")
        {
            ClearMateriaScreen();
            return;
        }
        else
        {
            materiaNameText.text = materiaToDisplay.name;
            MateriaElementsActivator(materiaToDisplay);
            materiaCurrentAPText.text = materiaToDisplay.currentAP.ToString();
            materiaApNextLevelText.text = materiaToDisplay.apToLevel[materiaToDisplay.currentLevel].ToString();
            for (int i = 0; i < materiaSkillText.Length; i++)
            {
                if (i > materiaToDisplay.maxLevel)
                {
                    materiaSkillText[i].text = "";
                    break;
                }
                else if (i <= materiaToDisplay.currentLevel)
                {
                    materiaSkillText[i].color = new Color(1, 1, 1);

                }
                else
                {
                    materiaSkillText[i].color = new Color(0.3f, 0.3f, 0.3f);
                }
                materiaSkillText[i].text = materiaToDisplay.skillNames[i];
            }
            for (int i = 0; i < materiaSkillDescriptionText.Length; i++)
            {
                if (i > materiaToDisplay.maxLevel)
                {
                    materiaSkillDescriptionText[i].text = "";
                    break;
                }
                else if (i <= materiaToDisplay.currentLevel)
                {
                    materiaSkillDescriptionText[i].color = new Color(1, 1, 1);

                }
                else
                {
                    materiaSkillDescriptionText[i].color = new Color(0.3f, 0.3f, 0.3f);
                }

                materiaSkillDescriptionText[i].text = materiaToDisplay.skillDescriptions[i];
            }

            if (materiaToDisplay.bonusStats.Length >= 1)
            {
                for (int i = 0; i < materiaBonusStatText.Length; i++)
                {
                    materiaBonusStatText[i].text = "";

                }
                for (int i = 0; i < materiaToDisplay.bonusStats.Length; i++)
                {
                    materiaBonusStatText[i].text = materiaToDisplay.bonusStats[i];
                }
            }
            else
            {
                for (int i = 0; i < materiaBonusStatText.Length; i++)
                {
                    materiaBonusStatText[i].text = "";
                }
            }
        }

    }

    private void MateriaElementsActivator(Materia materiaToScan)
    {//used to clear out the sprites and the text for levels higher than it should display

        for (int i = 0; i < materiaLevelSprites.Length; i++)
        {
            if (i <= materiaToScan.maxLevel)
            {
                if (i <= materiaToScan.currentLevel)
                {
                    if (materiaToScan.materiaType == Materia.MateriaType.Magic)
                    {
                        materiaLevelSprites[i].sprite = materiaMagicFullSprite;
                    }
                    else if (materiaToScan.materiaType == Materia.MateriaType.Skill)
                    {
                        materiaLevelSprites[i].sprite = materiaSkillFullSprite;
                    }
                    else if (materiaToScan.materiaType == Materia.MateriaType.Support)
                    {
                        materiaLevelSprites[i].sprite = materiaSupportFullSprite;
                    }
                    else if (materiaToScan.materiaType == Materia.MateriaType.Independant)
                    {
                        materiaLevelSprites[i].sprite = materiaStandFullSprite;
                    }
                    else if (materiaToScan.materiaType == Materia.MateriaType.Personal)
                    {
                        materiaLevelSprites[i].sprite = materiaPersonalFullSprite;
                    }

                }
                else if (i > materiaToScan.currentLevel)
                {
                    if (materiaToScan.materiaType == Materia.MateriaType.Magic)
                    {
                        materiaLevelSprites[i].sprite = materiaMagicEmptySprite;
                    }
                    else if (materiaToScan.materiaType == Materia.MateriaType.Skill)
                    {
                        materiaLevelSprites[i].sprite = materiaSkillEmptySprite;
                    }
                    else if (materiaToScan.materiaType == Materia.MateriaType.Support)
                    {
                        materiaLevelSprites[i].sprite = materiaSupportEmptySprite;
                    }
                    else if (materiaToScan.materiaType == Materia.MateriaType.Independant)
                    {
                        materiaLevelSprites[i].sprite = materiaStandEmptySprite;
                    }
                    else if (materiaToScan.materiaType == Materia.MateriaType.Personal)
                    {
                        materiaLevelSprites[i].sprite = materiaPersonalEmptySprite;
                    }
                }
            }
            else if (i > materiaToScan.maxLevel)
            {
                materiaLevelSprites[i].sprite = invisiBro;
            }
        }

        for (int i = 0; i < materiaSkillDescriptionText.Length; i++)
        {
            if (i <= materiaToScan.maxLevel)
            {
                materiaSkillDescriptionText[i].text = materiaToScan.skillDescriptions[i];
            }
            else if (i > materiaToScan.maxLevel)
            {
                materiaSkillDescriptionText[i].text = "";
            }
        }

        for (int i = 0; i < materiaSkillText.Length; i++)
        {
            if (i <= materiaToScan.maxLevel)
            {
                materiaSkillText[i].text = materiaToScan.skillNames[i];
            }
            else if (i > materiaToScan.maxLevel)
            {
                materiaSkillText[i].text = "";
            }
        }
    }

    public void ClearMateriaScreen()
    {// used to clear the screen when leaving and before entering
        materiaNameText.text = "";
        materiaCurrentAPText.text = "";
        materiaApNextLevelText.text = "";
        for (int i = 0; i < materiaSkillText.Length; i++)
        {
            materiaSkillText[i].text = "";
        }
        for (int i = 0; i < materiaSkillDescriptionText.Length; i++)
        {

            materiaSkillDescriptionText[i].text = "";
        }
        for (int i = 0; i < materiaBonusStatText.Length; i++)
        {
            materiaBonusStatText[i].text = "";

        }
        for (int i = 0; i < materiaLevelSprites.Length; i++)
        {
            materiaLevelSprites[i].sprite = invisiBro;
        }


    }


    public void EquipMateria(int equipmentType, int equipmentMateriaSlot, int materiaListLocation)
    {//used when equipping a weapon
        InventoryManager.instance.blinkingFinger.SetActive(false);
        CharStatsHolder tempCharStats = GameMenus.instance.charStatHolders[InventoryManager.instance.playerCurrentOption];


        if (equipmentType == 0)
        {
            Inventory.instance.tempMateriaHolder = tempCharStats.weaponEquippedMateria[equipmentMateriaSlot];
            tempCharStats.weaponEquippedMateria[equipmentMateriaSlot] = Inventory.instance.materiaList[materiaListLocation];
            GameManager.instance.playerClasses[characterSelected].weaponEquippedMateria[equipmentMateriaSlot] = Inventory.instance.materiaList[materiaListLocation];

        }
        else if (equipmentType == 1)
        {
            Inventory.instance.tempMateriaHolder = tempCharStats.armorEquippedMateria[equipmentMateriaSlot];
            tempCharStats.armorEquippedMateria[equipmentMateriaSlot] = Inventory.instance.materiaList[materiaListLocation];
            GameManager.instance.playerClasses[characterSelected].armorEquippedMateria[equipmentMateriaSlot] = Inventory.instance.materiaList[materiaListLocation];

        }
        else if (equipmentType == 2)
        {
            Inventory.instance.tempMateriaHolder = tempCharStats.accEquippedMateria[equipmentMateriaSlot];
            tempCharStats.accEquippedMateria[equipmentMateriaSlot] = Inventory.instance.materiaList[materiaListLocation];
            GameManager.instance.playerClasses[characterSelected].accEquippedMateria[equipmentMateriaSlot] = Inventory.instance.materiaList[materiaListLocation];

        }

        //this is for when you are swapping materia, not when you are equipping to the inventory
        if (Inventory.instance.tempMateriaHolder.name != "Empty")
        {
            ItemHolder materiaListItemHolder =
                InventoryUI.instance.materiaHolderList[materiaListLocation].GetComponent<ItemHolder>();
            materiaListItemHolder.amountText.text =
                "Level " + (Inventory.instance.tempMateriaHolder.currentLevel + 1).ToString();
            materiaListItemHolder.nameText.text = Inventory.instance.tempMateriaHolder.name;
            materiaListItemHolder.typeImage.sprite = Inventory.instance.tempMateriaHolder.icon;
            materiaListItemHolder.typeText.text = Inventory.instance.tempMateriaHolder.materiaType.ToString();
            Inventory.instance.materiaList[materiaListLocation] = Inventory.instance.tempMateriaHolder;
        }
        else if (Inventory.instance.tempMateriaHolder.name == "Empty")
        {
            Destroy(InventoryUI.instance.materiaHolderList[materiaListLocation].gameObject); 
            //Debug.Log("This is an empty materia");
            Inventory.instance.materiaList.RemoveAt(materiaListLocation);
            InventoryUI.instance.materiaHolderList.RemoveAt(materiaListLocation);
            //InventoryUI.instance.materiaHolderList.
        }
        else{Debug.Log("Borked");}

        if (InventoryManager.instance.materiaSelectCurrentOption > InventoryUI.instance.materiaHolderList.Count -1)
        {
            InventoryManager.instance.materiaSelectCurrentOption--;
        }
    }


}