using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class AreaEntrance : MonoBehaviour
{
    public string transitionName;


    void Start()
    {
        MovePlayer(); //can move your player to the matching area entrance
        UIFade.instance.FadeFromBlack();
        GameManager.instance.fadingBetweenAreas = false; //lets you allowed to move by setting this bool to false in gamemanager
    }



    private void MovePlayer()
    {
        if (transitionName == PlayerController.instance.areaTransitionName) // if OUR transition name is equal to the PLAYERS transition name, then...
        {
            PlayerController.instance.transform.position = transform.position; //find the player, and move him to where this object is
        }
    }



}
