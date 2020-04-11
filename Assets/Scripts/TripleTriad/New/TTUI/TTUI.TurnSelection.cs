using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{
    public partial class TTUI
    {

        public void InitializeTurnSelection()
        {
            //turns on and updates elements needed for turn selection
            turnIndicatorFingerAnimator.gameObject.SetActive(true);
            _enemyScoreText.gameObject.SetActive(true);
            _playerScoreText.gameObject.SetActive(true);
            UpdateEnemyDialogBoxToTurnSelection();
        }

        public void UpdateEnemyDialogBoxToTurnSelection()
        {
            //changes the dialog box to display proper information for turn selection
            enemyDialogBoxDialogText.text = ttdb.WhatWillTheEnemySayWhenTurnSelection();
        }

        public void switchToFingerTurnSelectionAnimation()
        {
            SoundManager.instance.PlaySFX(5);
            turnIndicatorFingerAnimator.Play("TurnSelectionFinger");
        }
        
        public void LeavingTurnSelectionUiChange()
        {
            enemyHandDialogBoxAnimator.gameObject.SetActive(false);
            locationSelectionFinger.SetActive(false);
        }


    }
}
