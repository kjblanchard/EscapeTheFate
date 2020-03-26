using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MasterMateriaList : MonoBehaviour
{
    public static MasterMateriaList instance;

    public Materia[] totalMateriaList;

    private void Awake()
    {
        instance = this;
        int equipmentListCount = FindObjectsOfType<MasterMateriaList>().Length;
        if (equipmentListCount > 1)
        {
            Destroy(gameObject);
        }
        else
        {
            DontDestroyOnLoad(gameObject);
        }
    }
}
