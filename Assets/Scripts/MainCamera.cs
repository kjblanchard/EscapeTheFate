using System.Collections;
using System.Collections.Generic;
using Cinemachine;
using UnityEngine;
using UnityEngine.Tilemaps;

public class MainCamera : MonoBehaviour
{

    public bool shouldDisplayTransition;

    [SerializeField] private bool testing = true;
    public Transform target; // We want it to only focus on the "transform" category cause thats all that matters
    // we use target and not player, as we will likely want the camera to focus on different things at different times

    // Start is called before the first frame update, so that there is no blank space

    public Tilemap theMap;
    private Vector3 bottomLeftLimit;
    private Vector3 topRightLimit; //these private vectors will store the furthest that the camera can go in those directions, so the game can know how far to move the camera
    //make sure that you set theMap public to the largest tile in the map

    private float halfHeight;
    private float halfWidth;
    //Only half of our camera is going off the edge, so we just need to move the half

    public int musicToPlay; //what music to play will be set in the camera screen
    private bool musicStarted;

    public string mapDisplayText = "Welcome to Garbage";
    [SerializeField] private CinemachineVirtualCamera followCam;


    void Start()
    {
        if(testing)
        {        followCam.Follow = FindObjectOfType<PlayerController>().transform;}

        //target = PlayerController.instance.transform;
        target = FindObjectOfType<PlayerController>().transform;//search all objects in the scene, and search for the target, we did this due to script order, and the player not instance not being set yet

        halfHeight = Camera.main.orthographicSize;
        halfWidth = halfHeight * Camera.main.aspect;



        bottomLeftLimit = theMap.localBounds.min + new Vector3(halfWidth, halfHeight, 0f); //states to thange the bottom left limit to the lowest boundary number (all left and all down)
        topRightLimit = theMap.localBounds.max + new Vector3(-halfWidth, -halfHeight, 0f); // opposite of above, also adds the half width or subtracts to subtract that from the boundaries

        PlayerController.instance.SetBounds(theMap.localBounds.min, theMap.localBounds.max); //tells the playercontroller to set the bounds to theMaps bounds
        SoundManager.instance.bgmToPlay = musicToPlay;

        if (GameManager.instance.shouldFade)
        {
            GameManager.instance.shouldFade = false;
            UIFade.instance.FadeFromBlack();
        }
        if (GameManager.instance.joystickEnabled)
        {
            GameManager.instance.TurnOnRegularJoystick();
        }

        if (shouldDisplayTransition == true)
        {
           // Debug.Log("shoed the map transition");
            StartCoroutine(GameMenus.instance.ShowMapTransition());
            
        }
    }

    // LateUpdate is called once per frame after the update, change to late update as regular can cause stuttering of camera
    void LateUpdate()
    {

        /* taking this out testing cinemachine
        transform.position = new Vector3(target.position.x, target.position.y, transform.position.z);
        // this will make the object this script is tied to (camera) equal to, the targets x and y, and keep its z value the same
        // we use vector3 as we are setting 3 things, x,y, and z

        transform.position = new Vector3(Mathf.Clamp(transform.position.x, bottomLeftLimit.x, topRightLimit.x), Mathf.Clamp(transform.position.y, bottomLeftLimit.y, topRightLimit.y), transform.position.z);
        //this will keep the camera in bounds, by using the Mathf.clamp looking at the positions of x and y and keeping z the same

    */

        if (!musicStarted)
        {
            musicStarted = true;
            SoundManager.instance.PlayIntroLoop(musicToPlay); //when the music starts playing, set it to be true
        }

        /*
        if (GameManager.instance.joystickEnabled)
        {
            GameManager.instance.TurnOnRegularJoystick();
        }
        */
    }

    public string GetMapTransitionText()
    {
        return mapDisplayText;
    }
}
