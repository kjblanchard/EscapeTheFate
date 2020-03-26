using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "CharacterClass", menuName = "CharacterClass")]

public class CharacterClass : ScriptableObject
{
    [Header("Char Stats")]
    public int currentHP;
    public int[] maxHP;
    public int currentMP;
    public int[] maxMP;
    public int[] str;
    public int[] def;
    public int[] mag;
    public int[] spr;
    public int[] speed;
    public int hit;
    public int[] luck;
    public int currentLevel;
    public int maxLevel;
    public int limitGuage;
    public int currentEXP;
    public int[] expToNextLevel;

    //public StatusEffect currentStatusEffect

    [Header("Char Description")]

    public string characterName;
    public string characterClass;
    public string characterBio;
    public Equipment equippedWeapon;
    public Equipment equippedArmor;
    public Equipment equippedAccessory;
    public Sprite characterPortrait;
    public Sprite characterAnimation;
    public RuntimeAnimatorController characterWalkAnim;
    public Materia[] weaponEquippedMateria = new Materia[8];
    public Materia[] armorEquippedMateria = new Materia[8];
    public Materia[] accEquippedMateria = new Materia[8];

    public Equipment.UsableBy whoCanUse;
    

    //public Skill[] availableSkills


    //nextability learned





}
