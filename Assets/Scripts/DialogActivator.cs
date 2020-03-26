using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DialogActivator : MonoBehaviour
{
    public Dialog dialog;
    public bool isPerson;
    private bool canActivate;

    public bool isSaveCrystal;

    public Dialog afterBattleDialogWin;
    public Dialog afterBattleDialogDraw;
    public Dialog afterBattleDialogLose;

    public string npcName = null;
    public Sprite npcSprite = null;

    public bool showPortrait;
    public bool showName;












    private void Update()
    {
        //added this top one for save sound, can remove after implementing the right way
        if (isSaveCrystal == true && canActivate == true && (Input.GetKeyDown(KeyCode.Space)|| Input.GetButtonDown("Fire1")) && !DialogManager.instance.dialogBoxGameObject.activeInHierarchy && GameManager.instance.ttActive == false && !GameManager.instance.dialogCloseDelay)
        {
            SoundManager.instance.PlaySFX(8);
            ActivateDialog(dialog);
//            Debug.Log("played the sound");
        }

       else if (canActivate == true && ((Input.GetKeyDown(KeyCode.Space))|| (Input.GetButtonDown("Fire1"))) && !DialogManager.instance.dialogBoxGameObject.activeInHierarchy && GameManager.instance.ttActive == false && !GameManager.instance.dialogCloseDelay)
        {
            ActivateDialog(dialog);
        }

        
        else if (canActivate == true && GameManager.instance.afterCardBattle == true)
        {
            GameManager.instance.afterCardBattle = false;
            if(GameManager.instance.afterCardBattleStatus == 0)
            {
                ActivateDialog(afterBattleDialogWin);
            }
            else if (GameManager.instance.afterCardBattleStatus == 1)
            {
                ActivateDialog(afterBattleDialogDraw);
            }
            else if (GameManager.instance.afterCardBattleStatus == 2)
            {
                ActivateDialog(afterBattleDialogLose);
            }

        }
        

    }



    
    public void ActivateDialog(Dialog whatToSay)
    {
        DialogManager.instance.ShowDialog(whatToSay,isPerson,showPortrait,npcSprite,npcName);
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
