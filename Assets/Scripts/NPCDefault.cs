using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NPCDefault : MonoBehaviour
{

    [SerializeField] bool menuOpener;
    [SerializeField] bool tripleTriad;

    [SerializeField] Animator myAnim;

    public string sceneToLoad;

    // Start is called before the first frame update
    void Start()
    {
        //myAnim.Play();
    }

    // Update is called once per frame
    void Update()
    {
        /*
        if (tripleTriad == true && Input.GetKeyDown(KeyCode.X) && GameManager.instance.canActivate == true)
        {

            SoundManager.instance.StopBGM();
            GameManager.instance.ttCanvas.SetActive(true);
            TTManager.instance.startGame = true;

        }
        */

    }

    private void OnTriggerEnter2D(Collider2D other)

    {
        if (menuOpener == true)
        {

            StartCoroutine(UIFade.instance.MenuOpenerFade());
            GameMenus.instance.GrabCharacterStatsReference();
            /*
            PlayerController.instance.canMove = false;
            InventoryManager.instance.initializingFields = true;
            InventoryManager.instance.inventoryState = InventoryManager.InventoryStates.OPTIONS_PANEL;
            */
           // Debug.Log("I just entered the collider of the guy");
        }

        GameManager.instance.canActivate = true;


    }

    private void OnTriggerExit2D(Collider2D other)
    {
        GameManager.instance.canActivate = false;
    }
}
