using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public partial class TTUI
{

    [Header("Stuff For EnemyHandSelection")]
    [SerializeField] Animator[] enemyHandAnimator;

    [SerializeField] Animator enemyHandDialogBoxAnimator;
    private float _randomNumberForCardDisplaying;
    [SerializeField]  Animator blinkingHorizontalFingerAnimator;

    public IEnumerator PlayEnemyFullHandAnimation()
    {
        for (int i = 0; i < enemyHandAnimator.Length; i++)
        {
            _randomNumberForCardDisplaying = Random.Range(0.05f, 0.2f);
            yield return new WaitForSeconds(_randomNumberForCardDisplaying);
            PlayEnemyHandAnimation(i);
        }
        StartCoroutine(PlayEnemyHandCardFlipAnimations());
    }
    
    private void PlayEnemyHandAnimation(int cardToDisplay)
    {
        enemyHandAnimator[cardToDisplay].Play("MyHandSelect");
        SoundManager.instance.PlaySFX(6);
    }
    
    public IEnumerator InitializeEnemyHandSelectScreenUI()
    {
        isLoading = true;
        PlayCardSelectCanvasLeavingAnimation();
        PlayCardSelectCardDisplayInfoLeavingAnimation();
        PlayCardConfirmWindowLeavingAnimation();
        PlayEnemyHandDialogBoxAnimation();
        yield return new WaitForSeconds(0.5f);
        cardSelectionGameObject.SetActive(false);
        CardConfirmCanvas.SetActive(false);

        StartCoroutine(PlayEnemyFullHandAnimation());
    }

    public void PlayCardSelectCanvasLeavingAnimation()
    {
        cardSelectionWindowAnimator.Play("CardWindowLeaving");
    }

    public void PlayCardSelectCardDisplayInfoLeavingAnimation()
    {
        cardSelectionCardDisplayAnimator.Play("CardDisplayLeaving");
    }
    
    public void PlayCardConfirmWindowLeavingAnimation()
    {
        cardConfirmAnimator.Play("CardConfirmWindowLeaving");
    }

    public void PlayEnemyHandDialogBoxAnimation()
    {
        enemyHandDialogBoxAnimator.Play("EnemyHandDialogBoxDisplaying");
    }

    public IEnumerator PlayEnemyHandCardFlipAnimations()
    {
        for (int i = 0; i < ttdb.currentEnemyTripleTriadCards.Length; i++)
        {
            ttdb.currentEnemyTripleTriadCards[i].PlayFlipCardAnimation();
            yield return new WaitForSeconds(0.2f);
        }

        yield return new WaitForSeconds(1f);
        isLoading = false;
        ttMan.ttEnemyHandSelectionProcessor.changeTheTextToSelectingTurn();
        switchToFingerTurnSelectionAnimation();
    }

    public void switchToFingerTurnSelectionAnimation()
    {
        SoundManager.instance.PlaySFX(5);
        blinkingHorizontalFingerAnimator.Play("TurnSelectionFinger");
    }
}
