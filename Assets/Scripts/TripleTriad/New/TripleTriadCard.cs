using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TripleTriadCard : MonoBehaviour
{

    public Card whatCardIAm;
    [SerializeField] Image cardImage;
    public Animator cardAnimator;
     // public bool shouldIflipTheCard;


    public void FlipCard()
    {
        cardImage.sprite = whatCardIAm.BImage;
    }

    public void SetToEmptyCardBack()
    {
        cardImage.sprite = CardInventory.instance.emptyCard.BImage;
    }

    public void SetMyCurrentCard(Card whatCardImGonnaBe)
    {
        whatCardIAm = whatCardImGonnaBe;
    }

    public void PlayFlipCardAnimation()
    {
        cardAnimator.Play("CardFlip");
    }

    public void PlayCardFlipSound()
    {
        SoundManager.instance.PlaySFX(4);
    }

    public void InFocus()
    {
        cardAnimator.SetTrigger("inFocus");
    }
    public void OutFocus()
    {
        cardAnimator.SetTrigger("outFocus");
    }
}
