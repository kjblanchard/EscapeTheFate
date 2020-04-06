using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace ETF.TripleTriad
{
    public partial class TTUI
    {

        [Header("Stuff For EnemyHandSelection")]
        //[SerializeField] Animator[] enemyHandAnimator;

        [SerializeField]
        Animator enemyHandDialogBoxAnimator;

        private float _randomNumberForCardDisplaying;
        [SerializeField] Animator turnIndicatorFingerAnimator;
        [SerializeField] private Text enemyDialogBoxNameText;
        [SerializeField] private Text enemyDialogBoxDialogText;

        private IEnumerator PlayEnemyFullHandAnimationCo()
        {
            _randomNumberForCardDisplaying = Random.Range(0.05f, 0.12f);
            var _cachedWaitForSeconds = new WaitForSeconds(_randomNumberForCardDisplaying);
            
            for (int i = 0; i < ttdb.fullEnemyTripleTriadCards.Length; i++)
            {

                yield return _cachedWaitForSeconds;
                PlayEnemyHandAnimation(i);
            }

            StartCoroutine(PlayEnemyHandCardFlipAnimations());
        }

        private void PlayEnemyHandAnimation(int cardToDisplay)
        {
            ttdb.fullEnemyTripleTriadCards[cardToDisplay].cardAnimator.Play("MyHandSelect");
            SoundManager.instance.PlaySFX(6);
        }

        public IEnumerator InitializeEnemyHandSelectScreenUi()
        {
            //initializes the gameobjects and activates them, also loads in texts to be displayed in the boxes, as well as turning off the things after the animations are done playing;
            isLoading = true;
            PlayCardSelectCanvasLeavingAnimation();
            PlayCardSelectCardDisplayInfoLeavingAnimation();
            PlayCardConfirmWindowLeavingAnimation();
            UpdateEnemyNameInTextBox();
            UpdateEnemyDialogBoxToChoosingCards();
            PlayEnemyHandDialogBoxAnimation();
            yield return new WaitForSeconds(0.5f);
            // cardSelectionGameObject.SetActive(false);
            // CardConfirmCanvas.SetActive(false);

            StartCoroutine(PlayEnemyFullHandAnimationCo());
        }

        private void PlayCardSelectCanvasLeavingAnimation()
        {
            cardSelectionWindowAnimator.Play("CardWindowLeaving");
        }

        private void PlayCardSelectCardDisplayInfoLeavingAnimation()
        {
            cardSelectionCardDisplayAnimator.Play("CardDisplayLeaving");
        }

        private void PlayCardConfirmWindowLeavingAnimation()
        {
            cardConfirmAnimator.Play("CardConfirmWindowLeaving");
        }

        private void PlayEnemyHandDialogBoxAnimation()
        {
            enemyHandDialogBoxAnimator.Play("EnemyHandDialogBoxDisplaying");
        }

        private IEnumerator PlayEnemyHandCardFlipAnimations()
        {
            var _cachedWaitForSeconds = new WaitForSeconds(0.2f);
            for (int i = 0; i < ttdb.fullEnemyTripleTriadCards.Length; i++)
            {
                ttdb.fullEnemyTripleTriadCards[i].PlayFlipCardAnimation();
                yield return _cachedWaitForSeconds;
            }

            yield return new WaitForSeconds(1f);
            isLoading = false;
            ttMan.SendStateChange(ttMan.turnSelectionState);
        }


        public void UpdateEnemyNameInTextBox()
        {
            enemyDialogBoxNameText.text = ttdb.WhatIsTheEnemyNpcName() + " :";
        }

        public void UpdateEnemyDialogBoxToChoosingCards()
        {
            enemyDialogBoxDialogText.text = ttdb.WhatWillTheEnemySayWhenChoosingCards();
        }


    }
}
