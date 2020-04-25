using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public class GameStartedState : TtState
	{

		#region Configuration

		[SerializeField] private TripleTriadManager _ttMan;

		#endregion

		public override void Startup(int additionalArgs = 0)
		{
			_ttMan.ttUi.InitializeAndCacheAllUiElementsForGameStart();
            
			CardInventory.instance
				.CreateUsableBattleCardLists(); //this only needs to be ran once, it generates all of the lists for card usage
			GameManager.instance.TurnOnDirectionalJoystick();
			_ttMan.ttDb.InitializeDbValuesForStartingTripleTriad();
			//SoundManager.instance.PlayIntroLoop(1);
			_ttMan.ttUi.StartGameFadeIn();

		}

		#region Functions



		#endregion
	}
}
