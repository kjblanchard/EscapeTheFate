using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TtTurnSelectionProcessor : MonoBehaviour
{
    [Header("ScriptReferences")] 
    [SerializeField] TripleTriadManager ttMan;


    public void TripleTriadTurnSelectionProcessor()
    {
        ttMan.ttDb.SetWhoGetsToGoFirst(ttMan.ttLogic.TurnSelector());
        ttMan.ttUi.switchToFingerTurnSelectionAnimation();
        //this is ended from an animation event at the end of the finger move
    }
    
}
