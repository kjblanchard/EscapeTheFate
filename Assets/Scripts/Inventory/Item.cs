using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//[CreateAssetMenu(fileName ="New Item", menuName ="Inventory")]

public class Item : ScriptableObject
{
   new public string name;
    public int itemID = 0;
    public Sprite icon = null;
    public string description = null;
    public int buyPrice;
    public int sellPrice;


    //public int slots;
    public string effect;


    public enum Types
    {
        CONSUMABLE,
        KEY_ITEM,
        WEAPON,
        ARMOR,
        ACCESSORY,
        MATERIA,

    }

    public enum UsableBy
    {
        ALL,
        NONE,
        Kevin,
        Todd,
        Cory
    }

    public UsableBy useBy;

    public Types type;


    public virtual void UseItem()
    {

    }
}
