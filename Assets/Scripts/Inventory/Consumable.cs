using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName ="Consumable", menuName ="Consumables")]
public class Consumable : Item
{
    public int hpAmount;
    public int mpAmount;

    public override void UseItem()
    {
        base.UseItem();
        //remove item from inventory
        //add hp or mp to selected character
    }
}
