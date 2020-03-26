using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class AreaExit : MonoBehaviour
{

    public string areaToLoad; //adds a place to type in area to load

    public string areaTransitionName; //adds a place to type in a transition name

    public AreaEntrance theEntrance;

    public float waitToLoad = 1f; //this will make the variable to be set to 1 by default, we can still change it
    private bool shouldLoadAfterFade;

    [SerializeField] bool ShouldDisplayTrnasition = false;


    void Start()
    {
        theEntrance.transitionName = areaTransitionName; // when the scene starts, we will change theentrance string to the area transition name
    }

    void Update()
    {
        if (shouldLoadAfterFade)
        {
            waitToLoad -= Time.deltaTime; // makes it so that it will subtract the loading time from the wait to load time.  
            if (waitToLoad <= 0)
            {
                shouldLoadAfterFade = false;
                SceneManager.LoadScene(areaToLoad);
                GameMenus.instance.DisplayTransitionBanner();
            }
        }
        
    }

    private void OnTriggerEnter2D(Collider2D other)
    {
        if (other.tag == "Player")
        {
            //SceneManager.LoadScene(areaToLoad);
            shouldLoadAfterFade = true;
            GameManager.instance.fadingBetweenAreas = true; // stops character movement by changing this value in gamemanager to true, changed to false in the entrance script
            DisplayTransitionBannerSwitch();
            UIFade.instance.FadeToBlack(); //this pulls the instance of uifadetoblack and do what it is supposed to do regularly
            //GameMenus.instance.DisplayTransitionBanner();
            PlayerController.instance.areaTransitionName = areaTransitionName; // sets the instance of player controller area transition name, to the current area transition name
            //these scripts are loaded on the exit and entrance scripts
        }


    }

    private void DisplayTransitionBannerSwitch()
    {
        if (ShouldDisplayTrnasition == true)
        {
            GameMenus.instance.shouldDisplayTransition = true;
        }
        else if (ShouldDisplayTrnasition == false)
        {
            GameMenus.instance.shouldDisplayTransition = false;
        }
        else
        {
            Debug.Log("Something strange happened");
        }
    }
}
