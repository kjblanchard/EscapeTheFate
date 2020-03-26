using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MasterEquipmentList : MonoBehaviour
{

    public static MasterEquipmentList instance;

    public Equipment[] totalEquipmentListing;

    private void Awake()
    {
        instance = this;
        int equipmentListCount =  FindObjectsOfType<MasterEquipmentList>().Length;
        if(equipmentListCount > 1)
        {
            Destroy(gameObject);
        }
        else
        {
            DontDestroyOnLoad(gameObject);
        }
    }
}
