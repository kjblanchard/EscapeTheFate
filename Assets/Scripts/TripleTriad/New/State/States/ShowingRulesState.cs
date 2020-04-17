using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public class ShowingRulesState: TtState
	{

		#region Configuration

		[SerializeField] private TripleTriadManager _ttMan;

		#endregion


		public override void Startup(int additionalArgs = 0)
		{
			_ttMan.ttDb.InitializeShowingRulesDbValues();
			_ttMan.ttUi.InitializeShowingRulesUi();
			SoundManager.instance.PlayIntroLoop(7);
		}

		public override void Execute()
		{
			ListenForUserInputs();
			_ttMan.ttUi.KeepFingerOnProperLocationInShowingRules();
		}

		
		#region Functions

		private void ListenForUserInputs()
		{
			if ((Input.GetKeyDown(KeyCode.D) || Input.GetButtonDown("right")) &&
			    _ttMan.ttLogic.CanIScrollRightOnRuleSelection())
			{
				_ttMan.ttDb.ScrollRightInShowingRulesDb();
				SoundManager.instance.PlaySFX(0);
			}
			else if ((Input.GetKeyDown(KeyCode.A) || Input.GetButtonDown("left")) &&
			         _ttMan.ttLogic.CanIScrollLeftOnRuleSelection())
			{
				_ttMan.ttDb.ScrollLeftInShowingRulesDb();
				SoundManager.instance.PlaySFX(0);

			}
			else if ((Input.GetKeyDown(KeyCode.C) || Input.GetButtonDown("Fire2")) &&
			         _ttMan.ttLogic.CanIPressCancelOnRuleSelection())
			{
				CancelButtonInShowingRules();
			}
			else if ((Input.GetKeyDown(KeyCode.Space) || Input.GetButtonDown("Fire1")) &&
			         _ttMan.ttDb.ReturnFingerPositionInShowingRules() == 0)

			{
				StartCoroutine(ChangeToCardSelectionScreen());
			}
			else if ((Input.GetKeyDown(KeyCode.Space) || Input.GetButtonDown("Fire1")))
			{

			}
		}

		private void CancelButtonInShowingRules()
        {
            _ttMan.ttDb.CancelButtonInShowingRules();
            SoundManager.instance.PlaySFX(1);
        }

		private IEnumerator ChangeToCardSelectionScreen()
		{
			_ttMan.ttUi.isLoading = true;
			_ttMan.ttUi.showingRulesAnimator.SetTrigger("closing");
			yield return new WaitForSeconds(0.5f);
			_ttMan.SendStateChange(_ttMan.cardSelectionState);

		}

		#endregion
	}
}
