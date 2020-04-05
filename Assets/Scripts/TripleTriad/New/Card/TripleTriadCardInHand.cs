using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace ETF.TripleTriad
{
    public class TripleTriadCardInHand : TripleTriadCard
    {
        
        private bool _hasPlayedCard;
        private int _locationToGoTo;
        //public int positionInCardHand;

        

        public void SendCallToUiToStartBoardAnimation()
        {
            ttMan.ttUi.PlayCardToBoardInLocationSelection(_locationToGoTo,whatCardIAm);
        }
        
        public void SetLocationToGoTo(int boardLocation)
        {
            _locationToGoTo = boardLocation;
        }
        // public void SetTtCardToHavePlayed()
        // {
        //     _hasPlayedCard = true;
        // }
        // public void SetTtCardToHaveNotPlayed()
        // {
        //     _hasPlayedCard = false;
        // }

        // public bool HaveIBeenPlayed()
        // {
        //     return _hasPlayedCard;
        // }
        
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

        public void MoveCardUpOneSpot()
        {
            cardAnimator.SetTrigger("moveUpOneSpot");
        }

        public void MoveToNewLocationInHandWhenCardIsPlayed(int posToMoveto)
        {
            cardAnimator.Play($"Pos{posToMoveto}SelectedRest");
        }
        
        public void ModifyMyHandCurrentCardList()
        {
            ttMan.ttDb.modifyCurrentHandListWhenCardIsPlayed();
        }


        
        
    }

}