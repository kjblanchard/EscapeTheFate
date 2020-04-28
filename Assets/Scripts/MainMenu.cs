using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MainMenu : MonoBehaviour
{


    [SerializeField] GameObject titleText = null;

    [Header("Moving Text Stuff")]
    [SerializeField] float speed = 10f;
    private Vector3 ETFend;
    [SerializeField] GameObject partSystem = null;

    [Header("FingerLocations")]
    [SerializeField] GameObject finger = null;
    [SerializeField] GameObject[] fingerLocations = null;
    private int currentFingerLocation = 0;

    [Header("Canvas Groups for Fading")]
    [SerializeField] GameObject buttons = null;
    [SerializeField] GameObject fullCanvas = null;
    private CanvasGroup buttonCanvasGroup;
    private CanvasGroup canvasCanvasGroup;

    [Header("For Determining Fade")]

    private int noInput = 0;
    private Quaternion fingerDefault = new Quaternion(0,0,0,0); 


    // Start is called before the first frame update
    void Start()
    {
        InitializeFields();

        StartCoroutine(FadeIn(buttonCanvasGroup));
        SoundManager.instance.PlayIntroLoop(5);
        GameManager.instance.TurnOnDirectionalJoystick();


        
    }



    // Update is called once per frame
    void Update()
    {

        TitleMover();
        UIcontroller();
        ChangeMainMenu();
        
    }

   private void TitleMover()
    {
        Vector3 PositionA = (titleText.transform.position);
        if (titleText.transform.position.x != ETFend.x)
        {
            titleText.transform.position = Vector3.MoveTowards(PositionA, ETFend, Time.deltaTime * speed);
        }
    }

    IEnumerator FadeIn(CanvasGroup canvasGroup)
    {
        canvasGroup.alpha = 0;
        yield return new WaitForSeconds(2);
       while(canvasGroup.alpha < 1)
        {
            canvasGroup.alpha += Time.deltaTime * 2;
            yield return null;
        }
        StartFinger();
        //StartCoroutine(ChangeMainMenu());
        yield return null;
    }

    IEnumerator FadeOut(CanvasGroup canvasGroup)
    {
        canvasGroup.alpha = 1;
        yield return new WaitForSeconds(2);
        while (canvasGroup.alpha > 0)
        {
            canvasGroup.alpha -= Time.deltaTime * 1.5f;
            yield return null;
        }
        partSystem.SetActive(false);
        yield return new WaitForSeconds(3);
        SceneLoader.instance.Reload();
    }


    public void UIcontroller()
    {
        if ((Input.GetKeyDown(KeyCode.S)) || Input.GetButtonDown("down"))
        {
            if (currentFingerLocation < fingerLocations.Length - 1)
            {
                SoundManager.instance.PlaySFX(0);
                currentFingerLocation++;
                finger.transform.position = fingerLocations[currentFingerLocation].transform.position;
            }
        }

        if ((Input.GetKeyDown(KeyCode.W))|| Input.GetButtonDown("up"))
        {
            if (currentFingerLocation > 0)
            {
                SoundManager.instance.PlaySFX(0);
                currentFingerLocation--;
                finger.transform.position = fingerLocations[currentFingerLocation].transform.position;
                
            }
        }

        if ((Input.GetKeyDown(KeyCode.Space))|| Input.GetButtonDown("Fire1"))
        {
            if(currentFingerLocation == 0)
            {
                SoundManager.instance.PlaySFX(3);
                
            }
            else if(currentFingerLocation == 1)
            {
                SoundManager.instance.PlaySFX(3);
            }
            else if(currentFingerLocation == 2)
            {
                DebugRoom();
                SoundManager.instance.PlaySFX(0);
            }
        }


    }

    private void ChangeMainMenu()
    {
        if (Input.anyKeyDown)
        {
            noInput = 0;
            finger.transform.rotation = fingerDefault;
        }
        else if (noInput >= 2000)
        {
            noInput = 0;
            finger.transform.rotation = fingerDefault;
            StartCoroutine(FadeOut(canvasCanvasGroup));
        }
        else if (noInput == 1000)
        {
            finger.transform.rotation = new Quaternion(finger.transform.rotation.x, finger.transform.rotation.y, finger.transform.rotation.z + 50, finger.transform.rotation.w);
            noInput++;
        }
        else if (noInput == 1200)
        {
            finger.transform.rotation = fingerDefault; //new Quaternion(finger.transform.rotation.x, finger.transform.rotation.y, finger.transform.rotation.z + 50, finger.transform.rotation.w);
            noInput++;
        }
        else if (noInput == 1400)
        {
            finger.transform.rotation = new Quaternion(finger.transform.rotation.x, finger.transform.rotation.y, finger.transform.rotation.z + 50, finger.transform.rotation.w);
            noInput++;
        }
        else if (noInput == 1600)
        {
            finger.transform.rotation = fingerDefault; //new Quaternion(finger.transform.rotation.x, finger.transform.rotation.y, finger.transform.rotation.z + 50, finger.transform.rotation.w);
            noInput++;
        }
        else if (noInput == 1800)
        {
            finger.transform.rotation = new Quaternion(finger.transform.rotation.x, finger.transform.rotation.y, finger.transform.rotation.z + 50, finger.transform.rotation.w);
            noInput++;
        }
        else
        {
            noInput++;
            //Debug.Log("The noinput counter is " + noInput);
        }
        //Debug.Log(noInput);

    }

    private void StartFinger()
    {
        finger.SetActive(true);
        finger.transform.position = fingerLocations[0].transform.position;
        currentFingerLocation = 0;
    }

    private void InitializeFields()
    {
        ETFend = new Vector3(titleText.transform.position.x, titleText.transform.position.y, titleText.transform.position.z);
        Vector3 ETFstart = new Vector3(ETFend.x + 40, ETFend.y, ETFend.z);
        titleText.transform.position = ETFstart;

        buttonCanvasGroup = buttons.GetComponent<CanvasGroup>();
        canvasCanvasGroup = fullCanvas.GetComponent<CanvasGroup>();
    }




    //This is for the Mouse/Touch Functions

    public void NewGame()
    {
        SoundManager.instance.PlaySFX(3);
        //SceneLoader.instance.NewGame();
    }

    public void LoadGame()
    {
        SoundManager.instance.PlaySFX(3);
        //SceneLoader.instance.NewGame();
    }

    public void DebugRoom()
    {
        //SoundManager.instance.StopBGM();
        SceneLoader.instance.NewGame();
    }




    //This is for the finger moving stuffs

    public void MoveFingerToNewGame()
    {
        if (currentFingerLocation != 0)
        {
            SoundManager.instance.PlaySFX(0);
            currentFingerLocation = 0;
            finger.transform.position = fingerLocations[currentFingerLocation].transform.position;
        }
    }

    public void MoveFingerToContinue()
    {
        if (currentFingerLocation != 1)
        {
            SoundManager.instance.PlaySFX(0);
            currentFingerLocation = 1;
            finger.transform.position = fingerLocations[currentFingerLocation].transform.position;
        }
    }

    public void MoveFingerToDebug()
    {
        SoundManager.instance.PlaySFX(0);
        currentFingerLocation = 2;
        finger.transform.position = fingerLocations[currentFingerLocation].transform.position;
    }

    

    
}
