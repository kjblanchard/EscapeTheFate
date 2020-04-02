using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{
    public partial class TTUI
    {
        [SerializeField] GameObject playerSelectionFinger;

        public void InitializePlayerTurnCanvasFromTurnSelection()
        {
            enemyHandDialogBoxAnimator.gameObject.SetActive(false);
            setFocusOnPlayerCardSelected();
        }

        public void keepPlayerSelectionFingerOnProperLocation()
        {
            if (isLoading == false)
            {
                if (!playerSelectionFinger.gameObject.activeInHierarchy)
                {
                    
                    playerSelectionFinger.gameObject.SetActive(true);
                }

                playerSelectionFinger.transform.position =
                    myHandFingerPoints[ttdb.RetrievePlayerTurnCurrentSelection()].transform.position;
            }
            else
            {
                playerSelectionFinger.gameObject.SetActive(false);
            }

        }

        public void setFocusOnPlayerCardSelected()
        {
            myHandCardAnimators[ttdb.RetrievePlayerTurnCurrentSelection()].SetTrigger("inFocus");
        }

        public void setOutFocusOnPlayerCardSelected()
        {
            myHandCardAnimators[ttdb.RetrievePlayerTurnCurrentSelection()].SetTrigger("outFocus");

        }

    }
}
