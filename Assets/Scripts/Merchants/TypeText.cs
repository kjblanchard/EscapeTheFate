using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class TypeText : MonoBehaviour
{
    [SerializeField] public TMP_Text dialogText;
    [SerializeField] private Color32 textColor;
    [SerializeField] private bool textSound;
    [SerializeField] private float textSpeed;
    [SerializeField] bool canCancelTyping;
    private int counter;
    public bool closingMenu;

    // Start is called before the first frame update
    void Start()
    {
        StartCoroutine(TypewriterText());
    }

    // Update is called once per frame
    void Update()
    {

    }

    public IEnumerator TypewriterText()
    {
        counter = 0;
        canCancelTyping = false;
        dialogText.alpha = 0;

        //start the test
        TMP_TextInfo textInfo = dialogText.textInfo;
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
                if (textSound)
                {
                    SoundManager.instance.PlaySFX(7);
                }

                // New function which pushes (all) updated vertex data to the appropriate meshes when using either the Mesh Renderer or CanvasRenderer.
                dialogText.UpdateVertexData(TMP_VertexDataUpdateFlags.Colors32);

                // This last process could be done to only update the vertex data that has changed as opposed to all of the vertex data but it would require extra steps and knowing what type of renderer is used.
                // These extra steps would be a performance optimization but it is unlikely that such optimization will be necessary.
                if (Input.GetKeyDown(KeyCode.Space) && canCancelTyping)
                {
                    currentCharacter = dialogText.textInfo.characterCount;
                    dialogText.alpha = 1;
                    StopAllCoroutines();
                    ShopManager.instance.blinkingFinger.SetActive(true);
                    GameManager.instance.canMoveCursor = true;

                }

                counter++;
                if (counter >= 3)
                {
                    canCancelTyping = true;
                }
            }

            currentCharacter++;

            yield return new WaitForSeconds(textSpeed);
            if (currentCharacter == dialogText.textInfo.characterCount &&!closingMenu)
            {
                ShopManager.instance.blinkingFinger.SetActive(true);
                GameManager.instance.canMoveCursor = true;
            }
            
            
        }
    }

    public void ChangeTextGoodbye()
    {
        closingMenu = true;
        dialogText.text = ShopUI.instance.variableShopExitText;
        StartCoroutine(TypewriterText());
    }

}
