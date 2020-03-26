using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ShopUI : MonoBehaviour
{
    public static ShopUI instance;






    [Header("Equipment Holder")]
    public List<GameObject> equipmentHolderList = new List<GameObject>();
    public GameObject equipmentHolderPrefab;
    public Transform equipmentSpacer;

    [Header("Shop List and Prices")]
    public List<Equipment> shopEquipmentList = new List<Equipment>();

    [Header("Confirm Screen Modifiers")]
    [SerializeField] TMPro.TMP_Text itemName;
    [SerializeField] TMPro.TMP_Text itemDescription;
    [SerializeField] TMPro.TMP_Text itemEffect;
    [SerializeField] TMPro.TMP_Text itemPower;
    [SerializeField] TMPro.TMP_Text itemArmor;
    [SerializeField] TMPro.TMP_Text itemMatk;
    [SerializeField] TMPro.TMP_Text itemMdef;
    [SerializeField] TMPro.TMP_Text itemSpeed;
    [SerializeField] TMPro.TMP_Text itemCrit;
    [SerializeField] TMPro.TMP_Text itemHit;
    [SerializeField] TMPro.TMP_Text itemDodge;
    [SerializeField] TMPro.TMP_Text itemUsableBy;

    [SerializeField] Image itemImage;


    [Header("Info Panel ")]
    [SerializeField] SlotController equipmentScreenSlotController;
    public int tempCurrentGil;
    private int totalOwnedCounter;
    private int totalEquippedCounter;
    [SerializeField] TMPro.TMP_Text currentGil;
    [SerializeField] TMPro.TMP_Text currentOwned;
    [SerializeField] TMPro.TMP_Text currentEquipped;


    [Header("Text")]
    public string variableShopEnterText;
    public string variableShopExitText;

    [Header("Alert Panel")]

    [SerializeField] TMPro.TMP_Text alertTextObject;
    [SerializeField] string alertTextToDisplay;
    private bool alertDisplaying = false;
    // public float alertTimer;

    [Header("Character Display")]
    public CharHolder charHolderReference;


    private void Awake()
    {
        instance = this;
    }

    public void FillEquipmentShopList()
    {
        for (int i = 0; i < shopEquipmentList.Count; i++)
        {
            GameObject newShopEquipmentHolder = Instantiate(equipmentHolderPrefab, equipmentSpacer, false);
            ItemHolder shopEquipmentHolderScript = newShopEquipmentHolder.GetComponent<ItemHolder>();
            shopEquipmentHolderScript.amountText.text = shopEquipmentList[i].buyPrice.ToString();
            shopEquipmentHolderScript.nameText.text = shopEquipmentList[i].name;
            shopEquipmentHolderScript.typeImage.sprite = shopEquipmentList[i].icon;
            equipmentHolderList.Add(newShopEquipmentHolder);

        }
    }


    public void EquipmentShopRecreateList()
    {
        foreach (GameObject itemHolder in equipmentHolderList)
        {
            Destroy(itemHolder);
        }
        equipmentHolderList.Clear();

        FillEquipmentShopList();
    }

    public void UpdateEquipmentConfirmScreen(Equipment equipmentToShow)
    {
        itemName.text = equipmentToShow.name;
        itemDescription.text = equipmentToShow.description;
        itemEffect.text = equipmentToShow.effect;
        itemImage.sprite = equipmentToShow.icon;
        itemPower.text = equipmentToShow.power.ToString();
        itemArmor.text = equipmentToShow.armor.ToString();
        itemMatk.text = equipmentToShow.magicAttack.ToString();
        itemMdef.text = equipmentToShow.magicDefense.ToString();
        itemSpeed.text = equipmentToShow.speed.ToString();
        itemCrit.text = equipmentToShow.crit.ToString();
        itemHit.text = equipmentToShow.hit.ToString();
        itemDodge.text = equipmentToShow.dodge.ToString();
        itemUsableBy.text = equipmentToShow.useBy.ToString();
    }

    public void EquipmentBuyAttempt(int equipmentLocation)
    {
        int buyPrice = shopEquipmentList[equipmentLocation].buyPrice;
        Debug.Log("trying to buy");

        if (tempCurrentGil >= buyPrice)
        {
            Debug.Log("you can buy this");
            tempCurrentGil = tempCurrentGil - buyPrice;
            UpdateGilUI();
            SoundManager.instance.PlaySFX(12);
            Inventory.instance.AddWeapon(shopEquipmentList[equipmentLocation]);
            EquipmentChecker(shopEquipmentList[equipmentLocation]);
            
        }
        else if (tempCurrentGil < buyPrice)
        {
            alertTextToDisplay = (" You need " + (buyPrice - tempCurrentGil) + " more gil ya fuckin turd");
            alertTextObject.text = alertTextToDisplay;
            TurnOnAlertPanel();
        }
    }

    public void UpdateGilUI()
    {
        currentGil.text = tempCurrentGil.ToString();
    }

    private void TurnOnAlertPanel()
    {
        if (ShopManager.instance.alertGameObject.activeInHierarchy)
        {
           // Debug.Log("the alert panel is already on, refreshing its timer to 1 second");
          //  alertTimer = 0;
        }
        else if (!ShopManager.instance.alertGameObject.activeInHierarchy)
        {
            Debug.Log("Alert panel isn't active, lets turn it on for a second");

            ShopManager.instance.alertGameObject.SetActive(true);
            //  alertTimer = 0;
            SoundManager.instance.PlaySFX(3);
            StartCoroutine(CountdownAlertTimer());
        }
    }
    private IEnumerator CountdownAlertTimer()
    {
        yield return new WaitForSeconds(1f);
        ShopManager.instance.alertGameObject.SetActive(false);

    }

    private void CheckForEquippedItem(Equipment equipmentToCheck)
    {//this doesn't include the character benched equipped items though
        totalEquippedCounter = 0;
        for (int i = 0; i < GameManager.instance.playerClasses.Length; i++)
        {
            if (equipmentToCheck.name == GameManager.instance.playerClasses[i].equippedWeapon.name || equipmentToCheck.name == GameManager.instance.playerClasses[i].equippedArmor.name || equipmentToCheck.name == GameManager.instance.playerClasses[i].equippedAccessory.name)
            {
                totalEquippedCounter++;
            }

        }
        currentEquipped.text = totalEquippedCounter.ToString();
        Debug.Log("there is a total of this many equipped " + totalEquippedCounter);
    }

    private void CheckForOwnedItem(Equipment equipmentToCheck)
    {
        totalOwnedCounter = 0;
        for (int i = 0; i < Inventory.instance.weaponList.Count; i++)
        {
            if (equipmentToCheck.name == Inventory.instance.weaponList[i].equipment.name)
            {
                totalOwnedCounter = totalOwnedCounter + Inventory.instance.weaponList[i].currentAmount;
            }
        }
        totalOwnedCounter = totalOwnedCounter + totalEquippedCounter;
        currentOwned.text = totalOwnedCounter.ToString();
        Debug.Log("there is a total of this many owned " + totalOwnedCounter);
    }

    public void EquipmentChecker(Equipment check)
    {
        CheckForEquippedItem(check);
        CheckForOwnedItem(check);
        Debug.Log("finished the check, and found equpped/owned " + totalEquippedCounter + " " + totalOwnedCounter);
        SlotDisplay(check);

    }














































































    private void SlotDisplay(Equipment equipmentToCheck)
    {
        if (equipmentToCheck.numberOfSlots == Equipment.Slots.NONE)
        {
            int[] singleSlotsToSend = { };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };

             equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend);
        }


        
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.ONE)
        {
            int[] singleSlotsToSend = { 0 };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };

           equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.TWO)
        {
            int[] singleSlotsToSend = { 0, 1 };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.TWO_LINKED)
        {
            int[] singleSlotsToSend = { };
            int[] leftLinkSlotToSend = { 0 };
            int[] rightLinkSlotToSend = { 1 };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 

        }

        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.THREE)
        {
            int[] singleSlotsToSend = { 0, 1, 2 };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.THREE_AND_TWOLINKED)
        {
            int[] singleSlotsToSend = { 2 };
            int[] leftLinkSlotToSend = { 0 };
            int[] rightLinkSlotToSend = { 1 };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend);

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.FOUR)
        {
            int[] singleSlotsToSend = { 0, 1, 2, 3 };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.FOUR_AND_TWOLINKED)
        {
            int[] singleSlotsToSend = { 2, 3 };
            int[] leftLinkSlotToSend = { 0 };
            int[] rightLinkSlotToSend = { 1 };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 

        }

        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.FOUR_AND_TWO_TWOLINKED)
        {
            int[] singleSlotsToSend = { };
            int[] leftLinkSlotToSend = { 0, 2 };
            int[] rightLinkSlotToSend = { 1, 3 };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 

        }

        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.FIVE)
        {
            int[] singleSlotsToSend = { 0, 1, 2, 3, 4 };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.FIVE_AND_TWOLINKED)
        {
            int[] singleSlotsToSend = { 2, 3, 4 };
            int[] leftLinkSlotToSend = { 0 };
            int[] rightLinkSlotToSend = { 1 };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.FIVE_AND_TWO_TWOLINKED)
        {
            int[] singleSlotsToSend = { 4 };
            int[] leftLinkSlotToSend = { 0, 2 };
            int[] rightLinkSlotToSend = { 1, 3 };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.SIX)
        {
            int[] singleSlotsToSend = { 0, 1, 2, 3, 4, 5 };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 

        }

        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.SIX_AND_TWOLINKED)
        {
            int[] singleSlotsToSend = { 2, 3, 4, 5 };
            int[] leftLinkSlotToSend = { 0 };
            int[] rightLinkSlotToSend = { 1 };
equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.SIX_AND_TWO_TWOLINKED)
        {
            int[] singleSlotsToSend = { 4, 5 };
            int[] leftLinkSlotToSend = { 0, 2 };
            int[] rightLinkSlotToSend = { 1, 3 };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend);

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.SIX_AND_THREE_TWOLINKED)
        {
            int[] singleSlotsToSend = { };
            int[] leftLinkSlotToSend = { 0, 2, 4 };
            int[] rightLinkSlotToSend = { 1, 3, 5 };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 
        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.SEVEN)
        {
            int[] singleSlotsToSend = { 0, 1, 2, 3, 4, 5, 6 };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 
        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.SEVEN_AND_TWOLINKED)
        {
            int[] singleSlotsToSend = { 2, 3, 4, 5, 6 };
            int[] leftLinkSlotToSend = { 0 };
            int[] rightLinkSlotToSend = { 1 };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend);
        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.SEVEN_AND_TWO_TWOLINKED)
        {
            int[] singleSlotsToSend = { 4, 5, 6 };
            int[] leftLinkSlotToSend = { 0, 2 };
            int[] rightLinkSlotToSend = { 1, 3 };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 
        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.SEVEN_AND_THREE_TWOLINKED)
        {
            int[] singleSlotsToSend = { 6 };
            int[] leftLinkSlotToSend = { 0, 2, 4 };
            int[] rightLinkSlotToSend = { 1, 3, 5 };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.EIGHT)
        {
            int[] singleSlotsToSend = { 0, 1, 2, 3, 4, 5, 6, 7 };
            int[] leftLinkSlotToSend = { };
            int[] rightLinkSlotToSend = { };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 
        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.EIGHT_AND_TWOLINKED)
        {
            int[] singleSlotsToSend = { 2, 3, 4, 5, 6, 7 };
            int[] leftLinkSlotToSend = { 0 };
            int[] rightLinkSlotToSend = { 1 };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend);

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.EIGHT_AND_TWO_TWOLINKED)
        {
            int[] singleSlotsToSend = { 4, 5, 6, 7 };
            int[] leftLinkSlotToSend = { 0, 2 };
            int[] rightLinkSlotToSend = { 1, 3 };
equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 
        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.EIGHT_AND_THREE_TWOLINKED)
        {
            int[] singleSlotsToSend = { 6, 7 };
            int[] leftLinkSlotToSend = { 0, 2, 4 };
            int[] rightLinkSlotToSend = { 1, 3, 5 };
equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 

        }
        else if (equipmentToCheck.numberOfSlots == Equipment.Slots.EIGHT_AND_FOUR_TWOLINKED)
        {
            int[] singleSlotsToSend = { };
            int[] leftLinkSlotToSend = { 0, 2, 4, 6 };
            int[] rightLinkSlotToSend = { 1, 3, 5, 7 };
 equipmentScreenSlotController.DisplayEmptySlots(singleSlotsToSend, leftLinkSlotToSend, rightLinkSlotToSend); 
        }

    }
}
