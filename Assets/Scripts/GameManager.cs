using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{

    public static GameManager instance;


    [Header("Character State Controls")]
    public bool isLoading = false;
    public bool canActivate = false; //can start dialog should try and remove this

    public bool inMenu;

    public bool dialogActive;
    public bool dialogCloseDelay = false; 
    public bool ttActive;

    public bool fadingBetweenAreas;

    public bool mapTransitionTextDisplaying;

    public bool afterCardBattle;
    public int afterCardBattleStatus;

    public bool canMoveCursor = true;

    public bool shouldCloseMenu = true;
    public bool shouldCloseShop;
    public bool makingDecision = false;
    public bool inShop = false;



    [Header("Canvas Panels")]
    public GameObject optionsCanvas;
    public GameObject ttCanvas;
    public GameObject dialogCanvas;

    [Header("Current Classes GameObjects")]
    public GameObject[] playerGameObjects;


    [Header("Current Classes In Party")]
    public CharacterClass[] playerClasses;

    [Header("Character Bench")]
    public GameObject characterBenchGameObject;
    public CharacterBench characterBench;

    [Header("Joystick Handler")]
    public bool joystickEnabled;
    public GameObject joystickMaster;
    public GameObject[] regularJoystick;
    public GameObject directionalJoystick;
    public bool frameRateOf60;
    public bool frameRateOf120;

    [Header("FadeControl")]
    public bool shouldFade;

    [Header("Played Timer")]
    public float totalPlayedTime;


    private void Awake()
    {
        if (frameRateOf60)
        {
            SetVsync1();
        }
        else if (frameRateOf120)
        {
            SetVsync0_120FPS();
        }
        else
        {
            SetVsync1();
        }
        instance = this;

        int gameManagerCount = FindObjectsOfType<GameManager>().Length;
        if(gameManagerCount > 1)
        {
            Destroy(gameObject);
        }
        else
        {
            DontDestroyOnLoad(gameObject);
        }
    }

    private void Start()
    {
        //optionsCanvas.SetActive(false);

        InitializeCharacterClasses();
        UpdateStatsHolders(); 
        Debug.Log("I commented this section below out to get rid of an error");
        //GameMenus.instance.UpdateCharInfoPanel(0);//updates menu to show first players equipment when opening for looks


    }

    private void Update()
    {
        totalPlayedTime += Time.deltaTime;


    }

    private void InitializeCharacterClasses()
    {//copies the player class from game manager to the player stats
        for (int i = 0; i < playerGameObjects.Length; i++)
        {
            CharStatsHolder tempCharStats = playerGameObjects[i].GetComponent<CharStatsHolder>();
            if (tempCharStats.characterSpotTaken == true)
            {
                
                playerClasses[i] = tempCharStats.playersClass;
                //add to the references
                //turn on proper components?
            }
            else if (tempCharStats.characterSpotTaken == false)
            {
                //do not add to references
                //turn off proper components / disable elements / put in blank informations and class
            }
        }
    }

    public void UpdateStatsHolders()
    {
        for (int i = 0; i < playerGameObjects.Length; i++)
        {
            CharStatsHolder tempCharStats = playerGameObjects[i].GetComponent<CharStatsHolder>();
            tempCharStats.currentLevel = playerClasses[i].currentLevel;
            tempCharStats.currentHP = playerClasses[i].currentHP;
            tempCharStats.currentMaxHP = playerClasses[i].maxHP[tempCharStats.currentLevel];
            tempCharStats.currentMP = playerClasses[i].currentMP;
            tempCharStats.currentMaxMP = playerClasses[i].maxMP[tempCharStats.currentLevel];
            tempCharStats.currentStr = playerClasses[i].str[tempCharStats.currentLevel];
            tempCharStats.currentDef = playerClasses[i].def[tempCharStats.currentLevel];
            tempCharStats.currentMag = playerClasses[i].mag[tempCharStats.currentLevel];
            tempCharStats.currentSpr = playerClasses[i].spr[tempCharStats.currentLevel];
            tempCharStats.currentSpeed = playerClasses[i].speed[tempCharStats.currentLevel];
            tempCharStats.currentLuck = playerClasses[i].luck[tempCharStats.currentLevel];
            tempCharStats.currentLimit = playerClasses[i].limitGuage;
            tempCharStats.currentEXP = playerClasses[i].currentEXP;
            tempCharStats.EXPtoNextLevel = playerClasses[i].expToNextLevel[tempCharStats.currentLevel];
            tempCharStats.characterName = playerClasses[i].characterName;
            tempCharStats.characterClass = playerClasses[i].characterClass;
            tempCharStats.characterBio = playerClasses[i].characterBio;
            tempCharStats.currentHit = playerClasses[i].hit;
            tempCharStats.currentUseBy = playerClasses[i].whoCanUse;


            tempCharStats.characterPortrait = playerClasses[i].characterPortrait;
            tempCharStats.characterAnimation = playerClasses[i].characterAnimation;
            tempCharStats.characterWalkAnimation = playerClasses[i].characterWalkAnim;

            if(playerClasses[i].equippedWeapon != null)
            {
                tempCharStats.equippedWeapon = playerClasses[i].equippedWeapon;

            }
            else
            {
                tempCharStats.equippedWeapon = MasterEquipmentList.instance.totalEquipmentListing[0];
                
            }
            if (playerClasses[i].equippedArmor != null)
            {
                tempCharStats.equippedArmor = playerClasses[i].equippedArmor;

            }
            else
            {
                tempCharStats.equippedArmor = MasterEquipmentList.instance.totalEquipmentListing[0];
            }
            if (playerClasses[i].equippedAccessory != null)
            {
                tempCharStats.equippedAccessory = playerClasses[i].equippedAccessory;
            }
            else
            {
                tempCharStats.equippedAccessory = MasterEquipmentList.instance.totalEquipmentListing[0];
            }




            tempCharStats.currentPower = tempCharStats.equippedWeapon.power + tempCharStats.equippedArmor.power + tempCharStats.equippedAccessory.power;
            tempCharStats.currentArmor = tempCharStats.equippedArmor.armor + tempCharStats.equippedWeapon.armor + tempCharStats.equippedAccessory.armor;
            tempCharStats.currentMagAtk = tempCharStats.equippedArmor.magicAttack + tempCharStats.equippedWeapon.magicAttack + tempCharStats.equippedAccessory.magicAttack;
            tempCharStats.currentMagDef = tempCharStats.equippedArmor.magicDefense + tempCharStats.equippedWeapon.magicDefense + tempCharStats.equippedAccessory.magicDefense;
            tempCharStats.currentCrit = (5+(Mathf.FloorToInt(tempCharStats.currentLuck/3))) + tempCharStats.equippedWeapon.crit + tempCharStats.equippedArmor.crit + tempCharStats.equippedAccessory.crit;
            tempCharStats.currentDodge = (5 + (Mathf.FloorToInt(tempCharStats.currentLuck/10))+ tempCharStats.equippedWeapon.dodge + tempCharStats.equippedArmor.dodge + tempCharStats.equippedAccessory.dodge);
            tempCharStats.currentSpeed = tempCharStats.currentSpeed + tempCharStats.equippedArmor.speed + tempCharStats.equippedWeapon.speed + tempCharStats.equippedAccessory.speed;

            
            for (int j = 0; j < tempCharStats.weaponEquippedMateria.Length; j++)
            {
                tempCharStats.weaponEquippedMateria[j] = playerClasses[i].weaponEquippedMateria[j];
            }
            for (int j = 0; j < tempCharStats.armorEquippedMateria.Length; j++)
            {
                tempCharStats.armorEquippedMateria[j] = playerClasses[i].armorEquippedMateria[j];
            }
            for (int j = 0; j < tempCharStats.accEquippedMateria.Length; j++)
            {
                tempCharStats.accEquippedMateria[j] = playerClasses[i].accEquippedMateria[j];
            }
            
        }
    }

    public void TurnOnRegularJoystick()
    {
        if (joystickEnabled)
        {
            joystickMaster.SetActive(true);
            for (int i = 0; i < regularJoystick.Length; i++)
            {
                regularJoystick[i].SetActive(true);
            }
            directionalJoystick.SetActive(false);
        }
        else
        {
            joystickMaster.SetActive(false);
        }
    }

    public void TurnOnDirectionalJoystick()
    {
        if (joystickEnabled)
        {
            joystickMaster.SetActive(true);
            for (int i = 0; i < regularJoystick.Length; i++)
            {
                regularJoystick[i].SetActive(false);
            }
            directionalJoystick.SetActive(true);
        }
        else
        {
            joystickMaster.SetActive(false);
        }


    }

    public void SetVsync0_120FPS()
    {//turns of vsync and tries to get 120fps, best for stable framerate
        QualitySettings.vSyncCount = 0;
        Application.targetFrameRate = 120;
    }

    public void SetVsync0_Default()
    {
        QualitySettings.vSyncCount = 0;
        Application.targetFrameRate = -1;
    }
    
    public void SetVsync1            ()    {    QualitySettings.vSyncCount = 1;    Application.targetFrameRate =  -1;    }


}
