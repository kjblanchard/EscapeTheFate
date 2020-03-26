using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TripleTriadManager : MonoBehaviour
{
    public enum TripleTriadGameStates
    {
        DISABLED,
        SHOWING_RULES,
        CHOOSING_CARDS,
        CONFIRMING_CARDS,
        DISPLAY_ENEMY_CARDS,
        TURN_SELECTION,
        CARD_SELECTION,
        LOCATION_SELECTION,
        ENEMY_SELECTION,
        ENEMY_LOCATION_SELECTION,
        END,
    }

    [SerializeField] TripleTriadGameStates tripleTriadCurrentGameState;

    [Header("Script References")]
    [SerializeField] TTUI ttUI;
    [SerializeField] TTDB ttDb;
    public TTCardSelectionProcessor ttCardSelectProcessor;
    public TTConfirmCardProcessor ttConfirmCardProcessor;


    [Header("Script References")]
    public static TripleTriadManager instance;

    private void Awake()
    {
        instance = this;
    }


    private void Start()
    {//so that we start from the proper screen
        ChangeToCardSelectProcessor();
        GameManager.instance.TurnOnDirectionalJoystick();
    }


    void Update()
    {
        switch (tripleTriadCurrentGameState)
        {
            case TripleTriadGameStates.CHOOSING_CARDS:
                ttCardSelectProcessor.TtCardSelectProcessor();
                break;
            case TripleTriadGameStates.CONFIRMING_CARDS:
                ttConfirmCardProcessor.TtConfirmCardProcessor();
                break;
                


        }
    }

    public void ChangeToCardSelectProcessor()
    {
        ttCardSelectProcessor.InitializeBattleCardAlbum();
        ttUI.InitializeCardSelectionScreen();
        tripleTriadCurrentGameState = TripleTriadGameStates.CHOOSING_CARDS;
    }
    public void ChangeToCardConfirmGameState()
    {
        
        tripleTriadCurrentGameState = TripleTriadGameStates.CONFIRMING_CARDS;
        ttConfirmCardProcessor.TurnOnInitializer();
        
    }

    public void ChangeBackToChoosingCards()
    {
        ttCardSelectProcessor.CancelLastSelection();
        ttUI.InitializeCardSelectionScreen();
        tripleTriadCurrentGameState = TripleTriadGameStates.CHOOSING_CARDS;
    }
}
