using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TTConfirmCardProcessor : MonoBehaviour
{
    [SerializeField] private TTUI ttUi;
    [SerializeField] private TTLogic ttLogic;
    private int _currentFingerPosition = 0;
    private bool _needsToInitializeFields;
    public void TtConfirmCardProcessor()
    {
        if (_needsToInitializeFields){InitializeFields();}


        if (Input.GetKeyDown(KeyCode.D) && ttLogic.CanIScrollOnCardConfirm(true, _currentFingerPosition))
        {
            _currentFingerPosition++;
        }
        else if (Input.GetKeyDown(KeyCode.A) && ttLogic.CanIScrollOnCardConfirm(false, _currentFingerPosition))
        {
            _currentFingerPosition--;
        }
        else if (Input.GetKeyDown(KeyCode.Space) && _currentFingerPosition == 1 && !ttUi.isLoading)
        {
            TripleTriadManager.instance.ChangeBackToChoosingCards();
        }
        
        ttUi.KeepFingerOnProperLocationInCardConfirmation(_currentFingerPosition);        

    }





    private void InitializeFields()
    {
        _needsToInitializeFields = false;
        TripleTriadManager.instance.ttCardSelectProcessor.StopTheCoroutineScrolling();
        ttUi.InitializeCardConfirmScreen();
        _currentFingerPosition = 0;
    }
    
    public void TurnOnInitializer()
    {
        _needsToInitializeFields = true;
    }
}
