using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public partial class TTUI
	{

		#region Configuration

		[Header("Showing Rules Stuff")] [SerializeField]
		private Canvas _showingRulesCanvas;
		[SerializeField] private Transform[] _showingRulesFingerLocations;
		public Animator showingRulesAnimator;

		
		#endregion



		#region Functions

		public void InitializeShowingRulesUi()
		{
			_showingRulesCanvas.gameObject.SetActive(true);
			locationSelectionFinger.SetActive(false);
			playerSelectionFinger.SetActive(false);
			_enemyScoreText.gameObject.SetActive(false);
			_playerScoreText.gameObject.SetActive(false);
			turnIndicatorFingerAnimator.gameObject.SetActive(false);
		}


		public void KeepFingerOnProperLocationInShowingRules()
		{
		
			//finger is turned on and off when in update method by the isloading bool
			if (isLoading == false)
			{
				if (!fingerPrefab.gameObject.activeInHierarchy)
				{
					fingerPrefab.gameObject.SetActive(true);
				}

				fingerPrefab.transform.position =
					_showingRulesFingerLocations[ttdb.ReturnFingerPositionInShowingRules()].transform.position;
			}
			else
			{
				fingerPrefab.gameObject.SetActive(false);
			}	
		}

		public int ReturnShowingRulesFingerPositionCount()
		{
			return _showingRulesFingerLocations.Length;
		}
		

		#endregion
	}
}
