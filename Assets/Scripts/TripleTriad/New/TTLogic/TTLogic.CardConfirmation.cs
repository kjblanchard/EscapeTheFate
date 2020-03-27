using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public partial class TTLogic
{

    public bool CanIScrollOnCardConfirm(bool isIncrementing, int currentFingerPos)
    {
        if (ttUi.isLoading) { return false; }

        if (isIncrementing)
        {
            if(currentFingerPos < ttUi.ReturnCardConfirmFingerPosCount()-1)
            {
                return true;
            }
        }
        else
        {
            if (currentFingerPos > 0)
            {
                return true;
            }
        }
        return false;

    }

    public bool CanIPressCancelButtonOnConfirmScreen(int currentFingerPosition)
    {
        if(ttUi.isLoading){return false;}

        return currentFingerPosition != 1;
    }
    public bool CanIGoBackToCardSelectionScreenFromConfirmScreen(int currentFingerPosition)
    {
        if(ttUi.isLoading){return false;}
        return currentFingerPosition == 1;
    }
    
}
