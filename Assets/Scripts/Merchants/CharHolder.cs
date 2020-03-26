using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CharHolder : MonoBehaviour
{
    [SerializeField] GameObject[] charGameObjects;
    [SerializeField] Image[] charGameImages;
    [SerializeField] CharacterClass[] tempCharClass;

    [SerializeField] TMPro.TMP_Text[] atkValues;
    [SerializeField] TMPro.TMP_Text[] defValues;
    [SerializeField] TMPro.TMP_Text[] matkValues;
    [SerializeField] TMPro.TMP_Text[] mdefValues;

    private Equipment.Types typeToCheck;
    private int tempAtkValue;
    private int tempDefValue;
    private int tempMatkValue;
    private int tempMdefValue;


    private void TurnOnCharacter()
    {
        for (int i = 0; i < (charGameObjects.Length - 2); i++)
        {
            if(GameManager.instance.playerClasses.Length >= (i+1) )
            {
                charGameObjects[i].SetActive(true);

                tempCharClass[i] = GameManager.instance.playerClasses[i];
            }
            else
            {
                tempCharClass[i] = null;

            }
        }
        if(GameManager.instance.characterBench.benchedPlayers.Length == 0)
        {
            charGameObjects[3].SetActive(false);
            tempCharClass[3] = null;
            charGameObjects[4].SetActive(false);
            tempCharClass[4] = null;
        }
        else if(GameManager.instance.characterBench.benchedPlayers.Length == 1)
        {
            charGameObjects[3].SetActive(true);
            tempCharClass[3] = GameManager.instance.characterBench.benchedPlayers[0];
            charGameObjects[4].SetActive(false);
            tempCharClass[4] = null;

        }
        else if(GameManager.instance.characterBench.benchedPlayers.Length == 2)
        {
            charGameObjects[3].SetActive(true);
            tempCharClass[3] = GameManager.instance.characterBench.benchedPlayers[0];
            charGameObjects[4].SetActive(true);
            tempCharClass[4] = GameManager.instance.characterBench.benchedPlayers[1];
        }

    }

    private void LoadCharImage()
    {
        for (int i = 0; i < ( charGameImages.Length); i++)
        {
            if(tempCharClass[i] != null)
            {
                charGameImages[i].sprite = tempCharClass[i].characterPortrait;

            }
        }
    }

    public void LoadCharacters()
    {
        TurnOnCharacter();
        LoadCharImage();
    }

    public void EquipmentComparison(Equipment equipmentToCheck)
    {
        for (int i = 0; i < tempCharClass.Length; i++)
        {
            if (tempCharClass[i] != null)
            {
                if ((equipmentToCheck.useBy.ToString() == tempCharClass[i].characterName) || (equipmentToCheck.useBy == Equipment.UsableBy.ALL))
                {

                    charGameImages[i].color = new Color(1, 1, 1);
                    typeToCheck = equipmentToCheck.type;

                    if (typeToCheck == Item.Types.WEAPON)
                    {
                        tempAtkValue = equipmentToCheck.power - tempCharClass[i].equippedWeapon.power;
                        if (tempAtkValue == 0)
                        {
                            atkValues[i].color = new Color(1, 1, 1);
                            atkValues[i].text = "+ " + tempAtkValue.ToString();
                        }
                        else if (tempAtkValue > 0)
                        {
                            atkValues[i].color = new Color(0, 1, 0);
                            atkValues[i].text = "+ " + tempAtkValue.ToString();
                        }
                        else if (tempAtkValue < 0)
                        {
                            atkValues[i].color = new Color(1, 0, 0);
                            atkValues[i].text = tempAtkValue.ToString();
                        }
                        else
                        {
                            Debug.Log("i need helps");
                        }
                        tempDefValue = equipmentToCheck.armor- tempCharClass[i].equippedWeapon.armor;
                        if (tempDefValue == 0)
                        {
                            defValues[i].color = new Color(1, 1, 1);
                            defValues[i].text = "+ " + tempDefValue.ToString();
                        }
                        else if (tempDefValue > 0)
                        {
                            defValues[i].color = new Color(0, 1, 0);
                            defValues[i].text = "+ " + tempDefValue.ToString();
                        }
                        else if (tempDefValue < 0)
                        {
                            defValues[i].color = new Color(1, 0, 0);
                            defValues[i].text = tempDefValue.ToString();
                        }
                        else
                        {
                            Debug.Log("i need helps");
                        }
                        tempMatkValue = equipmentToCheck.magicAttack - tempCharClass[i].equippedWeapon.magicAttack;
                        if (tempMatkValue == 0)
                        {
                            matkValues[i].color = new Color(1, 1, 1);
                            matkValues[i].text = "+ " + tempMatkValue.ToString();
                        }
                        else if (tempMatkValue > 0)
                        {
                            matkValues[i].color = new Color(0, 1, 0);
                            matkValues[i].text = "+ " + tempMatkValue.ToString();
                        }
                        else if (tempMatkValue < 0)
                        {
                            matkValues[i].color = new Color(1, 0, 0);
                            matkValues[i].text = tempMatkValue.ToString();
                        }
                        else
                        {
                            Debug.Log("i need helps");
                        }
                        tempMdefValue = equipmentToCheck.magicDefense - tempCharClass[i].equippedWeapon.magicDefense;
                        if (tempMdefValue == 0)
                        {
                            mdefValues[i].color = new Color(1, 1, 1);
                            mdefValues[i].text = "+ " + tempMdefValue.ToString();
                        }
                        else if (tempMdefValue > 0)
                        {
                            mdefValues[i].color = new Color(0, 1, 0);
                            mdefValues[i].text = "+ " + tempMdefValue.ToString();
                        }
                        else if (tempMdefValue < 0)
                        {
                            mdefValues[i].color = new Color(1, 0, 0);
                            mdefValues[i].text = tempMdefValue.ToString();
                        }
                        else
                        {
                            Debug.Log("i need helps");
                        }
                    }
                    else if (typeToCheck == Item.Types.ARMOR)
                    {
                        tempAtkValue = equipmentToCheck.power - tempCharClass[i].equippedArmor.power;
                        if (tempAtkValue == 0)
                        {
                            atkValues[i].color = new Color(1, 1, 1);
                            atkValues[i].text = "+ " + tempAtkValue.ToString();

                        }
                        else if (tempAtkValue > 0)
                        {
                            atkValues[i].color = new Color(0, 1, 0);
                            atkValues[i].text = "+ " + tempAtkValue.ToString();
                        }
                        else if (tempAtkValue < 0)
                        {
                            atkValues[i].color = new Color(1, 0, 0);
                            atkValues[i].text = tempAtkValue.ToString();
                        }
                        else
                        {
                            Debug.Log("i need helps");
                        }
                        tempDefValue = equipmentToCheck.armor - tempCharClass[i].equippedArmor.armor;
                        if (tempDefValue == 0)
                        {
                            defValues[i].color = new Color(1, 1, 1);
                            defValues[i].text = "+ " + tempDefValue.ToString();

                        }
                        else if (tempDefValue > 0)
                        {
                            defValues[i].color = new Color(0, 1, 0);
                            defValues[i].text = "+ " + tempDefValue.ToString();
                        }
                        else if (tempDefValue < 0)
                        {
                            defValues[i].color = new Color(1, 0, 0);
                            defValues[i].text = tempDefValue.ToString();
                        }
                        else
                        {
                            Debug.Log("i need helps");
                        }
                        tempMatkValue = equipmentToCheck.magicAttack - tempCharClass[i].equippedArmor.magicAttack;
                        if (tempMatkValue == 0)
                        {
                            matkValues[i].color = new Color(1, 1, 1);
                            matkValues[i].text = "+ " + tempMatkValue.ToString();
                        }
                        else if (tempMatkValue > 0)
                        {
                            matkValues[i].color = new Color(0, 1, 0);
                            matkValues[i].text = "+ " + tempMatkValue.ToString();
                        }
                        else if (tempMatkValue < 0)
                        {
                            matkValues[i].color = new Color(1, 0, 0);
                            matkValues[i].text = tempMatkValue.ToString();
                        }
                        else
                        {
                            Debug.Log("i need helps");
                        }
                        tempMdefValue = equipmentToCheck.magicDefense - tempCharClass[i].equippedArmor.magicDefense;
                        if (tempMdefValue == 0)
                        {
                            mdefValues[i].color = new Color(1, 1, 1);
                            mdefValues[i].text = "+ " + tempMdefValue.ToString();
                        }
                        else if (tempMdefValue > 0)
                        {
                            mdefValues[i].color = new Color(0, 1, 0);
                            mdefValues[i].text = "+ " + tempMdefValue.ToString();
                        }
                        else if (tempMdefValue < 0)
                        {
                            mdefValues[i].color = new Color(1, 0, 0);
                            mdefValues[i].text = tempMdefValue.ToString();
                        }
                        else
                        {
                            Debug.Log("i need helps");
                        }
                    }
                    else if (typeToCheck == Item.Types.ACCESSORY)
                    {
                        tempAtkValue = equipmentToCheck.power - tempCharClass[i].equippedAccessory.power;
                        if (tempAtkValue == 0)
                        {
                            atkValues[i].color = new Color(1, 1, 1);
                            atkValues[i].text = "+ " + tempAtkValue.ToString();
                        }
                        else if (tempAtkValue > 0)
                        {
                            atkValues[i].color = new Color(0, 1, 0);
                            atkValues[i].text = "+ " + tempAtkValue.ToString();
                        }
                        else if (tempAtkValue < 0)
                        {
                            atkValues[i].color = new Color(1, 0, 0);
                            atkValues[i].text = tempAtkValue.ToString();
                        }
                        else
                        {
                            Debug.Log("i need helps");
                        }
                        tempDefValue = equipmentToCheck.armor - tempCharClass[i].equippedAccessory.armor;
                        if (tempDefValue == 0)
                        {
                            defValues[i].color = new Color(1, 1, 1);
                            defValues[i].text = "+ " + tempDefValue.ToString();
                        }
                        else if (tempDefValue > 0)
                        {
                            defValues[i].color = new Color(0, 1, 0);
                            defValues[i].text = "+ " + tempDefValue.ToString();
                        }
                        else if (tempDefValue < 0)
                        {
                            defValues[i].color = new Color(1, 0, 0);
                            defValues[i].text = tempDefValue.ToString();
                        }
                        else
                        {
                            Debug.Log("i need helps");
                        }
                        tempMatkValue = equipmentToCheck.magicAttack - tempCharClass[i].equippedAccessory.magicAttack;
                        if (tempMatkValue == 0)
                        {
                            matkValues[i].color = new Color(1, 1, 1);
                            matkValues[i].text = "+ " + tempMatkValue.ToString();
                        }
                        else if (tempMatkValue > 0)
                        {
                            matkValues[i].color = new Color(0, 1, 0);
                            matkValues[i].text = "+ " + tempMatkValue.ToString();
                        }
                        else if (tempMatkValue < 0)
                        {
                            matkValues[i].color = new Color(1, 0, 0);
                            matkValues[i].text = tempMatkValue.ToString();
                        }
                        else
                        {
                            Debug.Log("i need helps");
                        }
                        tempMdefValue = equipmentToCheck.magicDefense - tempCharClass[i].equippedAccessory.magicDefense;
                        if (tempMdefValue == 0)
                        {
                            mdefValues[i].color = new Color(1, 1, 1);
                            mdefValues[i].text = "+ " + tempMdefValue.ToString();
                        }
                        else if (tempMdefValue > 0)
                        {
                            mdefValues[i].color = new Color(0, 1, 0);
                            mdefValues[i].text = "+ " + tempMdefValue.ToString();
                        }
                        else if (tempMdefValue < 0)
                        {
                            mdefValues[i].color = new Color(1, 0, 0);
                            mdefValues[i].text = tempMdefValue.ToString();
                        }
                        else
                        {
                            Debug.Log("i need helps");
                        }

                    }



                }
                else
                {
                    charGameImages[i].color = new Color(0, 0, 0);
                    atkValues[i].text = "";
                    defValues[i].text = "";
                    matkValues[i].text = "";
                    mdefValues[i].text = "";
                }

                }
            }
        }

    }
