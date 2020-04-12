using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[CreateAssetMenu(fileName = "Card", menuName = "Card")]
public class Card : ScriptableObject
{
    public int cardID = 0;
    public string description = null;

    public enum cardRarity
    {
        Common,
        Uncommon,
        Rare,
        Elite
    }

    public cardRarity cardsRarity;
    public string cardName;


    public Sprite playerImage;
    public Sprite enemyImage;
    public int amountOwned = 0;
    public bool hasOwnedBefore = false;

    public bool myCard;
    public bool enemyCard;
    public int cardSlot;
    public int percentChanceForEnemyToUseInBattle;

    //new test stuff
    public int[] cardValues = new int[4];
    
    
}

