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
                ttMan.ttUi.InitializePlayerTurnCanvas();
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



        public override void End()
        {
        }

        #region Functions

        private void ListenForInput()
        {
            if ((Input.GetKeyDown(KeyCode.W) || Input.GetButtonDown("up")))
            {
                if (ttMan.ttLogic.CanIScrollUpInPlayerTurn())
                {
                    ScrollUp();
                }
                else if (ttMan.ttLogic.CanILoopUpInPlayerTurn())
                {
                    LoopToBottom();
                }
            }
            else if ((Input.GetKeyDown(KeyCode.S) || Input.GetButtonDown("down")))
            {
                if (ttMan.ttLogic.CanIScrollDownInPlayerTurn())
                {
                    ScrollDown();
                }
                else if (ttMan.ttLogic.CanILoopDownInPlayerTurn())
                {
                    LoopToTop();
                }
            }
            else if ((Input.GetKeyDown(KeyCode.Space) || Input.GetButtonDown("Fire1")))
            {
                ttMan.SendStateChange(ttMan.locationSelectionState);
            }
            else if (Input.GetKeyDown(KeyCode.Y))
            {
                ttMan.ttDb.InitializeEnemyTurnDbValues();
                if (ttMan.ttDb.isEnemyTurn)
                {
                    ttMan.ttDb.isEnemyTurn = false;
                }
                else if (!ttMan.ttDb.isEnemyTurn)
                {
                    ttMan.ttDb.isEnemyTurn = true;
                }

                print($"is enemy turn is equal to {ttMan.ttDb.isEnemyTurn}");
            }
            else if (Input.GetKeyDown(KeyCode.P))
            {
                
                ttMan.ttLogic.FullEnemyTurnHandChoices();
            }
            else if (Input.GetKeyDown(KeyCode.L))
            {
                var boardLocation = ttMan.ttLogic.RetrieveBoardPositionToPlaceCard();
                var handPosition = ttMan.ttLogic.RetrieveHandPositionToPlaceCard();
                ttMan.ttDb.currentEnemyTripleTriadCardsInHand[handPosition].SetLocationToGoTo(boardLocation);
                ttMan.ttDb.ChangeBoardLocationToInPlayEnemy(boardLocation);
                ttMan.ttUi.setFocusOnEnemyCardSelected();
                ttMan.ttUi.PlayCardInEnemyTurn(handPosition);
                
                print($"the spot I'm going to put the card is {boardLocation} and I'm going to use this spot in my hand {handPosition}, which is card {ttMan.ttDb.currentEnemyTripleTriadCardsInHand[handPosition].whatCardIAm.name}");
            }
        }

        private void LoopToTop()
        {
            ttMan.ttUi.setOutFocusOnPlayerCardSelected();
            ttMan.ttDb.MovePlayerTurnCurrentSelectionTop();
            SoundManager.instance.PlaySFX(0);
            ttMan.ttUi.setFocusOnPlayerCardSelected();
        }

        private void ScrollDown()
        {
            ttMan.ttUi.setOutFocusOnPlayerCardSelected();
            ttMan.ttDb.MovePlayerTurnCurrentSelectionDown();
            SoundManager.instance.PlaySFX(0);
            ttMan.ttUi.setFocusOnPlayerCardSelected();
        }

        private void LoopToBottom()
        {
            ttMan.ttUi.setOutFocusOnPlayerCardSelected();
            ttMan.ttDb.MovePlayerTurnCurrentSelectionToBottom();
            SoundManager.instance.PlaySFX(0);
            ttMan.ttUi.setFocusOnPlayerCardSelected();
        }

        private void ScrollUp()
        {
            ttMan.ttUi.setOutFocusOnPlayerCardSelected();
            ttMan.ttDb.MovePlayerTurnCurrentSelectionUp();
            SoundManager.instance.PlaySFX(0);
            ttMan.ttUi.setFocusOnPlayerCardSelected();
        }

        #endregion
        
        
        
    }
}
