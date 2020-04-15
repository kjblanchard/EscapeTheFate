using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{
    public abstract class TtState : MonoBehaviour
    {
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
