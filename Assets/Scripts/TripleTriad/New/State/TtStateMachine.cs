using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR;

namespace ETF
{
    public class TtStateMachine : MonoBehaviour
    {

        [SerializeField] TtState _currentState;
        [SerializeField] TtState _previousState;


        public void RunCurrentState()
        {

            _currentState.Execute();

        }

        public void ChangeState(TtState stateToChangeTo)
        {
            if (_currentState != null)
            {
                _currentState.End();
                _previousState = _currentState;
            }

            _currentState = stateToChangeTo;
            _currentState.Startup();
        }

        public void ChangeStateToPreviousState()
        {
            _currentState.End();
            _currentState = _previousState;
            _currentState.Startup();
        }
    }
}
