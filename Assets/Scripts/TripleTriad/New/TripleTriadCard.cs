using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TripleTriadCard : MonoBehaviour
{

    public Card whatCardIAm;
    [SerializeField] Image cardImage;
    [SerializeField]  Animator cardAnimator;
     // public bool shouldIflipTheCard;


    public void FlipCard()
    {
        cardImage.sprite = whatCardIAm.BImage;
    }

    public void SetToEmptyCardBack()
    {
        cardImage.sprite = CardInventory.instance.emptyCard.BImage;
    }

    // private void Update()
    // {
    //     if (shouldIflipTheCard)
    //     {
    //         shouldIflipTheCard = false;
    //         cardAnimator.Play("CardFlip");
    //     }
    // }

    public void SetMyCurrentCard(Card whatCardImGonnaBe)
    {
        whatCardIAm = whatCardImGonnaBe;
    }

    public void PlayFlipCardAnimation()
    {
        cardAnimator.Play("CardFlip");
    }
}
