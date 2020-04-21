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

        public override void Startup(int additionalArgs = 0)
        {
            StartCoroutine(ttMan.ttUi.InitializeEnemyHandSelectScreenUiCo());
            ttMan.ttDb.GenerateEnemyCardHand();
        }
        
    }
}
