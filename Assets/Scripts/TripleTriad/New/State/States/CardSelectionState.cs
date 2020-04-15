using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{
    public class CardSelectionState : TtState
    {
        #region configuration variables

        //reference for used values
        private int _numberToIncrement;
        private bool _regularAutoScrollSpeed;
        [SerializeField] TripleTriadManager ttMan;

        //reference to the gamestate that we were coming from
        private TripleTriadManager.TripleTriadGameStates _gameStateComingFrom;

        //reference to the running coroutine
        Coroutine _continuousCoroutineReference;


        //enums to use for methods
        public enum PageScrollDirections
        {
            GoingForward,
            GoingBackward,
        }

        private enum ModifyingCardQuantity
        {
            IsAddingCard,
            IsRemovingCard,
        }

        public enum WhichUpDownDirection
        {
            IsMovingDown,
            IsMovingUp,
        }
        // private enum WhichWayToLoop
        // {
        //     LoopToTop,
        //     LoopToBottom,
        // }

        #endregion

        public override void Startup(int additionalArgs = 0)
        {
            if (_gameStateComingFrom != TripleTriadManager.TripleTriadGameStates.ChoosingCards)
            {
                InitializeDbForCardSelection();

                InitializeUiForCardSelection();
            }
            else if (_gameStateComingFrom == TripleTriadManager.TripleTriadGameStates.ChoosingCards)
            {
                ttMan.ttUi.TurnOnCardSelectionScreenUiElements();
                CancelLastSelection();
            }
        }
        
        public override void Execute()
        {
            ListenForUserInputs();
            ttMan.ttUi.KeepFingerOnProperLocationInCardSelection();
        }

        public override void End()
        {
            _gameStateComingFrom = TripleTriadManager.TripleTriadGameStates.ChoosingCards;
            StopCoroutine(_continuousCoroutineReference);
            ttMan.ttDb.UpdateMyHandTripleTriadCardsWithMySelectionList();
        }

        #region Functions


        private void ListenForUserInputs()
        {
            if (Input.GetKeyDown(KeyCode.D) || (Input.GetButtonDown("right")))
            {
                if (_continuousCoroutineReference != null)
                {
                    StopTheCoroutineScrolling();
                }

                _continuousCoroutineReference =
                    StartCoroutine(PageScrollContinuouslyCo(PageScrollDirections.GoingForward));
            }
            else if (Input.GetKeyDown(KeyCode.A) || (Input.GetButtonDown("left")))
            {
                if (_continuousCoroutineReference != null)
                {
                    StopTheCoroutineScrolling();
                }

                _continuousCoroutineReference =
                    StartCoroutine(PageScrollContinuouslyCo(PageScrollDirections.GoingBackward));
            }
            else if (Input.GetKeyDown(KeyCode.W) || (Input.GetButtonDown("up")))
            {
                if (_continuousCoroutineReference != null)
                {
                    StopTheCoroutineScrolling();
                }

                _continuousCoroutineReference =
                    StartCoroutine(ScrollContinuously(WhichUpDownDirection.IsMovingUp));
            }
            else if (Input.GetKeyDown(KeyCode.S) || (Input.GetButtonDown("down")))
            {
                if (_continuousCoroutineReference != null)
                {
                    StopTheCoroutineScrolling();
                }

                _continuousCoroutineReference = StartCoroutine(ScrollContinuously(WhichUpDownDirection.IsMovingDown));
            }
            else if (Input.GetKeyDown(KeyCode.Space) || (Input.GetButtonDown("Fire1")))
            {
                CardSelection();
            }
            else if (Input.GetKeyDown(KeyCode.C) || (Input.GetButtonDown("Fire2")))
            {
                CancelLastSelection();
            }

            if (Input.GetKeyUp(KeyCode.D) || (Input.GetButtonUp("right")))
            {
                if (_continuousCoroutineReference != null)
                {
                    StopTheCoroutineScrolling();
                }
            }
            else if (Input.GetKeyUp(KeyCode.A) || (Input.GetButtonUp("left")))
            {
                if (_continuousCoroutineReference != null)
                {
                    StopTheCoroutineScrolling();
                }
            }
            else if (Input.GetKeyUp(KeyCode.W) || (Input.GetButtonUp("up")))
            {
                if (_continuousCoroutineReference != null)
                {
                    StopTheCoroutineScrolling();
                }
            }
            else if (Input.GetKeyUp(KeyCode.S) || (Input.GetButtonUp("down")))
            {
                if (_continuousCoroutineReference != null)
                {
                    StopTheCoroutineScrolling();
                }
            }
        }

        private void ChangePage(PageScrollDirections whichWayToScroll)
        {
            //uses ttlogic to figure out if you can page switch, and if you can then move the cursor and start the animation and sound
            //bool canPageSwitch;
            switch (whichWayToScroll)
            {
                case PageScrollDirections.GoingForward:

                    if (ttMan.ttLogic.CanISwitchPages(whichWayToScroll))
                    {
                        StartCoroutine(ttMan.ttUi.PlayCardSelectionPageScrollAnimation(whichWayToScroll));
                        MoveCursorToNextPageInUiAndDb(whichWayToScroll);
                        SoundManager.instance.PlaySFX(6);
                    }

                    break;
                case PageScrollDirections.GoingBackward:
                    if (ttMan.ttLogic.CanISwitchPages(whichWayToScroll))
                    {
                        StartCoroutine(ttMan.ttUi.PlayCardSelectionPageScrollAnimation(whichWayToScroll));
                        MoveCursorToNextPageInUiAndDb(whichWayToScroll);
                        SoundManager.instance.PlaySFX(6);
                    }

                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(whichWayToScroll), whichWayToScroll, null);
            }
        }

        private void MoveCursorUpDownInMenu(WhichUpDownDirection whichWayToScroll)
        {
            if (!ttMan.ttLogic.CanIScrollOnCardSelect(whichWayToScroll)) return;
            if (ttMan.ttLogic.AreYouGoingToLoopInCardSelection(whichWayToScroll))
            {
                LoopInCurrentMenu(whichWayToScroll);
            }
            else
            {
                MoveCursorOneSpotInInventory(whichWayToScroll);
            }

            SoundManager.instance.PlaySFX(0);
            ttMan.ttUi.UpdateTheBigCardDisplayUi();
        }

        private void MoveCursorOneSpotInInventory(WhichUpDownDirection whichWayAreYouGoing)
        {
            ttMan.ttDb.UpdateCardSelectionFingerPositionForScrollingInDb(whichWayAreYouGoing);
        }

        private void LoopInCurrentMenu(WhichUpDownDirection whichUpDown)
        {
            ttMan.ttDb.UpdateCardSelectionFingerPositionForLoopingInDb(whichUpDown);
        }

        private void MoveCursorToNextPageInUiAndDb(PageScrollDirections scrollDirection)
        {
            ttMan.ttDb.UpdateCardSelectionFingerPositionForPageScrollingInDb(scrollDirection);
            ttMan.ttUi.UpdatePageNum();
            if (ttMan.ttLogic.DidYouGoPastTheCardSelectionList())
            {
                BounceCursorUpOnPageMove();
            }
            else
            {

                ttMan.ttUi.UpdateTheBigCardDisplayUi();

            }
        }

        private void BounceCursorUpOnPageMove()
        {
            ttMan.ttDb.UpdateCardSelectionInfoOnScrollingPastEndOfPage();
            ttMan.ttUi.UpdateTheBigCardDisplayUi();
        }


        private IEnumerator ScrollContinuously(WhichUpDownDirection scrollDirection)

        {
            _numberToIncrement = 0;
            _regularAutoScrollSpeed = true;
            while (true)
            {
                if (_regularAutoScrollSpeed)
                {
                    MoveCursorUpDownInMenu(scrollDirection);
                    _numberToIncrement++;
                    if (_numberToIncrement >= 2)
                    {
                        _regularAutoScrollSpeed = false;
                    }

                    yield return new WaitForSeconds(ttMan.ttUi.RetrieveRegularAutoScrollSpeed());

                }
                else if (!_regularAutoScrollSpeed)
                {
                    MoveCursorUpDownInMenu(scrollDirection);
                    yield return new WaitForSeconds(ttMan.ttUi.RetrieveFastAutoScrollSpeed());

                }
            }
            // ReSharper disable once IteratorNeverReturns
        }

        private IEnumerator PageScrollContinuouslyCo(PageScrollDirections whichWayToScroll)
        {
            while (true)
            {
                ChangePage(whichWayToScroll);
                yield return new WaitForSeconds(ttMan.ttUi.RetrieveRegularAutoScrollSpeed() * .75f);

            }
            // ReSharper disable once IteratorNeverReturns
        }

        private void CardSelection()
        {
            if (!ttMan.ttLogic.CanSelectCardInCardSelection(ttMan.ttDb.RetrieveCardSelectionCurrentSpotInInventory()))
            {
                SoundManager.instance.PlaySFX(3);
                return;
            }

            ttMan.ttUi.UpdateMyHandImage();
            ttMan.ttUi.PlayMyHandAnimation(ttMan.ttDb.currentHandSelectionsList.Count);
            AddChoiceToCardSelectionList();
            ModifyCardQuantity(ModifyingCardQuantity.IsAddingCard);
            SoundManager.instance.PlaySFX(6);
            if (ttMan.ttDb.currentHandSelectionsList.Count == 5)
            {
                ttMan.SendStateChange(ttMan.cardConfirmationState);
            }
        }

        private void ModifyCardQuantity(ModifyingCardQuantity whatToDoWithQuantity)
        {
            //used to modify the card select quantity in screen and in the db
            switch (whatToDoWithQuantity)
            {
                case ModifyingCardQuantity.IsAddingCard:
                    ttMan.ttDb.currentBattleQuantityForCards[
                        ttMan.ttDb.RetrieveCardSelectionCurrentSpotInInventory()]--;
                    if (ttMan.ttDb.currentBattleQuantityForCards[
                            ttMan.ttDb.RetrieveCardSelectionCurrentSpotInInventory()] >
                        0)
                    {
                        ttMan.ttUi.UpdateQuantityAfterSelecting();
                    }
                    else
                    {
                        ttMan.ttUi.UpdateColorOfCardSelectionText();
                        ttMan.ttUi.UpdateQuantityAfterSelecting();
                    }

                    break;
                case ModifyingCardQuantity.IsRemovingCard:
                    var spotInSelectionsListToCheck = ttMan.ttDb.currentHandSelectionsList.Count - 1;
                    ttMan.ttDb.currentBattleQuantityForCards[
                        ttMan.ttDb.currentHandSelectionsList[spotInSelectionsListToCheck].spotInCardInv]++;
                    ttMan.ttUi.UpdateQuantityAfterRemovingFromSelection();
                    ttMan.ttUi.UpdateColorOfCardSelectionWhenRemovedText();
                    break;
            }
        }

        private void AddChoiceToCardSelectionList()
        {
            //used when you press select
            var cardChoice = new LastItemChosen(ttMan.ttDb.RetrieveCardSelectionCurrentPageNumber(),
                ttMan.ttDb.RetrieveCardSelectionFingerLocationOnCurrentPage(),
                ttMan.ttDb.RetrieveCardSelectionCurrentSpotInInventory());
            ttMan.ttDb.currentHandSelectionsList.Add(cardChoice);
        }

        private void CancelLastSelection()
        {
            //used when you press cancel button
            if (!ttMan.ttLogic.CanRemoveCardFromCardSelection()) return;
            ModifyCardQuantity(ModifyingCardQuantity.IsRemovingCard);
            ttMan.ttUi.PlayRestAnimation(ttMan.ttDb.currentHandSelectionsList.Count - 1);
            ttMan.ttDb.currentHandSelectionsList.RemoveAt(ttMan.ttDb.currentHandSelectionsList.Count - 1);
            SoundManager.instance.PlaySFX(1);
        }

        private void StopTheCoroutineScrolling()
        {
            StopCoroutine(_continuousCoroutineReference);
        }


        private void InitializeDbForCardSelection()
        {
            ttMan.ttDb.ClearBattleSelectionsList();
            ttMan.ttDb.BringInUsableBattleCards();
            ttMan.ttDb.InitializeCardSelectionValuesInDB();
        }

        private void InitializeUiForCardSelection()
        {
            ttMan.ttUi.TurnOnCardSelectionScreenUiElements();
            ttMan.ttUi.ResetScrollRectPosition();
            ttMan.ttUi.GeneratePages();
            ttMan.ttUi.InitializeUiFromTtBattleList();
            ttMan.ttUi.UpdatePageNum();
            ttMan.ttUi.UpdateTheBigCardDisplayUi();
            ttMan.ttUi.isLoading = false;
        }

        #endregion

    }
}
