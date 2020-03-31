using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TtPlayerTurn : MonoBehaviour
{
    [SerializeField] TTUI ttUi;
    private int _playerTurnCurrentSelection;

    public void TtPlayerTurnProcessor()
    {

        if (Input.GetKeyDown(KeyCode.W))
        {
            ttUi.setOutFocusOnPlayerCardSelected(_playerTurnCurrentSelection);
            _playerTurnCurrentSelection--;
            SoundManager.instance.PlaySFX(0);
            ttUi.setFocusOnPlayerCardSelected(_playerTurnCurrentSelection);
        }
        else if (Input.GetKeyDown(KeyCode.S))
        {
            ttUi.setOutFocusOnPlayerCardSelected(_playerTurnCurrentSelection);
            _playerTurnCurrentSelection++;
            SoundManager.instance.PlaySFX(0);
            ttUi.setFocusOnPlayerCardSelected(_playerTurnCurrentSelection);
        }
        
        //listen for inputs
        ttUi.keepPlayerSelectionFingerOnProperLocation(_playerTurnCurrentSelection);
    }

    public void InitializePlayerTurnValues()
    {
        _playerTurnCurrentSelection = 0;
    }
}
