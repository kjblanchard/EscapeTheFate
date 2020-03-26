using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MateriaScreenController : MonoBehaviour
{
    public static MateriaScreenController instance;

    [Header("Panels")]
    //public GameObject materiaScreenMaster;
    public GameObject materiaWeaponPanelMaster;
    public GameObject materiaArmorPanelMaster;
    public GameObject materiaAccPanelMaster;

    [Header("Animators")]
    public Animator weaponAnimator;
    public Animator armorAnimator;
    public Animator accAnimator;

    [SerializeField] float animationDelay = 0.25f;


    private void Awake()
    {
        instance = this;

        int numOfMatiaScreenController = FindObjectsOfType<MateriaScreenController>().Length;
        if(numOfMatiaScreenController > 1)
        {
            Destroy(gameObject);
        }
        else
        {
            DontDestroyOnLoad(gameObject);
        }
    }

    public IEnumerator OpenMateriaScreen(bool shouldOpen)
    {
        if (shouldOpen)
        {


            yield return new WaitForSeconds(animationDelay);
            materiaAccPanelMaster.SetActive(true);
            accAnimator.SetBool("materiaOpen", true);
            SoundManager.instance.PlaySFX(6);
            yield return new WaitForSeconds(animationDelay);
            materiaArmorPanelMaster.SetActive(true);
            armorAnimator.SetBool("materiaOpen", true);
            SoundManager.instance.PlaySFX(6);
            yield return new WaitForSeconds(animationDelay);
            materiaWeaponPanelMaster.SetActive(true);
            weaponAnimator.SetBool("materiaOpen", true);
            SoundManager.instance.PlaySFX(6);
            yield return new WaitForSeconds(animationDelay);
            GameManager.instance.canMoveCursor = true;
        }
        else if (shouldOpen == false)
        {
          
            weaponAnimator.SetBool("materiaOpen", false);
            SoundManager.instance.PlaySFX(6);
            //yield return new WaitForSeconds(animationDelay);
            armorAnimator.SetBool("materiaOpen", false);

            yield return new WaitForSeconds(animationDelay);
            accAnimator.SetBool("materiaOpen", false);
            SoundManager.instance.PlaySFX(6);


            yield return new WaitForSeconds(animationDelay);
            SoundManager.instance.PlaySFX(6);
            materiaWeaponPanelMaster.SetActive(false);
            materiaArmorPanelMaster.SetActive(false);
            materiaAccPanelMaster.SetActive(false);
            //yield return new WaitForSeconds(animationDelay);
            //GameManager.instance.canMoveCursor = true;
        }

    }

}
