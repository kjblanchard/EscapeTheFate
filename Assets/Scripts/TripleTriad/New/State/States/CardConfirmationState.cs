﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{
    public class CardConfirmationState : TtState
    {
        #region Configuration

        [Header("Script References")] [SerializeField]
        TripleTriadManager ttMan;

        public enum WhichWayToScroll
        {
            IncrementingFinger,
            DecrementingFinger,
        }


        #endregion

        public override void Startup()
        {
            ttMan.ttDb.InitializeCardConfirmationDbValues();
            ttMan.ttUi.InitializeCardConfirmScreen();
        }

        public override void Execute()
        {
            if ((Input.GetKeyDown(KeyCode.D) || Input.GetButtonDown("right")) &&
                ttMan.ttLogic.CanIScrollOnCardConfirm(WhichWayToScroll.IncrementingFinger))
            {
                CardConfirmLeftRightScroll(WhichWayToScroll.IncrementingFinger);
            }
            else if ((Input.GetKeyDown(KeyCode.A) || Input.GetButtonDown("left")) &&
                     ttMan.ttLogic.CanIScrollOnCardConfirm(WhichWayToScroll.DecrementingFinger))
            {
                CardConfirmLeftRightScroll(WhichWayToScroll.DecrementingFinger);
            }
            else if ((Input.GetKeyDown(KeyCode.C) || Input.GetButtonDown("Fire2")) &&
                     ttMan.ttLogic.CanIPressCancelButtonOnConfirmScreen())
            {
                ChangeToGoBackOnCancelButton();
            }
            else if ((Input.GetKeyDown(KeyCode.Space) || Input.GetButtonDown("Fire1")) &&
                     ttMan.ttLogic.CanIGoBackToCardSelectionScreenFromConfirmScreen())
            {
                ttMan.SendChangeToPreviousState();
            }
            else if ((Input.GetKeyDown(KeyCode.Space) || Input.GetButtonDown("Fire1")) &&
                     ttMan.ttLogic.CanIAdvanceToChooseEnemyHandScreen())
            {
                ttMan.SendStateChange(ttMan.enemyHandSelectionState);
            }

            ttMan.ttUi.KeepFingerOnProperLocationInCardConfirmation(ttMan.ttDb
                .RetrieveCurrentFingerPositionInCardConfirmation());
        }

        public override void End()
        {
            //ttMan.cardSelectionState.ChangeGameStateBackToOriginal(); Put this somewhere, not here.
        }

        #region Functions

        private void CardConfirmLeftRightScroll(WhichWayToScroll directionToMove)
        {
            switch (directionToMove)
            {
                case WhichWayToScroll.IncrementingFinger:
                    ttMan.ttDb.CardConfirmLeftRightUpdateDb(directionToMove);
                    SoundManager.instance.PlaySFX(0);
                    break;
                case WhichWayToScroll.DecrementingFinger:
                    ttMan.ttDb.CardConfirmLeftRightUpdateDb(directionToMove);
                    SoundManager.instance.PlaySFX(0);
                    break;
            }

        }

        private void ChangeToGoBackOnCancelButton()
        {
            ttMan.ttDb.CardConfirmCancelButtonPress();
            SoundManager.instance.PlaySFX(0);
        }


        #endregion
    }
}
