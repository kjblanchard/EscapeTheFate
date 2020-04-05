using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{
    public class PlayerTurnState : TtState
    {
        #region Configuration

        [SerializeField] private TripleTriadManager ttMan;
    

        #endregion


        public override void Startup()
        {
            if (!ttMan.ttDb.RetrievePlayerTurnIsComingFromCancelScreen())
            {
                ttMan.ttDb.InitializePlayerTurnDbValues();
                ttMan.ttUi.InitializePlayerTurnCanvasFromTurnSelection();
            }
            else
            {
                ttMan.ttUi.InitializePlayerTurnCanvasFromLocationSelectionCancel();
            }
        }

        public override void Execute()
        {
            ListenForInput();
            
            ttMan.ttUi.keepPlayerSelectionFingerOnProperLocation();
        }

        private void ListenForInput()
        {
            if ((Input.GetKeyDown(KeyCode.W) || Input.GetButtonDown("up")))
            {
                if(ttMan.ttLogic.CanIScrollUpInPlayerTurn())
                {

                    ttMan.ttUi.setOutFocusOnPlayerCardSelected();
                    ttMan.ttDb.MovePlayerTurnCurrentSelectionUp();
                    SoundManager.instance.PlaySFX(0);
                    ttMan.ttUi.setFocusOnPlayerCardSelected();
                }
                else if(ttMan.ttLogic.CanILoopUpInPlayerTurn())
                {
                    
                    ttMan.ttUi.setOutFocusOnPlayerCardSelected();
                    ttMan.ttDb.MovePlayerTurnCurrentSelectionToBottom();
                    SoundManager.instance.PlaySFX(0);
                    ttMan.ttUi.setFocusOnPlayerCardSelected();
                }
            }
            else if ((Input.GetKeyDown(KeyCode.S) || Input.GetButtonDown("down")))
            {
                if (ttMan.ttLogic.CanIScrollDownInPlayerTurn())
                {
                    // if (!ttMan.ttLogic.HasThisCardBeenPlayedInPlayerTurn())
                    // {
                    ttMan.ttUi.setOutFocusOnPlayerCardSelected();
                    ttMan.ttDb.MovePlayerTurnCurrentSelectionDown();
                    SoundManager.instance.PlaySFX(0);
                    ttMan.ttUi.setFocusOnPlayerCardSelected();
                    // }
                    // else
                    // {
                    //     
                    //     ttMan.ttUi.setOutFocusOnPlayerCardSelected();
                    //     SoundManager.instance.PlaySFX(0);
                    //     ttMan.ttDb.SearchForNextUsableCardInCurrentHandAndChangeSelection();
                    //     ttMan.ttUi.setFocusOnPlayerCardSelected();
                    //
                    // }

                }
                else if (ttMan.ttLogic.CanILoopDownInPlayerTurn())
                {
                    ttMan.ttUi.setOutFocusOnPlayerCardSelected();
                    ttMan.ttDb.MovePlayerTurnCurrentSelectionTop();
                    SoundManager.instance.PlaySFX(0);
                    ttMan.ttUi.setFocusOnPlayerCardSelected();
                }
            }
            else if ((Input.GetKeyDown(KeyCode.Space) || Input.GetButtonDown("Fire1")))
            {
                ttMan.SendStateChange(ttMan.locationSelectionState);
            }
        }

        public override void End()
        {
        }
        
    }
}
