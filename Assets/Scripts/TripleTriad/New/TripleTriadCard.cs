using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace ETF.TripleTriad
{
    public class TripleTriadCard : MonoBehaviour
    {
        private static TripleTriadManager ttMan;

        public Card whatCardIAm;
        [SerializeField] Image cardImage;

        public Animator cardAnimator;

        private bool _hasPlayedCard;

        private int _locationToGoTo;
        // public bool shouldIflipTheCard;

        private void Awake()
        {
            ttMan = FindObjectOfType<TripleTriadManager>();
        }

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
        
        public void CardPlayed()
        {
            cardAnimator.SetTrigger("isPlayed");
        }
        
        public void MoveToBoard()
        {
            cardAnimator.SetTrigger("moveToBoard");
        }

        public void SetTtCardToHaveNotPlayed()
        {
            _hasPlayedCard = false;
        }

        public void SetTtCardToHavePlayed()
        {
            _hasPlayedCard = true;
        }

        public void SetLocationToGoTo(int boardLocation)
        {
            _locationToGoTo = boardLocation;
        }

        public void SendCallToUiToStartBoardAnimation()
        {
            ttMan.ttUi.PlayCardToBoardInLocationSelection(_locationToGoTo,whatCardIAm);
        }

        public void ChangeImage()
        {
            cardImage.sprite = whatCardIAm.AImage;
        }
        
    }

}