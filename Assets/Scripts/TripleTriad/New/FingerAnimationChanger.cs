using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{
    public class FingerAnimationChanger : MonoBehaviour
    {
        [SerializeField] private TripleTriadManager _ttMan;
        [SerializeField] Animator fingerAnimator;
        private static readonly int kWhichWay = Animator.StringToHash("WhichWay");


        public void ChangeTurn()
        {
            if (_ttMan.RetrieveCurrentState() == _ttMan.locationSelectionState)
            {
                PlayChangeToEnemyTurnAnimation();
                //ChangeToEnemyTurn();
            }
            else
            {
                PlayChangeToPlayerTurnAnimation();
                //ChangeToPlayerTurn();
            }
        }
        
        
        private void ChangeToPlayerTurn()
        {//these are used to call the change of state from within the animation
            _ttMan.ttDb.ModifyPlayerTurnIsCancelling(false);
            _ttMan.ttUi.InitializeBetweenTurnUiEnemyToPlayer();
            _ttMan.SendStateChange(_ttMan.playerTurnState);
            //print("playerturn" + _ttMan.RetrieveCurrentState());
        }
        private void ChangeToEnemyTurn()
        {
            //ttMan.ttDb.ModifyPlayerTurnIsCancelling(false);
            _ttMan.SendStateChange(_ttMan.enemyTurnState);
        }
        

        public void ChooseWhichAnimationToPlay()
        {
            if (TripleTriadManager.instance.ttDb.ReturnWhichPlayerGetsToGoFirst() == 0)
            {//these are used when the turn is selected in player turn
                fingerAnimator.SetTrigger("GoToPlayerSide");
                //play the going to me
            }
            else
            {
                fingerAnimator.SetTrigger("GoToEnemySide");
                //play the going to me
            }
        }

        private void PlayChangeToEnemyTurnAnimation()
        {//these are used to call from ttman to start the animation/turn change
            fingerAnimator.SetTrigger("PlayerToEnemy");
        }
        
        private void PlayChangeToPlayerTurnAnimation()
        {
            fingerAnimator.SetTrigger("EnemyToPlayer");
        }

        private void ChangeFromPlayerToEnemyState()
        {//for fingerturning off, could probably change the name in the future if nothing else comes of this.
            _ttMan.ttUi.InitializeBetweenTurnUiPlayerToEnemy();
        }
        
        private void ChangeFromEnemyToPlayerState()
        {//for fingerturning off, could probably change the name in the future if nothing else comes of this.
            _ttMan.ttUi.InitializeBetweenTurnUiEnemyToPlayer();
        }

    }

}