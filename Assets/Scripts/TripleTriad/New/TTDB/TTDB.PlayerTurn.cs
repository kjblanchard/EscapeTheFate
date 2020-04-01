using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF
{

	public partial class TTDB
	{

		#region Configuration
		
		private int _playerTurnCurrentSelection;


		#endregion
		

		#region Functions

		public int RetrievePlayerTurnCurrentSelection()
		{
			return _playerTurnCurrentSelection;
		}

		public void MovePlayerTurnCurrentSelectionDown()
		{
			_playerTurnCurrentSelection++;
		}

		public void MovePlayerTurnCurrentSelectionUp()
		{
			_playerTurnCurrentSelection--;
		}

		public void MovePlayerTurnCurrentSelectionTop()
		{
			_playerTurnCurrentSelection = 0;
		}

		public void MovePlayerTurnCurrentSelectionToBottom()
		{
			_playerTurnCurrentSelection = _myHandTripleTriadCards.Length-1;
		}

		public void InitializePlayerTurnDbValues()
		{
			_playerTurnCurrentSelection = 0;
		}
		

		#endregion
	}
}
