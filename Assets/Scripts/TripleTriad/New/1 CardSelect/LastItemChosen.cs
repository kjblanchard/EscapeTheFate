using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LastItemChosen : MonoBehaviour
{
    public readonly int pageNum;
    public readonly  int spotOnPage;
    public readonly int spotInCardInv;



    public LastItemChosen(int pageNum, int spotOnPage, int spotInCardInv)
    {
        this.pageNum = pageNum;
        this.spotOnPage = spotOnPage;
        this.spotInCardInv = spotInCardInv;
    }

}
