using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TTActivator : MonoBehaviour
{
    public Dialog dialog;
    //public Dialog afterBattleDialog;
    private bool canActivate;
    public bool isPerson;
    public string npcName = null;
    public Sprite npcSprite = null;

    public bool showPortrait;
    public bool showName;











    private void Update()
    {
        if (canActivate == true && (Input.GetKeyDown(KeyCode.X)||Input.GetButtonDown("Fire3")) && !DialogManager.instance.dialogBoxGameObject.activeInHierarchy && !GameManager.instance.ttActive)
        {
            ActivateDialog(dialog);
        }



    }




    public void ActivateDialog(Dialog whatToSay)
    {
        DialogManager.instance.ShowDialog(whatToSay, isPerson,showPortrait,npcSprite,npcName);
        GameManager.instance.ttActive = true;
    }


    private void OnTriggerEnter2D(Collider2D other)
    {
        canActivate = true;
    }
    private void OnTriggerExit2D(Collider2D collision)
    {
        canActivate = false;
    }
}
