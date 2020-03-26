using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class LoadingScreen : MonoBehaviour
{
    [SerializeField] string[] loadingText = null;
    [SerializeField] GameObject loadingTextGO = null;
    [SerializeField] float wait = 1;
    [SerializeField] string endGameText = null;
    [SerializeField] GameObject yoboi = null;
    private CanvasGroup yobois;
    [SerializeField] Slider wolf3DSlider;

    [Range(0f,0.1f)]
    [SerializeField] float loadSliderSpeed;

    // Start is called before the first frame update
    void Start()
    {
        yobois = yoboi.GetComponent<CanvasGroup>();
        StartCoroutine(FadeIn(yobois));
        StartCoroutine(LoadingText());
        StartCoroutine(loadSliderAnimate());
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    IEnumerator LoadingText()
    {
        Text loadtext = loadingTextGO.GetComponent<Text>();
        for(int i = 0; i < loadingText.Length; i++)
        {
            loadtext.text = loadingText[i];
            yield return new WaitForSeconds(wait);
        }
        loadtext.text = loadingText[5];
        yield return null;
        StartCoroutine(FadeOut(yobois));
        loadtext.text = endGameText;

    }

    IEnumerator FadeIn(CanvasGroup canvasGroupie)
    {
        canvasGroupie.alpha = 0;
        while (canvasGroupie.alpha < 1)
        {
            canvasGroupie.alpha += Time.deltaTime;
            yield return null;
        }
        yield return null;
    }

    IEnumerator FadeOut(CanvasGroup canvasGroupie)
    {
        UIFade.instance.FadeToBlack();
        yield return new WaitForSeconds(2);
        SceneLoader.instance.sceneToLoad = "DebugRoom";
        SceneLoader.instance.LoadScene();
        GameManager.instance.shouldFade = true;
       
    }

    private IEnumerator loadSliderAnimate()
    {
        wolf3DSlider.value = 0;
        bool hasPlayedChestSound = false;
        bool hasPlayedCrossSound = false;
        bool hasPlayedBingSound = false;
        while(wolf3DSlider.value <1)
        {
            if(!hasPlayedBingSound && wolf3DSlider.value > 0.2f){ SoundManager.instance.PlaySFX(13);hasPlayedBingSound = true; }
            if (!hasPlayedCrossSound && wolf3DSlider.value > 0.9f) { SoundManager.instance.PlaySFX(14); hasPlayedCrossSound = true; }
            //if (!hasPlayedChestSound && wolf3DSlider.value > 0.8f) { SoundManager.instance.PlaySFX(15); hasPlayedChestSound = true; }
            wolf3DSlider.value = wolf3DSlider.value = wolf3DSlider.value + 0.03f + Random.Range(0.0f, 0.03f);
            yield return new WaitForSeconds(loadSliderSpeed);
        }


    }

}
