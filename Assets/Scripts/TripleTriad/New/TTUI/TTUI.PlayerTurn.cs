using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public partial class TTUI
{
    [SerializeField] GameObject playerSelectionFinger;
    
    public void InitializePlayerTurnCanvasFromTurnSelection()
    {
        enemyHandDialogBoxAnimator.gameObject.SetActive(false);
        setFocusOnPlayerCardSelected(0);
    }

    public void keepPlayerSelectionFingerOnProperLocation(int fingerPointLocationToGoTo)
    {
        if (isLoading == false)
        {
            if (!playerSelectionFinger.gameObject.activeInHierarchy)
            {
                fingerPrefab.gameObject.SetActive(true);
            }

            playerSelectionFinger.transform.position = myHandFingerPoints[fingerPointLocationToGoTo].transform.position;
        }
        else
        {
            fingerPrefab.gameObject.SetActive(false);
        }

    }

    public void setFocusOnPlayerCardSelected(int currentPlayerCardSelected)
    {
        myHandCardAnimators[currentPlayerCardSelected].SetTrigger("inFocus");
    }

    public void setOutFocusOnPlayerCardSelected(int currentPlayerCardSelected)
    {
        myHandCardAnimators[currentPlayerCardSelected].SetTrigger("outFocus");

    }

}
