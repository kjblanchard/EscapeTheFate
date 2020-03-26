using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
[System.Serializable]
public class SlotController : MonoBehaviour
{
    //public static SlotController instance;

    public GameObject[] singleSlotsGameObjects;
    public Image[] singleSlotSpriteRenderers;

  /*  private void Awake()
    {
        instance = this;

        int slotControllerCount = FindObjectsOfType<SlotController>().Length;

        if(slotControllerCount > 1)
        {
            Destroy(gameObject);
        }
        else
        {
            DontDestroyOnLoad(gameObject);
        }
    }
    */

    public void DisplaySlots(int[] singleSlotsActive,int[] leftLinksActive, int[] rightLinksActive,int equipToCheck)
    {
        int playerToCheck = InventoryManager.instance.playerCurrentOption;
        HideAllSlots();
        for (int i = 0; i < singleSlotsActive.Length; i++)
        {
            if(equipToCheck == 1)
            {
                if(GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[singleSlotsActive[i]].name == "Empty")
                {
                    singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaEmpty;
                }
                else if(GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[singleSlotsActive[i]].materiaType == Materia.MateriaType.Magic)
                    {
                        singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                        singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaMag;
                    }
                else if (GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[singleSlotsActive[i]].materiaType == Materia.MateriaType.Skill)
                {
                    singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaSkill;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[singleSlotsActive[i]].materiaType == Materia.MateriaType.Support)
                {
                    singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaSup;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[singleSlotsActive[i]].materiaType == Materia.MateriaType.Personal)
                {
                    singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaPers;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[singleSlotsActive[i]].materiaType == Materia.MateriaType.Independant)
                {
                    singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaStand;
                }
            }
            if (equipToCheck == 2)
            {
                if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[singleSlotsActive[i]].name == "Empty")
                {
                    singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaEmpty;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[singleSlotsActive[i]].materiaType == Materia.MateriaType.Magic)
                {
                    singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaMag;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[singleSlotsActive[i]].materiaType == Materia.MateriaType.Skill)
                {
                    singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaSkill;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[singleSlotsActive[i]].materiaType == Materia.MateriaType.Support)
                {
                    singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaSup;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[singleSlotsActive[i]].materiaType == Materia.MateriaType.Personal)
                {
                    singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaPers;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[singleSlotsActive[i]].materiaType == Materia.MateriaType.Independant)
                {
                    singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaStand;
                }
            }
            if (equipToCheck == 3)
            {
                if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[singleSlotsActive[i]].name == "Empty")
                {
                    singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaEmpty;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[singleSlotsActive[i]].materiaType == Materia.MateriaType.Magic)
                {
                    singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaMag;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[singleSlotsActive[i]].materiaType == Materia.MateriaType.Skill)
                {
                    singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaSkill;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[singleSlotsActive[i]].materiaType == Materia.MateriaType.Support)
                {
                    singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaSup;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[singleSlotsActive[i]].materiaType == Materia.MateriaType.Personal)
                {
                    singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaPers;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[singleSlotsActive[i]].materiaType == Materia.MateriaType.Independant)
                {
                    singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaStand;
                }
            }



        }
        for (int i = 0; i < leftLinksActive.Length; i++)
        {
            if (equipToCheck == 1)
            {
                if (GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[leftLinksActive[i]].name == "Empty")
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaEmptyLL;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[leftLinksActive[i]].materiaType == Materia.MateriaType.Magic)
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaMagLL;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[leftLinksActive[i]].materiaType == Materia.MateriaType.Skill)
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaSkillLL;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[leftLinksActive[i]].materiaType == Materia.MateriaType.Support)
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaSupLL;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[leftLinksActive[i]].materiaType == Materia.MateriaType.Personal)
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaPersLL;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[leftLinksActive[i]].materiaType == Materia.MateriaType.Independant)
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaStandLL;
                }
            }
            if (equipToCheck == 2)
            {
                if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[leftLinksActive[i]].name == "Empty")
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaEmptyLL;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[leftLinksActive[i]].materiaType == Materia.MateriaType.Magic)
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaMagLL;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[leftLinksActive[i]].materiaType == Materia.MateriaType.Skill)
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaSkillLL;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[leftLinksActive[i]].materiaType == Materia.MateriaType.Support)
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaSupLL;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[leftLinksActive[i]].materiaType == Materia.MateriaType.Personal)
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaPersLL;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[leftLinksActive[i]].materiaType == Materia.MateriaType.Independant)
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaStandLL;
                }
            }
            if (equipToCheck == 3)
            {
                if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[leftLinksActive[i]].name == "Empty")
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaEmptyLL;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[leftLinksActive[i]].materiaType == Materia.MateriaType.Magic)
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaMagLL;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[leftLinksActive[i]].materiaType == Materia.MateriaType.Skill)
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaSkillLL;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[leftLinksActive[i]].materiaType == Materia.MateriaType.Support)
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaSupLL;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[leftLinksActive[i]].materiaType == Materia.MateriaType.Personal)
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaPersLL;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[leftLinksActive[i]].materiaType == Materia.MateriaType.Independant)
                {
                    singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaStandLL;
                }
            }
        }

        for (int i = 0; i < rightLinksActive.Length; i++)
        {
            if (equipToCheck == 1)
            {
                if (GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[rightLinksActive[i]].name == "Empty")
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaEmptyLR;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[rightLinksActive[i]].materiaType == Materia.MateriaType.Magic)
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaMagLR;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[rightLinksActive[i]].materiaType == Materia.MateriaType.Skill)
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaSkillLR;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[rightLinksActive[i]].materiaType == Materia.MateriaType.Support)
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaSupLR;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[rightLinksActive[i]].materiaType == Materia.MateriaType.Personal)
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaPersLR;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].weaponEquippedMateria[rightLinksActive[i]].materiaType == Materia.MateriaType.Independant)
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaStandLR;
                }

            }
            if (equipToCheck == 2)
            {
                if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[rightLinksActive[i]].name == "Empty")
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaEmptyLR;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[rightLinksActive[i]].materiaType == Materia.MateriaType.Magic)
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaMagLR;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[rightLinksActive[i]].materiaType == Materia.MateriaType.Skill)
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaSkillLR;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[rightLinksActive[i]].materiaType == Materia.MateriaType.Support)
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaSupLR;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[rightLinksActive[i]].materiaType == Materia.MateriaType.Personal)
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaPersLR;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].armorEquippedMateria[rightLinksActive[i]].materiaType == Materia.MateriaType.Independant)
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaStandLR;
                }
            }
            if (equipToCheck == 3)
            {
                if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[rightLinksActive[i]].name == "Empty")
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaEmptyLR;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[rightLinksActive[i]].materiaType == Materia.MateriaType.Magic)
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaMagLR;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[rightLinksActive[i]].materiaType == Materia.MateriaType.Skill)
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaSkillLR;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[rightLinksActive[i]].materiaType == Materia.MateriaType.Support)
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaSupLR;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[rightLinksActive[i]].materiaType == Materia.MateriaType.Personal)
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaPersLR;
                }
                else if (GameMenus.instance.charStatHolders[playerToCheck].accEquippedMateria[rightLinksActive[i]].materiaType == Materia.MateriaType.Independant)
                {
                    singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
                    singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaStandLR;
                }
            }
        }
    }

    public void HideAllSlots()
    {
        for (int i = 0; i < singleSlotsGameObjects.Length; i++)
        {
            singleSlotsGameObjects[i].SetActive(false);
        }
    }

    public void DisplayEmptySlots(int[] singleSlotsActive, int[] leftLinksActive, int[] rightLinksActive)
    {
        HideAllSlots();
        for (int i = 0; i < singleSlotsActive.Length; i++)
        {
            singleSlotsGameObjects[singleSlotsActive[i]].SetActive(true);
            singleSlotSpriteRenderers[singleSlotsActive[i]].sprite = InventoryUI.instance.materiaEmpty;


        }
        for (int i = 0; i < leftLinksActive.Length; i++)
        {
            singleSlotsGameObjects[leftLinksActive[i]].SetActive(true);
            singleSlotSpriteRenderers[leftLinksActive[i]].sprite = InventoryUI.instance.materiaEmptyLL;
        }

        for (int i = 0; i < rightLinksActive.Length; i++)
        {
            singleSlotsGameObjects[rightLinksActive[i]].SetActive(true);
            singleSlotSpriteRenderers[rightLinksActive[i]].sprite = InventoryUI.instance.materiaEmptyLR;
        }
    }


}
