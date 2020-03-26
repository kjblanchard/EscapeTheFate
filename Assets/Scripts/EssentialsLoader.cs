using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EssentialsLoader : MonoBehaviour
{
    public static EssentialsLoader instance;

    [Header("Things to Load")]
    [SerializeField] GameObject sceneManager = null;
    [SerializeField] GameObject gameManager = null;
    [SerializeField] GameObject soundManager = null;
    [SerializeField] GameObject optionsCanvas = null;
    [SerializeField] GameObject ttManager = null;
    [SerializeField] GameObject dialogManager = null;
    [SerializeField] GameObject shopManager = null;

    private void Awake()
    {
        
        int essentialsLoaderCount = FindObjectsOfType<EssentialsLoader>().Length;
        if(essentialsLoaderCount > 1)
        {
            Destroy(gameObject);
        }
        else
        {
            instance = this;
            DontDestroyOnLoad(gameObject);
        }
    }

    private void Start()
    {
        Instantiate(sceneManager);
        Instantiate(gameManager);
        Instantiate(soundManager);
        
        Instantiate(ttManager);
        Instantiate(dialogManager);
        Instantiate(shopManager);
        Instantiate(optionsCanvas);//keep this at the end for the uifade since I put it here like a noob
    }

}
