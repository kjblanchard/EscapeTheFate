using System;
using System.Collections;
using System.Collections.Generic;
using ETF.Battle;
using UnityEngine;

namespace ETF.Battle
{

	public class MoveTimeCalculator : MonoBehaviour
	{

		#region Configuration
		[Header("Script References")]
		[SerializeField] private TimeKeeper _timeKeeper;
		[SerializeField] private BattleUi _battleUi;
		
		private List<TempTime> tempMoveTimeCalculatorList = new List<TempTime>();

		#endregion



		#region Functions

		public void CalculateNextFiveTurnsForMoveCalculationOn(int whichPositionIsTheCharacterToModify, float skillSpeedModifier = 1)
		{
			CopyTimekeepersList();
			ModifyNextTurnsSpeed(whichPositionIsTheCharacterToModify, skillSpeedModifier);
			ModifyAllTurnsSpeed(whichPositionIsTheCharacterToModify, skillSpeedModifier);
			tempMoveTimeCalculatorList.Sort();
			UpdateUiDisplayForTenTurns();
			tempMoveTimeCalculatorList.Clear();
			GC.Collect();
		}

		private void UpdateUiDisplayForTenTurns()
		{
			for (int i = 0; i < 10; i++)
			{
				_battleUi.UpdateImageInTurnMoveCalcList(i,
					_timeKeeper.allBattlePositions[tempMoveTimeCalculatorList[i].RetrievePosition()].positionStats
						.characterPortrait);
			}
		}

		private void ModifyAllTurnsSpeed(int whichPositionIsTheCharacterToModify, float skillSpeedModifier)
		{//future use, for haste etc
			var isUsingHaste = false;
			if (isUsingHaste)
			{
				for (int i = 1; i < tempMoveTimeCalculatorList.Count; i++)
				{
					if (tempMoveTimeCalculatorList[i].RetrievePosition() == whichPositionIsTheCharacterToModify)
					{
						//modify the speed value by the skill speed modifier
						tempMoveTimeCalculatorList[i].ModifySpeed(skillSpeedModifier);
					}
				}
			}
		}

		private void ModifyNextTurnsSpeed(int whichPositionIsTheCharacterToModify, float skillSpeedModifier)
		{
			for (int i = 1; i < tempMoveTimeCalculatorList.Count; i++)
			{
				if (tempMoveTimeCalculatorList[i].RetrievePosition() != whichPositionIsTheCharacterToModify) continue;
				tempMoveTimeCalculatorList[i].ModifySpeed(skillSpeedModifier);
				break;
			}
		}

		private void CopyTimekeepersList()
		{
			for (int i = 0; i < _timeKeeper.turnOrderList.Count; i++)
			{
				var tempTimeReference = _timeKeeper.turnOrderList[i];
				tempMoveTimeCalculatorList.Add(new TempTime(tempTimeReference.RetrievePosition(),
					tempTimeReference.RetrieveSpeed()));
			}
		}

		#endregion
	}
}
