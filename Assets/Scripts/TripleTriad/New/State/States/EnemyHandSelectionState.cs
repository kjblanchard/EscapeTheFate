using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{
    public class EnemyHandSelectionState : TtState
    {
        #region Configuration

        [SerializeField] TripleTriadManager ttMan;

        #endregion

        public override void Startup()
        {
            StartCoroutine(ttMan.ttUi.InitializeEnemyHandSelectScreenUi());
        }

        public override void Execute()
        {
            ttMan.ttDb.GenerateEnemyCardHand();
        }

        public override void End()
        {
            base.End();
        }
    }
}
