using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CharStatsHolder : MonoBehaviour
{
    public bool characterSpotTaken;

    public CharacterClass playersClass;

    //public int currentHP;
    public int currentHP;
    public int currentMaxHP;
    public int currentMP;
    public int currentMaxMP;
    public int currentStr;
    public int currentDef;
    public int currentMag;
    public int currentSpr;
    [Range(0,255)]
    public int currentSpeed;
    public int currentLuck;
    public int currentPower;
    public int currentArmor;
    public int currentMagAtk;
    public int currentMagDef;
    public int currentCrit;
    public int currentDodge;
    public int currentHit;
    public int currentLevel;
    public int currentLimit;
    public int currentEXP;
    public int EXPtoNextLevel;


    public string characterName;
    public string characterClass;
    public string characterBio;
    public Equipment equippedWeapon;
    public Equipment equippedArmor;
    public Equipment equippedAccessory;
    public Sprite characterPortrait;
    public Sprite characterAnimation;
    public RuntimeAnimatorController characterWalkAnimation;
    public Equipment.UsableBy currentUseBy;
    public Materia[] weaponEquippedMateria = new Materia[8];
    public Materia[] armorEquippedMateria = new Materia[8];
    public Materia[] accEquippedMateria = new Materia[8];



}