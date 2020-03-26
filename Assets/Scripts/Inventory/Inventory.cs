using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Inventory : MonoBehaviour
{

    public int currentGil; 

    public static Inventory instance;
    public int maxAmount = 99;

    private List<InventoryItem> tempList = new List<InventoryItem>();


    public List<InventoryItem> inventoryList = new List<InventoryItem>();

    [Header("Weapon Inventory")]
    public List<InventoryWeapon> weaponList = new List<InventoryWeapon>();
    public List<InventoryWeapon> tempWeaponList = new List<InventoryWeapon>();

    [Header("Materia Inventory")]
    public List<Materia> materiaList = new List<Materia>();

    [System.Serializable]
    public class InventoryItem
    {
        public Item item;
        //public Equipment equipment;
        public int currentAmount;

    }

    [System.Serializable]
    public class InventoryWeapon
    {
        public Equipment equipment;
        public int currentAmount;

    }


    public Equipment tempWeaponHolder;
    public Materia tempMateriaHolder;

    private void Awake()
    {
        instance = this;
    }

    public void AddItem(Item itemToAdd)
    {
        InventoryItem inventItem = new InventoryItem();
        inventItem.item = itemToAdd;
        bool fullStack = false;

        for (int i = 0; i < inventoryList.Count; i++)
        {
            if(inventoryList[i].item == itemToAdd && inventoryList[i].currentAmount < maxAmount)
            {
                inventoryList[i].currentAmount++;
                return;
            }
            else
            {
                fullStack = true;
                
            }
        }
        if (!fullStack)
        {
            inventItem.currentAmount++;
            inventoryList.Add(inventItem);
        }


    }

    public void RemoveItem(Item itemToRemove)
    {
        for (int i = 0; i < inventoryList.Count; i++)
        {
            if (inventoryList[i].item == itemToRemove && inventoryList[i].currentAmount > 0)
            {
                inventoryList[i].currentAmount--;

                if(inventoryList[i].currentAmount <= 0)
                {
                    inventoryList.Remove(inventoryList[i]);
                    return;
                }
                
            }
        }
    }

    public void SwapItems(int item1, int item2)
    {
        InventoryItem tempItem = inventoryList[item1];
        inventoryList[item1] = inventoryList[item2];
        inventoryList[item2] = tempItem;
        InventoryUI.instance.RecreateList();
    }

    public void OrganizeItems()
    {
        for (int i = 0; i < inventoryList.Count - 1; i++)
        {
            if(inventoryList[i].item.type == Item.Types.CONSUMABLE)
            {
               // Debug.Log(i + "is a consumable item");
            }

            if(inventoryList[i].item.type == Item.Types.KEY_ITEM)
            {
                int tempNu = tempList.Count;
                tempList.Add(inventoryList[i]);
                tempList[tempNu].currentAmount = inventoryList[i].currentAmount;
                //Debug.Log("I just added " + inventoryList[i].item.name + " to the temporary list and then removed it");
                inventoryList[i] = null;
            }

        }

        inventoryList.RemoveAll(item => item == null);

        for (int i = 0; i < tempList.Count; i++)
        {
            int tempNu = inventoryList.Count;
            inventoryList.Add(tempList[i]);
            inventoryList[tempNu].currentAmount = tempList[i].currentAmount;
        }

        InventoryUI.instance.RecreateList();
        InventoryManager.instance.inventoryState = InventoryManager.InventoryStates.INVENTORY_PANEL;
        tempList.Clear();
    }

    public void AddWeapon(Equipment weaponToAdd)
    {
        InventoryWeapon inventItem = new InventoryWeapon();
        inventItem.equipment = weaponToAdd;
        bool fullStack = false;
        if(weaponToAdd.name == "Empty")
        {
            return;
        }

        for (int i = 0; i < weaponList.Count; i++)
        {
            if (weaponList[i].equipment == weaponToAdd && weaponList[i].currentAmount < maxAmount)
            {
                weaponList[i].currentAmount++;
                return;
            }
            else
            {
                fullStack = true;

            }
        }
        if (fullStack)
        {
            fullStack = false;
            inventItem.currentAmount++;
            weaponList.Add(inventItem);
        }


    }

    public void RemoveWeapon(Equipment weaponToRemove)
    {
        for (int i = 0; i < weaponList.Count; i++)
        {
            if (weaponList[i].equipment == weaponToRemove && weaponList[i].currentAmount > 0)
            {
                weaponList[i].currentAmount--;

                if (weaponList[i].currentAmount <= 0)
                {
                    weaponList.Remove(weaponList[i]);
                    return;
                }

            }
        }
    }

    public void AddMateria(Materia materiaToAdd)
    {

        if (materiaToAdd.name == "Empty")
        {
            return;
        }

        Inventory.instance.materiaList.Add(materiaToAdd);


    }

    public void RemoveMateria(int materiaSlotToRemove)
    {
        Inventory.instance.materiaList.RemoveAt(materiaSlotToRemove);
    }

    

}
