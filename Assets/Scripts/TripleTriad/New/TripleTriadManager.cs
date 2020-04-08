using System;
using UnityEngine;

namespace ETF.TripleTriad
{
    public class TripleTriadManager : MonoBehaviour
    {

        #region Configuration

        public enum TripleTriadGameStates
        {
            Disabled,
            ShowingRules, //SHOWING RULES NEEDS TO RESET CARD SELECTION COMING FROM TO SHOWING RULES WHEN IT ENDS for his startup to work right
            ChoosingCards,
            ConfirmingCards,
            ChooseEnemyCards,
            TurnSelection,
            PlayerCardSelection,
            LocationSelection,
            EnemySelection,
            EnemyLocationSelection,
            End,
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
        public EnemyTurnState enemyTurnState;
        public FingerAnimationChanger fingerAnimationChanger;

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
            ttDb.InitializeDbValuesForStartingTripleTriad();
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
            GC.Collect();
            ttStateMachine.ChangeStateToPreviousState();
        }

        public TtState RetrieveCurrentState()
        {
            return ttStateMachine.ReturnCurrentState();
        }

        public void SwitchToEnemyTurn()
        {
            fingerAnimationChanger.ChangeTurn();
        }

        #endregion



    }
}
