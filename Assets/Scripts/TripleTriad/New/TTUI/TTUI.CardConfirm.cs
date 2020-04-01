 using System.Collections;
using System.Collections.Generic;
using UnityEngine;

 namespace ETF
 {
     public partial class TTUI
     {

         [Header("Card Confirm - UI References")] [SerializeField]
         Transform[] cardConfirmFingerLocations;

         [SerializeField] GameObject CardConfirmCanvas;

         public int ReturnCardConfirmFingerPosCount()
         {
             return cardConfirmFingerLocations.Length;
         }

         private IEnumerator PlayCardConfirmAnimator()
         {
             cardConfirmAnimator.Play("CardConfirmWindowDisplaying");
             yield return new WaitForSeconds(0.50f);
             isLoading = false;
         }

         public void KeepFingerOnProperLocationInCardConfirmation(int fingerPos)
         {
             //finger is turned on and off when in update method by the isloading bool
             if (isLoading == false)
             {
                 if (!fingerPrefab.gameObject.activeInHierarchy)
                 {
                     fingerPrefab.gameObject.SetActive(true);
                 }

                 fingerPrefab.transform.position = cardConfirmFingerLocations[fingerPos].transform.position;
             }
             else
             {
                 fingerPrefab.gameObject.SetActive(false);
             }
         }

         public void InitializeCardConfirmScreen()
         {
             isLoading = true;
             CardConfirmCanvas.SetActive(true);
             StartCoroutine(PlayCardConfirmAnimator());
         }

     }

 }