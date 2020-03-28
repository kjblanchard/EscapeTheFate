using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TtEnemyHandSelectionProcessor : MonoBehaviour
{
    [SerializeField] TTDB ttDb;
    [SerializeField] Text enemyHandTextBox;
    [SerializeField] private string whatEnemyGuyShouldSayWhenChoosingCards;
    public void EnemyHandSelectionRoutine()
    {
        ttDb.GenerateEnemyCardHand();
    }

    public void changeTheTextToSelectingTurn()
    {
        enemyHandTextBox.text = whatEnemyGuyShouldSayWhenChoosingCards;
    }
}
