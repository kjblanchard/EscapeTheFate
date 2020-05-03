using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{
    public abstract class TtState : MonoBehaviour
    {
        protected static TTUI _ttui;
        protected static TripleTriadManager _ttMan;
        protected static TTDB _ttdb;
        protected static bool hasInitiated;

        private void Awake()
        {
            if (!hasInitiated)
            {
                hasInitiated = true;
                _ttui = FindObjectOfType<TTUI>();
                _ttMan = FindObjectOfType<TripleTriadManager>();
                _ttdb = FindObjectOfType<TTDB>();
            }
            
        }

        public virtual void Startup(int additionalArgs = 0)
        {

        }

        public virtual void Execute()
        {

        }

        public virtual void End()
        {

        }
    }
}
