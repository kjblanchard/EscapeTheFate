using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace ETF.TripleTriad
{
    public class TripleTriadCard : MonoBehaviour
    {
        protected static TripleTriadManager ttMan;

        public Card whatCardIAm;
        [SerializeField] protected Image cardImage;

        public Animator cardAnimator;

        [SerializeField] public Transform fingerPointer;
        [SerializeField] public bool cardOwnedByPlayer;

        

        private void Awake()
        {
            ttMan = FindObjectOfType<TripleTriadManager>();
        }

        public void FlipCard() // used for enemy cards currently for flipping
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



        public void ChangeImageToProperPlayer()
        {
            if (cardOwnedByPlayer)
            {
                cardImage.sprite = whatCardIAm.AImage;   
            }
            else
            {
                cardImage.sprite = whatCardIAm.BImage;
            }
        }

        
        public void ChangeImageToPlayer()
        {
            cardImage.sprite = whatCardIAm.AImage;
        }
        
        public void ChangeImageToEnemy()
        {
            cardImage.sprite = whatCardIAm.BImage;
        }

        public void ControlUiLoadingModifier(int shouldIBeLoading)
        {
            if (shouldIBeLoading == 0)
            {
                ttMan.ttUi.isLoading = true;
            }
            else
            {
                ttMan.ttUi.isLoading = false;
            }

        }


        

        
    }

}