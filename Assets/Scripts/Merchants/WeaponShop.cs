using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WeaponShop : MonoBehaviour
{
    public Dialog dialog;
    //public Dialog[] dialogLoop;
    private int whichDialogToDisplay;
    public bool isPerson;
    private bool canActivate;
    [SerializeField] private int sfxToPlayWhenOpening;
    [SerializeField] private bool shouldCreateDialogChoice;
    [SerializeField] private string[] choiceDialog;
    public bool saidInitialDialog = false;

    [TextArea(5, 5)]
    [SerializeField] string shopEnterText;

    [TextArea(5, 5)]
    [SerializeField] string shopExitText;
    
    
    public string npcName = null;
    public Sprite npcSprite = null;

    public bool showPortrait;
    public bool showName;

    //[SerializeField] Item[] itemsToSell;
    [SerializeField] private Equipment[] weaponsToSell;
    //[SerializeField] private int[] weaponPrice;
    //[SerializeField] private Materia[] materiaToSell;

    private void Update()
    {


        if(canActivate && !GameManager.instance.inShop && (Input.GetKeyDown(KeyCode.Space) || Input.GetButtonDown("Fire1")) &&
         !DialogManager.instance.dialogBoxGameObject.activeInHierarchy && GameManager.instance.ttActive == false)
        {
            SoundManager.instance.PlaySFX(sfxToPlayWhenOpening);
            GameManager.instance.makingDecision = true;
            ActivateDialog(dialog);
            saidInitialDialog = true;
            for (int i = 0; i < choiceDialog.Length; i++)
            {
                DialogManager.instance.choiceText[i].text = choiceDialog[i];
            }

            ModifyShopText();

            ModifyEquipmentList();

        }
        /*
        else if (canActivate && !GameManager.instance.inShop &&  ((Input.GetKeyDown(KeyCode.Space)) || (Input.GetButtonDown("Fire1"))) &&
                 !DialogManager.instance.dialogBoxGameObject.activeInHierarchy &&
                 GameManager.instance.ttActive == false)
        {
            SoundManager.instance.PlaySFX(sfxToPlayWhenOpening);
            GameManager.instance.makingDecision = true;
            ActivateDialog(dialog);
            saidInitialDialog = true;
        } *///Not sure why this is in here, it looks the same so I commented it out?
    }

    private void ModifyEquipmentList()
    {
        Debug.Log("ran the void " + System.Reflection.MethodBase.GetCurrentMethod().Name);
        ShopUI.instance.shopEquipmentList.Clear();

        for (int i = 0; i < weaponsToSell.Length; i++)
        {
            ShopUI.instance.shopEquipmentList.Add(weaponsToSell[i]);
        }
    }

    private void ModifyShopText()
    {
        ShopUI.instance.variableShopEnterText = shopEnterText;
        ShopManager.instance.shopDialog.dialogText.text = shopEnterText;
        ShopUI.instance.variableShopExitText = shopExitText;
    }

    private void ActivateDialog(Dialog whatToSay)
    {
        DialogManager.instance.ShowDialog(whatToSay,isPerson,showPortrait,npcSprite,npcName);
    }

    private void OnTriggerEnter2D(Collider2D other)
    {
        canActivate = true;
        
    }

    private void OnTriggerExit2D(Collider2D other)
    {
        canActivate = false;
        //SoundManager.instance.PlayIntroLoop(SoundManager.instance.bgmToPlay);
    }
}
