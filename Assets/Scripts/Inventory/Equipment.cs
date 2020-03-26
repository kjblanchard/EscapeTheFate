using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName ="Equipment", menuName ="Equipment")]
public class Equipment : Item
{
    public int power;
    public int armor;
    public int magicAttack;
    public int magicDefense;
    public int crit;
    public int speed;
    public int hit;
    public int dodge;

    public enum Slots
    {
        NONE,
        ONE,
        TWO,
        TWO_LINKED,
        THREE,
        THREE_AND_TWOLINKED,
        THREELINKED,
        FOUR,
        FOUR_AND_TWOLINKED,
        FOUR_AND_TWO_TWOLINKED,
        FOUR_AND_THREELINKED,
        FOUR_AND_FOURLINKED,
        FIVE,
        FIVE_AND_TWOLINKED,
        FIVE_AND_TWO_TWOLINKED,
        FIVE_AND_THREELINKED,
        FIVE_AND_THREELINKED_AND_TWOLINKED,
        FIVE_AND_FOURLINKED,
        SIX,
        SIX_AND_TWOLINKED,
        SIX_AND_TWO_TWOLINKED,
        SIX_AND_THREE_TWOLINKED,
        SIX_AND_THREELINKED,
        SIX_AND_THREELINKED_AND_TWOLINKED,
        SIX_AND_TWO_THREELINKED,
        SIX_AND_FOURLINKED,
        SIX_AND_FOURLINKED_AND_TWOLINKED,
        SEVEN,
        SEVEN_AND_TWOLINKED,
        SEVEN_AND_TWO_TWOLINKED,
        SEVEN_AND_THREE_TWOLINKED,
        EIGHT,
        EIGHT_AND_TWOLINKED,
        EIGHT_AND_TWO_TWOLINKED,
        EIGHT_AND_THREE_TWOLINKED,
        EIGHT_AND_FOUR_TWOLINKED,
    }

    public Slots numberOfSlots;

    public int intNoOfSlots = 0;

    /*
    public override void UseItem()
    {
        base.UseItem();
        //equip the item
        //remove it temporarily from inventory

    }
    */
}
