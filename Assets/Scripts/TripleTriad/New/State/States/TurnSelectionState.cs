using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF
{
    public class TurnSelectionState : TtState
    {
        #region Configuration

        [SerializeField] TripleTriadManager ttMan;

        #endregion

        public override void Startup()
        {
            ttMan.ttUi.InitializeTurnSelection();
            ttMan.ttDb.SetWhoGetsToGoFirst(ttMan.ttLogic.TurnSelector());
            ttMan.ttUi.switchToFingerTurnSelectionAnimation();
        }

        public override void Execute()
        {

            //this is ended from an animation event at the end of the finger move
        }

        public override void End()
        {
            base.End();
        }


    }
}
