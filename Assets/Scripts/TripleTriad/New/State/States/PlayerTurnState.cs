using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerTurnState : TtState
{
    #region Configuration

    [SerializeField] private TripleTriadManager ttMan;
    private int _playerTurnCurrentSelection;

    #endregion


    public override void Startup()
    {
        ttMan.ttUi.InitializePlayerTurnCanvasFromTurnSelection();
        InitializePlayerTurnValues();
    }

    public override void Execute()
    {
        if (Input.GetKeyDown(KeyCode.W))
        {
            ttMan.ttUi.setOutFocusOnPlayerCardSelected(_playerTurnCurrentSelection);
            _playerTurnCurrentSelection--;
            SoundManager.instance.PlaySFX(0);
            ttMan.ttUi.setFocusOnPlayerCardSelected(_playerTurnCurrentSelection);
        }
        else if (Input.GetKeyDown(KeyCode.S))
        {
            ttMan.ttUi.setOutFocusOnPlayerCardSelected(_playerTurnCurrentSelection);
            _playerTurnCurrentSelection++;
            SoundManager.instance.PlaySFX(0);
            ttMan.ttUi.setFocusOnPlayerCardSelected(_playerTurnCurrentSelection);
        }
        
        //listen for inputs
        ttMan.ttUi.keepPlayerSelectionFingerOnProperLocation(_playerTurnCurrentSelection);
    }

    public override void End()
    {
        base.End();
    }
    

    public void InitializePlayerTurnValues()
    {
        _playerTurnCurrentSelection = 0;
    }
}
