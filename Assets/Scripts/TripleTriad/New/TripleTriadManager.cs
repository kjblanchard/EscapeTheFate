using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{
    public class TripleTriadManager : MonoBehaviour
    {

        #region Configuration

        public enum TripleTriadGameStates
        {
            DISABLED,
            SHOWING_RULES, //SHOWING RULES NEEDS TO RESET CARD SELECTION COMING FROM TO SHOWING RULES WHEN IT ENDS for his startup to work right
            CHOOSING_CARDS,
            CONFIRMING_CARDS,
            CHOOSE_ENEMY_CARDS,
            TURN_SELECTION,
            PLAYER_CARD_SELECTION,
            LOCATION_SELECTION,
            ENEMY_SELECTION,
            ENEMY_LOCATION_SELECTION,
            END,
        }

        [Header("Script References")] public TTUI ttUi;
        public TTDB ttDb;
        public TTLogic ttLogic;
        [SerializeField] TtStateMachine ttStateMachine;

        [Header("States")] public CardSelectionState cardSelectionState;
        public CardConfirmationState cardConfirmationState;
        public EnemyHandSelectionState enemyHandSelectionState;
        public TurnSelectionState turnSelectionState;
        public PlayerTurnState playerTurnState;
        public LocationSelectionState locationSelectionState;

        [Header("Singleton")] public static TripleTriadManager instance;

        #endregion


        private void Awake()
        {
            instance = this;
        }

        private void Start()
        {
            CardInventory.instance
                .CreateUsableBattleCardLists(); //this only needs to be ran once, it generates all of the lists for card usage
            GameManager.instance.TurnOnDirectionalJoystick();
            SendStateChange(cardSelectionState);
        }

        void Update()
        {
            ttStateMachine.RunCurrentState();
        }

        #region Functions

        public void SendStateChange(TtState stateToChangeTo)
        {
            GC.Collect();
            ttStateMachine.ChangeState(stateToChangeTo);
        }

        public void SendChangeToPreviousState()
        {
            ttStateMachine.ChangeStateToPreviousState();
        }

        #endregion



    }
}
