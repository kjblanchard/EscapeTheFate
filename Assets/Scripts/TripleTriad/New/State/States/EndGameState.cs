using System.Collections;
using System.Collections.Generic;
using ETF.TripleTriad;
using UnityEngine;

namespace ETF.TripleTriad
{

	public class EndGameState : TtState
	{

		#region Configuration

		[SerializeField] private TripleTriadManager _ttMan;	
		#endregion

		public override void Startup()
		{
			_ttMan.ttUi.InitializeEndGameUi();
			
		}

		#region Functions



		#endregion
	}
}
