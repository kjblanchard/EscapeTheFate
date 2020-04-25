using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace ETF.TripleTriad
{
    public partial class TTUI : MonoBehaviour
    {
        //this is the master TTUI
        
        [Header("Script Reference")] [SerializeField]
        TTDB ttdb;

        [SerializeField] TripleTriadManager ttMan;
        // private static readonly int kProperty = Animator.StringToHash("default");
        // private static readonly int kMoveToBase = Animator.StringToHash("moveToBase");
        [SerializeField] private Canvas fingerCanvas;


        public void InitializeAndCacheAllUiElementsForGameStart(bool isGameEnded = false)
        {
            print("donethis");
            //start all Canvas's
            _titleScreenCanvas.enabled = true;
            _opponentSelectionCanvas.enabled = true;
            _showingRulesCanvas.enabled = true;
            _cardSelectionCanvas.enabled = true;
            _cardConfirmCanvas.enabled = true;
            var playerHand = ttdb.RetrieveAllPlayerHandCardsFull();
            for (int i = 0; i < playerHand.Length; i++)
            {
                playerHand[i].cardCanvas.enabled = true;
            }
            for (int i = 0; i < ttdb.fullEnemyTripleTriadCards.Length ; i++)
            {
                ttdb.fullEnemyTripleTriadCards[i].cardCanvas.enabled = true;
            }
            var boardCards = ttdb.RetrieveAllCardsOnBoard();
            for (int i = 0; i < boardCards.Length ; i++)
            {
                boardCards[i].cardCanvas.enabled = true;
            }

            _scoreDisplayCanvas.enabled = true;

            _enemyDialogBoxCanvas.enabled = true;
            _endGameCanvas.enabled = true;
            
            _cardRewardCanvas.enabled = true;
            _rewardConfirmCanvas.enabled = true;
            _cardWonCanvas.enabled = true;
            fingerCanvas.enabled = true;

            //reset all animations
            
             showingRulesAnimator.Rebind();
             cardSelectionWindowAnimator.Rebind();
             cardConfirmAnimator.Rebind();
             for (int i = 0; i < playerHand.Length; i++)
             {
                 playerHand[i].cardAnimator.Rebind();
             }
            
             for (int i = 0; i < ttdb.fullEnemyTripleTriadCards.Length; i++)
             {
                 ttdb.fullEnemyTripleTriadCards[i].cardAnimator.Rebind();
             }
            
             for (int i = 0; i < boardCards.Length; i++)
             {
                 boardCards[i].cardAnimator.Rebind();
             }
            
             enemyHandDialogBoxAnimator.Rebind();
             _youWinAnimator.Rebind();
             _youLoseAnimator.Rebind();
             _drawAnimator.Rebind();
             cardDisplayAnimator.Rebind();
             cardConfirmAnimator.Rebind();
             cardSelectionWindowAnimator.Rebind();
             cardSelectionCardDisplayAnimator.Rebind();
                
            //close all canvas'
            
            _titleScreenCanvas.enabled = false;
            _opponentSelectionCanvas.enabled = false;
            _showingRulesCanvas.enabled = false;
            _cardSelectionCanvas.enabled = false;
            _cardConfirmCanvas.enabled = false;
            for (int i = 0; i < playerHand.Length; i++)
            {
                playerHand[i].cardCanvas.enabled = false;
            }
            for (int i = 0; i < ttdb.fullEnemyTripleTriadCards.Length ; i++)
            {
                ttdb.fullEnemyTripleTriadCards[i].cardCanvas.enabled = false;
            }
            for (int i = 0; i < boardCards.Length ; i++)
            {
                boardCards[i].cardCanvas.enabled = false;
            }

            _scoreDisplayCanvas.enabled = false;

            _enemyDialogBoxCanvas.enabled = false;
            _endGameCanvas.enabled = false;
            
            _cardRewardCanvas.enabled = false;
            _rewardConfirmCanvas.enabled = false;
            _cardWonCanvas.enabled = false;
        }
        
        public void StartGameFadeIn()
        {
            _tripleTriadUiFade.animator.SetTrigger("startGame");
        }
        
    }
}

