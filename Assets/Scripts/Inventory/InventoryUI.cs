using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class InventoryUI : MonoBehaviour
{
    public static InventoryUI instance;

    Inventory inventory;

    public List<GameObject> itemHolderList = new List<GameObject>();
    public GameObject itemHolderPrefab;
    public Transform itemSpacer;
    public Text descriptionTextField;

    public CanvasGroup fullMenuCanvasGroup = null;

    [Header("Weapons Holder")]
    public List<GameObject> weaponHolderList = new List<GameObject>();
    public GameObject weaponHolderPrefab;
    public Transform weaponSpacer;
    public Text weaponDescriptionTextField;
    public Text weaponEffectTextField;
    public Text weaponSlotsTextField;

    [Header("Materia Holder")]
    public List<GameObject> materiaHolderList = new List<GameObject>();
    public GameObject materiaHolderPrefab;
    public Transform materiaSpacer;
    //public Text materiaNameTextField;
    //public Text materiaTypeTextField;
    //public Text materiaLevelTextField;
    public GameObject blankMateriaHolderPrefab;



    [Header("Materia Slots Images")]
    public Sprite materiaEmpty;
    public Sprite materiaEmptyLL;
    public Sprite materiaEmptyLR;
    public Sprite materiaMag;
    public Sprite materiaMagLL;
    public Sprite materiaMagLR;
    public Sprite materiaSkill;
    public Sprite materiaSkillLL;
    public Sprite materiaSkillLR;
    public Sprite materiaPers;
    public Sprite materiaPersLL;
    public Sprite materiaPersLR;
    public Sprite materiaSup;
    public Sprite materiaSupLL;
    public Sprite materiaSupLR;
    public Sprite materiaStand;
    public Sprite materiaStandLL;
    public Sprite materiaStandLR;

    // public List<CanvasGroup> materiaCanvasGroups = new List<CanvasGroup>();

    [Header("Currency Updater")]
    [SerializeField] Text currentGilText;
    [SerializeField] Text currentLocationText;
    [SerializeField] Text currentPlaytimeHoursText;
    [SerializeField] Text currentPlaytimeMinutesText;
    public bool timerUpdaterIsRunning = false;



    private void Awake()
    {
        instance = this;
    }

    private void Start()
    {
        inventory = Inventory.instance;
        FillList();
        //FillWeaponList();
    }

    void FillList()
    {
        for (int i = 0; i < inventory.inventoryList.Count; i++)
        {
            GameObject newItemHolder = Instantiate(itemHolderPrefab, itemSpacer, false);
            ItemHolder holderScript = newItemHolder.GetComponent<ItemHolder>();
            holderScript.amountText.text = inventory.inventoryList[i].currentAmount.ToString();
            holderScript.nameText.text = inventory.inventoryList[i].item.name;
            holderScript.typeImage.sprite = inventory.inventoryList[i].item.icon;

            holderScript.typeText.text = inventory.inventoryList[i].item.type.ToString();

            itemHolderList.Add(newItemHolder);
        }
    }

    public void RecreateList()
    {
        foreach(GameObject itemHolder in itemHolderList)
        {
            Destroy(itemHolder);
        }
        itemHolderList.Clear();
        FillList();
    }

    public IEnumerator FadeIn(InventoryManager.InventoryStates newState)
    {
        GameManager.instance.isLoading = true;
        fullMenuCanvasGroup.alpha = 1;
        while (fullMenuCanvasGroup.alpha > 0)
        {
            fullMenuCanvasGroup.alpha -= Time.deltaTime * 5;
            yield return null;
        }
        InventoryManager.instance.inventoryState = newState;
        while (fullMenuCanvasGroup.alpha < 1)
        {
            fullMenuCanvasGroup.alpha += Time.deltaTime * 5;
            yield return null;
        }
        GameManager.instance.isLoading = false;
        InventoryManager.instance.inventoryCurrentOption = 0;
    }

    public IEnumerator FadeOut()
    {
        fullMenuCanvasGroup.alpha = 1;
        while (fullMenuCanvasGroup.alpha > 0)
        {
            fullMenuCanvasGroup.alpha -= Time.deltaTime * 5;
            yield return null;
        }
        yield return null;
    }

    public void FillWeaponList()
    {
        for (int i = 0; i < inventory.weaponList.Count; i++)
        {
            if(inventory.weaponList[i].equipment.type == Item.Types.WEAPON)
            {
                GameObject newWeaponHolder = Instantiate(weaponHolderPrefab, weaponSpacer, false);
                ItemHolder weaponHolderScript = newWeaponHolder.GetComponent<ItemHolder>();
                weaponHolderScript.amountText.text = inventory.weaponList[i].currentAmount.ToString();
                weaponHolderScript.nameText.text = inventory.weaponList[i].equipment.name;
                weaponHolderScript.typeImage.sprite = inventory.weaponList[i].equipment.icon;

                weaponHolderScript.typeText.text = inventory.weaponList[i].equipment.type.ToString();

                weaponHolderList.Add(newWeaponHolder);
            }

        }
    }

    public void WeaponRecreateList()
    {
        foreach (GameObject itemHolder in weaponHolderList)
        {
            Destroy(itemHolder);
        }
        weaponHolderList.Clear();
        Inventory.instance.tempWeaponList.Clear();
        //FillWeaponList();
    }

    public void CanvasAlphaSwapper(CanvasGroup whatYouWannaDisappear)
    {
        if (whatYouWannaDisappear.alpha == 1)
        {
            whatYouWannaDisappear.alpha = 0;
        }
        else if (whatYouWannaDisappear.alpha == 0)
        {
            whatYouWannaDisappear.alpha = 1;
        }
    }

    public IEnumerator FadeInCanvas()
    {
        GameManager.instance.isLoading = true;
        InventoryManager.instance.charOneCanvasGroup.alpha = 0;
        while (InventoryManager.instance.charOneCanvasGroup.alpha < 1)
        {
            InventoryManager.instance.charOneCanvasGroup.alpha -= Time.deltaTime * 5;
            yield return null;
        }

        GameManager.instance.isLoading = false;
    }

    public void CustomEquipmentList(Equipment.Types typeToDisplay, Equipment.UsableBy usedBy)
    {//used to generate the correct listing when selecting equipment in the menus
        WeaponRecreateList();
        for (int i = 0; i < inventory.weaponList.Count; i++)
        {
            if (inventory.weaponList[i].equipment.type == typeToDisplay && (inventory.weaponList[i].equipment.useBy == usedBy || inventory.weaponList[i].equipment.useBy == Equipment.UsableBy.ALL))
                {
                    GameObject newWeaponHolder = Instantiate(weaponHolderPrefab, weaponSpacer, false);
                    ItemHolder weaponHolderScript = newWeaponHolder.GetComponent<ItemHolder>();
                    weaponHolderScript.amountText.text = inventory.weaponList[i].currentAmount.ToString();
                    weaponHolderScript.nameText.text = inventory.weaponList[i].equipment.name;
                    weaponHolderScript.typeImage.sprite = inventory.weaponList[i].equipment.icon;

                    weaponHolderScript.typeText.text = inventory.weaponList[i].equipment.type.ToString();

                    weaponHolderList.Add(newWeaponHolder);
                Inventory.instance.tempWeaponList.Add(inventory.weaponList[i]);
                }
           

        }
    }
    public void FillMateriaList()
    {
        MateriaRecreateList();
        for (int i = 0; i < inventory.materiaList.Count; i++)
        {
            if (inventory.materiaList[i].type == Item.Types.MATERIA)
            {
                GameObject newMateriaHolder = Instantiate(materiaHolderPrefab, materiaSpacer, false);
                ItemHolder materiaHolderScript = newMateriaHolder.GetComponent<ItemHolder>();
                materiaHolderScript.amountText.text ="Level " + (inventory.materiaList[i].currentLevel + 1).ToString();
                materiaHolderScript.nameText.text = inventory.materiaList[i].name;
                materiaHolderScript.typeImage.sprite = inventory.materiaList[i].icon;

                materiaHolderScript.typeText.text = inventory.materiaList[i].materiaType.ToString();

                materiaHolderList.Add(newMateriaHolder);

            }

        }
    }

    public void RemoveMateriaFromEquipment(Materia materiaToRemove)
    {
        GameObject newMateriaHolder = Instantiate(materiaHolderPrefab, materiaSpacer, false);
        ItemHolder materiaHolderScript = newMateriaHolder.GetComponent<ItemHolder>();
        materiaHolderScript.amountText.text ="Level " + (materiaToRemove.currentLevel + 1).ToString();
        materiaHolderScript.nameText.text = materiaToRemove.name;
        materiaHolderScript.typeImage.sprite = materiaToRemove.icon;

        materiaHolderScript.typeText.text = materiaToRemove.materiaType.ToString();

        materiaHolderList.Add(newMateriaHolder);
        Debug.Log("just did it");
    }

    public void MateriaRecreateList()
    {
        foreach (GameObject itemHolder in materiaHolderList)
        {
            Destroy(itemHolder);
        }
        materiaHolderList.Clear();

    }

    private void UpdateCurrentPlaytime()
    {
        int totalCurrentTimeInSeconds = Mathf.RoundToInt(GameManager.instance.totalPlayedTime);
        int totalCurrentTimeInMinutes = totalCurrentTimeInSeconds / 60 ;
        int totalCurrentTimeInHours = totalCurrentTimeInMinutes / 60;
        if (totalCurrentTimeInHours >= 1)
        {
            totalCurrentTimeInMinutes -= (totalCurrentTimeInHours * 60);        }

        if (totalCurrentTimeInMinutes < 10)
        {
            currentPlaytimeMinutesText.text = "0" + totalCurrentTimeInMinutes;
        }
        else
        {
            currentPlaytimeMinutesText.text = totalCurrentTimeInMinutes.ToString();
        }


        if (totalCurrentTimeInHours < 10)
        {
            currentPlaytimeHoursText.text ="0" + totalCurrentTimeInHours.ToString();
        }
        else
        {
            currentPlaytimeHoursText.text = totalCurrentTimeInHours.ToString();
        }

    }

    public void UpdatePlaytimeEveryMinute()
    {
        if (!timerUpdaterIsRunning)
        {
            Debug.Log("timer updater is not running, starting now");
            timerUpdaterIsRunning = true;
            StartCoroutine(RunTheUpdater());

        }
        else
        {
            Debug.Log("timer updater is already running, finishing coroutine early");
        }
    }

    public IEnumerator RunTheUpdater()
    {
        UpdateCurrentPlaytime();
        yield return new WaitForSeconds(60);
        if (timerUpdaterIsRunning)
        {
            StartCoroutine(RunTheUpdater());
            Debug.Log("restarted the coroutine");
        }
    }

    public void UpdateGilValue()
    {
        currentGilText.text = Inventory.instance.currentGil.ToString();
    }




    /*
    public void FillSmallMateriaList(int pageToDisplay)
    {//testing using pages instead
        MateriaRecreateList();
        if(pageToDisplay == 0)
        {


            for (int i = 0; i < 15; i++)
            {
                if (inventory.materiaList[i].type == Item.Types.MATERIA)
                {
                    if (inventory.materiaList[i] != null)
                    {
                        GameObject newMateriaHolder = Instantiate(materiaHolderPrefab, materiaSpacer, false);
                        ItemHolder materiaHolderScript = newMateriaHolder.GetComponent<ItemHolder>();
                        materiaHolderScript.amountText.text = "Level " + (inventory.materiaList[i].currentLevel + 1).ToString();
                        materiaHolderScript.nameText.text = inventory.materiaList[i].name;
                        materiaHolderScript.typeImage.sprite = inventory.materiaList[i].icon;

                        materiaHolderScript.typeText.text = inventory.materiaList[i].materiaType.ToString();

                        materiaHolderList.Add(newMateriaHolder);
                    }

                }
            }

        }
        if (pageToDisplay == 1)
        {


            for (int i = 15; i < 30; i++)
            {
                if (inventory.materiaList[i].type == Item.Types.MATERIA)
                {
                    if (inventory.materiaList[i] != null)
                    {
                        GameObject newMateriaHolder = Instantiate(materiaHolderPrefab, materiaSpacer, false);
                        ItemHolder materiaHolderScript = newMateriaHolder.GetComponent<ItemHolder>();
                        materiaHolderScript.amountText.text = "Level " + (inventory.materiaList[i].currentLevel + 1).ToString();
                        materiaHolderScript.nameText.text = inventory.materiaList[i].name;
                        materiaHolderScript.typeImage.sprite = inventory.materiaList[i].icon;

                        materiaHolderScript.typeText.text = inventory.materiaList[i].materiaType.ToString();

                        materiaHolderList.Add(newMateriaHolder);
                    }

                }
            }

        }
    }
*/

}
