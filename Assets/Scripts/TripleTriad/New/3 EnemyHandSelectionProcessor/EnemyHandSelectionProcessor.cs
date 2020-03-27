using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyHandSelectionProcessor : MonoBehaviour
{
    [SerializeField] TTDB ttdb;


    public void EnemyHandSelectionRoutine()
    {
        ttdb.GenerateEnemyCardHand();
    }
    
}
