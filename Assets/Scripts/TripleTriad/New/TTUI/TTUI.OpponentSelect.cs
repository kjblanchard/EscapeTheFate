using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace ETF.TripleTriad
{

	public partial class TTUI
	{

		#region Configuration

		[Header("Opponent Selection Stuff")]
		[SerializeField] private Canvas _opponentSelectionCanvas;
		[SerializeField] private Image[] _opponentBoxImages;
		[SerializeField] private Animator[] _opponentAnimators;
		private static readonly int kRest = Animator.StringToHash("rest");
		private static readonly int kIdle = Animator.StringToHash("idle");
		private static readonly int kSelected = Animator.StringToHash("selected");


		[SerializeField] private Text _opponentNameText;
		[SerializeField] private Text _opponentDescriptionText;
		[SerializeField] private Text _isRandomText;
		[SerializeField] private Text _isOpenText;
		[SerializeField] private Text _rareCardText;
		[SerializeField] private Text _aiDifficultyText;


		#endregion


		#region Functions

		public void InitializeOpponentSelectionUi()
		{
			_opponentSelectionCanvas.enabled = true;
		}
		
		public void TurnOnProperBoxImage()
		{//probably make this do the right ones not all of them in the future
			for (int i = 0; i < _opponentBoxImages.Length; i++)
			{
				_opponentBoxImages[i].enabled = false;
			}

			_opponentBoxImages[ttdb.RetrieveOpponentSelectionCurrentValue()].enabled = true;

		}

		public void TurnOnProperAnimatorOpponentSelection()
		{
			for (int i = 0; i < _opponentAnimators.Length; i++)
			{
				_opponentAnimators[i].SetBool(kIdle,false);
			}
			_opponentAnimators[ttdb.RetrieveOpponentSelectionCurrentValue()].SetBool(kIdle,true);
		}

		public void SelectedAnimatorOpponentSelection()
		{
		_opponentAnimators[ttdb.RetrieveOpponentSelectionCurrentValue()].SetTrigger(kSelected);
		}

		public void StartShowingRulesFadeOut()
		{
			_tripleTriadUiFade.animator.SetTrigger("showingRulesFadeOut");
		}

		public void UpdateOpponentInfoOpponentSelection()
		{
			var enemyInfo = ttdb.RetrieveCurrentSelectedEnemyInfo();
			_opponentNameText.text = enemyInfo.RetrieveName();
			_opponentDescriptionText.text = enemyInfo.RetrieveDescription();
			_isOpenText.text = enemyInfo.RetrieveOpen();
			_aiDifficultyText.text = enemyInfo.RetrieveOpen();
			_rareCardText.text = enemyInfo.RetrieveRares();
			_isRandomText.text = enemyInfo.RetrieveRandom();


		}

		public void TurnOffOpponentSelectUi()
		{
			_opponentSelectionCanvas.enabled = false;
		}


		

		#endregion
	}
}
