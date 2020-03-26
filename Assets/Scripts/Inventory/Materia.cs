using System.Collections;
using System.Collections.Generic;
using UnityEngine;


[CreateAssetMenu(fileName = "Materia", menuName = "Materia")]
public class Materia : Item
{
    //public Sprite materiaImage;
    //public string materiaName;
    public int currentLevel;
    public int maxLevel;
    public int currentAP;
    public int[] apToLevel;
    public string[] skillNames;
    public string[] skillDescriptions;
    public string[] bonusStats;
    public MateriaType materiaType;
 


    public enum MateriaType
    {
        Personal,
        Magic,
        Skill,
        Support,
        Independant
    }
}
