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
						.characterPortrait,tempMoveTimeCalculatorList[i].RetrieveSpeed());
				//print($"The sorted Players turn number {i} is going to be {_timeKeeper.allBattlePositions[tempMoveTimeCalculatorList[i].RetrievePosition()].positionStats.characterName} cause his value is {tempMoveTimeCalculatorList[i].RetrieveSpeed()}");
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
			var whenToStartModifyingYourSpeed = 0;
			var whatToModifyClockBy = 0;

			for (int i = 1; i < tempMoveTimeCalculatorList.Count; i++)
			{
				if (tempMoveTimeCalculatorList[i].RetrievePosition() != whichPositionIsTheCharacterToModify) continue;
				whatToModifyClockBy =  tempMoveTimeCalculatorList[i].ModifySpeed(skillSpeedModifier);
				whatToModifyClockBy = whatToModifyClockBy * -1;
				//print($"{whatToModifyClockBy} is the number we are going to modify the rest of the turns by");
				whenToStartModifyingYourSpeed = i+1;
				break;
			}

			for (int i = whenToStartModifyingYourSpeed; i < tempMoveTimeCalculatorList.Count; i++)
			{
				if (tempMoveTimeCalculatorList[i].RetrievePosition() == whichPositionIsTheCharacterToModify)
				{
					tempMoveTimeCalculatorList[i].ModifySpeedByValue(whatToModifyClockBy);
				}
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
