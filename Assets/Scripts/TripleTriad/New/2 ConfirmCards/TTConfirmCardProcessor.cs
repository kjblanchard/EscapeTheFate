using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TTConfirmCardProcessor : MonoBehaviour
{
    [Header("Script References")]
    [SerializeField] private TTUI ttUi;
    [SerializeField] private TTLogic ttLogic;
    
    [Header("Finger Memory and Enum")]
    private int _currentFingerPosition;
    private enum WhichWayToScroll
    {
        IncrementingFinger,
        DecrementingFinger,
    }

    public void TtConfirmCardProcessor()
    {
        if ((Input.GetKeyDown(KeyCode.D)|| Input.GetButtonDown("right")) && ttLogic.CanIScrollOnCardConfirm(true, _currentFingerPosition))
        {
            CardConfirmLeftRightScroll(WhichWayToScroll.IncrementingFinger);
        }
        else if ((Input.GetKeyDown(KeyCode.A) || Input.GetButtonDown("left")) && ttLogic.CanIScrollOnCardConfirm(false, _currentFingerPosition))
        {
            CardConfirmLeftRightScroll(WhichWayToScroll.DecrementingFinger);
        }
        else if ((Input.GetKeyDown(KeyCode.C) || Input.GetButtonDown("Fire2")) && ttLogic.CanIPressCancelButtonOnConfirmScreen(_currentFingerPosition))
        {
            ChangeToGoBackOnCancelButton();
        }
        else if ((Input.GetKeyDown(KeyCode.Space) || Input.GetButtonDown("Fire1")) && ttLogic.CanIGoBackToCardSelectionScreenFromConfirmScreen(_currentFingerPosition))
        {
            TripleTriadManager.instance.ChangeBackToChoosingCards();
        }
        else if ((Input.GetKeyDown(KeyCode.Space) || Input.GetButtonDown("Fire1")) && _currentFingerPosition==0)
        {
            TripleTriadManager.instance.ChangeFromCardConfirmationToChooseEnemyHand();
        }
        
        ttUi.KeepFingerOnProperLocationInCardConfirmation(_currentFingerPosition);        

    }

    private void CardConfirmLeftRightScroll(WhichWayToScroll directionToMove)
    {
        switch (directionToMove)
        {
         case WhichWayToScroll.IncrementingFinger:
             _currentFingerPosition++;
             SoundManager.instance.PlaySFX(0);
             break;
         case    WhichWayToScroll.DecrementingFinger:
             _currentFingerPosition--;
             SoundManager.instance.PlaySFX(0);
             break;
        }

    }

    private void ChangeToGoBackOnCancelButton()
    {
        _currentFingerPosition = 1;
        SoundManager.instance.PlaySFX(0);
    }


    public void InitializeCardConfirmScreen()
    {
        TripleTriadManager.instance.ttCardSelectProcessor.StopTheCoroutineScrolling();
        ttUi.InitializeCardConfirmScreen();
        _currentFingerPosition = 0;
    }
}
