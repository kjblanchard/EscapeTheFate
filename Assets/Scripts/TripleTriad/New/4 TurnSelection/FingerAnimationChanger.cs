using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FingerAnimationChanger : MonoBehaviour
{
    [SerializeField] Animator fingerAnimator;
    private static readonly int kWhichWay = Animator.StringToHash("WhichWay");

    

    public void ChooseWhichAnimationToPlay()
    {
        if (TripleTriadManager.instance.ttDb.ReturnWhichPlayerGetsToGoFirst() == 0)
        {
            fingerAnimator.SetTrigger("GoToPlayerSide");
            //play the going to me
        }
        else
        {
            fingerAnimator.SetTrigger("GoToEnemySide");
            //play the going to me
        }
    }

    public void ChangeToPlayerTurn()
    {
        TripleTriadManager.instance.SendStateChange(TripleTriadManager.instance.playerTurnState);
    }
    
    // public void ChangeToEnemyTurn()
    // {
    //     TripleTriadManager.instance.ChangeFromTurnSelectionToEnemyTurn();
    // }
    
}
