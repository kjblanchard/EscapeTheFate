using System.Collections;
using System.Collections.Generic;
using ETF.TripleTriad;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class DialogManager : MonoBehaviour
{
    public static DialogManager instance;

    [Header("Canvas Panels")] public GameObject dialogBoxGameObject;
    [SerializeField] GameObject nameTextGameObject;
    [SerializeField] GameObject charImageGameObject;
    //[SerializeField] CanvasGroup dialogTextCanvasGroup;
    [SerializeField] Text nameText;
    [SerializeField] Image charImage;
    [SerializeField] TMP_Text dialogText;


    public List<string> linesOfDialog = new List<string>();
    public int placeInDialog;

    //public bool displayingDialog;
    public bool justStartedDialog;
    //public bool showNameBox;
    public bool typingText;

    private Sprite npcImage;
    private string npcName;

    public Animator animator;

    [Header("Saved Sprites")] [SerializeField]
    Sprite kevPic;

    [SerializeField] Sprite coryPic;
    [SerializeField] Sprite toddPic;

    [Header("Choices")] public GameObject[] choiceFingerLocations;

    public Text[] choiceText;
    [SerializeField] GameObject fingerGameObject;
    public GameObject choiceBoxGameObject;
    [SerializeField] private int currentChoice;
    [SerializeField] private bool choiceBoxOpen = false;

    [Header("Options")] 
    [SerializeField] private float textSpeed;

    [SerializeField] private bool textSound;
    [SerializeField] Color32 textColor;

    private void Awake()
    {
        if (instance == null)
        {
            instance = this;
        }
        else
        {
            if (instance != this)
            {
                Destroy(gameObject);
            }
        }

        DontDestroyOnLoad(gameObject);
    }

    private void Update()
    {
        if (dialogBoxGameObject.activeInHierarchy)
        {
            if ((Input.GetKeyUp(KeyCode.Space)) || (Input.GetButtonUp("Fire1")) && !GameManager.instance.dialogCloseDelay) // added the dialog close delay to create a timeout for after dialog
            {
                if (!justStartedDialog && !choiceBoxOpen)
                {
                    if (placeInDialog <= linesOfDialog.Count - 1)
                    {
                        placeInDialog++;
                    }
                    
                    if (placeInDialog >= linesOfDialog.Count && !GameManager.instance.makingDecision && !GameManager.instance.dialogCloseDelay)
                    {
                        CloseDialogWindow();
                        if (GameManager.instance.ttActive)
                        {
                            //TurnOffPanel();
                            dialogBoxGameObject.SetActive(false);
                            //SoundManager.instance.StopBGM();
                            //TripleTriadManager.instance.ttDb.InitializeDbValuesForStartingTripleTriad();
                            //TripleTriadManager.instance.SendStateChange(TripleTriadManager.instance.showingRulesState);

                            StartCoroutine(UIFade.instance.StartCardBattleFade(0.5f));
                            //StartCoroutine(Oneframe());
                        }
                    }
                    else
                    {
                        NameChanger();
                        StopAllCoroutines();
                        StartCoroutine(TypewriterText(linesOfDialog[placeInDialog]));
                    }
                }
                else
                {
                    justStartedDialog = false;
                }
            }

            else if ((Input.GetKeyDown(KeyCode.Space) || Input.GetButtonDown("Fire1")) && typingText &&
                     justStartedDialog == false)
            {
                typingText = false;
                StopAllCoroutines();
                dialogText.text = linesOfDialog[placeInDialog];
                dialogText.alpha = 255;
                dialogText.color = textColor;
                justStartedDialog = true;

                if (GameManager.instance.makingDecision)
                {
                    StartCoroutine(ActivateChoiceBox());
                }

            }
        }

        if (GameManager.instance.makingDecision && choiceBoxGameObject.activeInHierarchy)
        {
            if (Input.GetKeyDown(KeyCode.S) && currentChoice < choiceFingerLocations.Length -1)
            {
                currentChoice++;
                fingerGameObject.transform.position = choiceFingerLocations[currentChoice].transform.position;
            }
            else if (Input.GetKeyDown(KeyCode.W) && currentChoice > 0)
            {
                
                currentChoice--;
                fingerGameObject.transform.position = choiceFingerLocations[currentChoice].transform.position;
            }
            else if (Input.GetKeyDown(KeyCode.Space) && choiceBoxOpen)
            {
                GameManager.instance.makingDecision = false;
              //CloseDialogWindow();
              //StartCoroutine(TurnOffPanel());
              StartCoroutine(UIFade.instance.ShopOpenerFade());

              
            }
        }
    }

    public void ShowDialog(Dialog dialogs, bool isPerson, bool showNPCPic, Sprite npcPic, string name)
    {
        if (GameManager.instance.mapTransitionTextDisplaying)
        {//clears the map transition text
            GameMenus.instance.mapScreenBox.SetActive(false);
            GameManager.instance.mapTransitionTextDisplaying = false;
        }

        dialogText.alpha = 0;
        typingText = true;
        linesOfDialog.Clear();
        dialogBoxGameObject.SetActive(true);

        for (int i = 0; i < dialogs.dialogLines.Length; i++)
        {
            linesOfDialog.Add(dialogs.dialogLines[i]);
        }

        placeInDialog = 0;
        npcName = name;
        justStartedDialog = true;
        GameManager.instance.dialogActive = true;
        PlayerController.instance.canMove = false;
        if (showNPCPic)
        {
            charImageGameObject.SetActive(true);
            npcImage = npcPic;
        }
        else if (!showNPCPic)
        {
            charImageGameObject.SetActive(false);
        }

        if (isPerson)
        {
            nameTextGameObject.SetActive(true);
        }
        else if (!isPerson)
        {
            nameTextGameObject.SetActive(false);
        }


        NameChanger();
        animator.SetBool("isOpen", true);
        StartCoroutine(QuickDelay());
    }
    
    void NameChanger()
    {
        if (linesOfDialog[placeInDialog].StartsWith("k-"))
        {
            nameText.text = "Kevin :";
            charImage.sprite = kevPic;
            placeInDialog++;
        }
        else if (linesOfDialog[placeInDialog].StartsWith("c-"))
        {
            nameText.text = "Cory :";
            charImage.sprite = coryPic;
            placeInDialog++;
        }
        else if (linesOfDialog[placeInDialog].StartsWith("t-"))
        {
            nameText.text = "Todd :";
            charImage.sprite = toddPic;
            placeInDialog++;
        }
        else if (linesOfDialog[placeInDialog].StartsWith("npc-"))
        {
            nameText.text = npcName;
            charImage.sprite = npcImage;
            placeInDialog++;
        }
    }

    IEnumerator TypewriterText(string lineToType)
    {
        typingText = true;
        dialogText.text = lineToType;
        dialogText.alpha = 0;
        
        //start the test
        TMP_TextInfo textInfo =  dialogText.textInfo;
        int currentCharacter = 0;

        Color32[] newVertexColors;
        Color32 c0 = dialogText.color;
        dialogText.ForceMeshUpdate();
        
         while (currentCharacter < dialogText.textInfo.characterCount)
            {
                int characterCount = textInfo.characterCount;

                // If No Characters then just yield and wait for some text to be added
                if (characterCount == 0)
                {
                    yield return new WaitForSeconds(0.25f);
                    continue;
                }

                // Get the index of the material used by the current character.
                int materialIndex = textInfo.characterInfo[currentCharacter].materialReferenceIndex;

                // Get the vertex colors of the mesh used by this text element (character or sprite).
                newVertexColors = textInfo.meshInfo[materialIndex].colors32;

                // Get the index of the first vertex used by this text element.
                int vertexIndex = textInfo.characterInfo[currentCharacter].vertexIndex;

                // Only change the vertex color if the text element is visible.
                if (textInfo.characterInfo[currentCharacter].isVisible)
                {

                    newVertexColors[vertexIndex + 0] = textColor;
                    newVertexColors[vertexIndex + 1] = textColor;
                    newVertexColors[vertexIndex + 2] = textColor;
                    newVertexColors[vertexIndex + 3] = textColor;
                    if(textSound){SoundManager.instance.PlaySFX(7);}

                    // New function which pushes (all) updated vertex data to the appropriate meshes when using either the Mesh Renderer or CanvasRenderer.
                    dialogText.UpdateVertexData(TMP_VertexDataUpdateFlags.Colors32);

                    // This last process could be done to only update the vertex data that has changed as opposed to all of the vertex data but it would require extra steps and knowing what type of renderer is used.
                    // These extra steps would be a performance optimization but it is unlikely that such optimization will be necessary.
                }

                currentCharacter++;

                yield return new WaitForSeconds(textSpeed);
            }

         justStartedDialog = false;
         typingText = false;
         if (GameManager.instance.makingDecision)
         {
             StartCoroutine(ActivateChoiceBox());
         }
         
    }

    private IEnumerator TurnOffPanel()
    {
        yield return new WaitForSeconds(0.2f);
        dialogBoxGameObject.SetActive(false);
        GameManager.instance.dialogActive = false;
    }

    private IEnumerator QuickDelay()
    {
        animator.SetBool("isOpen", true);
//        Debug.Log("just did the delay");
        yield return new WaitForSeconds(0.25f);
        StartCoroutine(TypewriterText(linesOfDialog[placeInDialog]));
//        Debug.Log("just started the typewriter");
    }

    private IEnumerator Oneframe()
    {
        SoundManager.instance.PlayIntroLoop(1);
        yield return new WaitForSeconds(0.5f);
        StartCoroutine(TTfade.instance.StartCardBattleFade(0.5f));
    }

    private IEnumerator ActivateChoiceBox()
    {
        if ( GameManager.instance.makingDecision)
        {
            
            choiceBoxGameObject.SetActive(true);
            fingerGameObject.transform.position = choiceFingerLocations[currentChoice].transform.position;
            fingerGameObject.SetActive(true);
            yield return new WaitForSeconds(0.25f);
            choiceBoxOpen = true;
        }
    }

    public void CloseDialogWindow()
    {
        
        dialogText.alpha = 0;
        animator.SetBool("isOpen", false);
        StopAllCoroutines();
        StartCoroutine(DialogDelayTimer());
        StartCoroutine(TurnOffPanel());
        PlayerController.instance.canMove = true;
        GameManager.instance.makingDecision = false;
        choiceBoxOpen = false;
        currentChoice = 0;
    }

    public void ShopCloseDialogWindow()
    {
        animator.SetBool("isOpen", false);
        StartCoroutine(TurnOffPanel());
        GameManager.instance.makingDecision = false;
    }
    private IEnumerator DialogDelayTimer()
    {
       // Debug.Log("started the delay");
        GameManager.instance.dialogCloseDelay = true;
        yield return new WaitForSeconds(0.4f);

        GameManager.instance.dialogCloseDelay = false;
        //Debug.Log("ended the delay");
        // yield return null;
    }


}