﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public class TitleScreenState : TtState
	{

		#region Configuration

		[SerializeField] private TripleTriadManager _ttMan;
		private TTUI _ttui;

		#endregion

		public override void Startup(int additionalArgs = 0)
		{
			//SoundManager.instance.PlayBGM(12);
			//SoundManager.instance.CacheNextMusic(11);
			_ttui = _ttMan.ttUi;
			_ttui.InitializeTitleScreenUi();
		}

		public override void Execute()
		{
			ListenForUserInput();
		}

		public override void End()
		{
			_ttui.TurnOffTitleScreenUi();
		}

		#region Functions

		private void ListenForUserInput()
		{
			if (!_ttui.isLoading)
			{
				if (Input.anyKey)
				{
					_ttMan.SendStateChange(_ttMan.opponentSelectState);
				}
			}
		}

		#endregion
	}
}
