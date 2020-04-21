using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Random = UnityEngine.Random;

namespace ETF.Battle
{

	public class TimeKeeper : MonoBehaviour
	{

		#region Configuration

		[Header("Script References")]
		private BattleUi _battleUi;
		private MoveTimeCalculator _moveTimeCalculator;

		
		public PositionHolder[] allBattlePositions;
		private readonly int _maxClockValue = 1024;
		public List<TempTime> turnOrderList = new List<TempTime>();
		


		#endregion

		void Awake()
		{
			_battleUi = FindObjectOfType<BattleUi>();
			_moveTimeCalculator = FindObjectOfType<MoveTimeCalculator>();
		}

		void Start()
		{
			DecideInitialTickValues();
			CalculateNextFiveTurnsForEachPlayerBattleStart();
		}


		void Update()
		{
			if (Input.GetKeyDown(KeyCode.P))
			{
				DecideInitialTickValues();
			}

			if (Input.GetKeyDown(KeyCode.L))
			{
				CalculateNextFiveTurnsForEachPlayerBattleStart();
			}

			if (Input.GetKeyDown(KeyCode.O))
			{
				_moveTimeCalculator.CalculateNextFiveTurnsForMoveCalculationOn(0, 2f);
			}

			if (Input.GetKeyDown(KeyCode.I))
			{
				_moveTimeCalculator.CalculateNextFiveTurnsForMoveCalculationOn(0,0.3f);
			}


		}

		#region Functions



		private void DecideInitialTickValues()
		{
			for (int i = 0; i < allBattlePositions.Length; i++)
			{
				var playerLevel = allBattlePositions[i].positionStats.currentLevel;
				var randomNumber = Random.Range(0, playerLevel);
				print(randomNumber);
				var speedToStartWith = (_maxClockValue / 2) /
				                       (randomNumber + allBattlePositions[i].positionStats.currentSpeed);
				allBattlePositions[i].currentSpotInClock = speedToStartWith;
			}
			GC.Collect();
		}

		private void CalculateNextFiveTurnsForEachPlayerBattleStart()
		{
			
			turnOrderList.Clear();
			AddEachBattlerCurrentClockToList();
			CalcEachBattlerFiveTurns();
			turnOrderList.Sort();
			UpdateUiTurnSprites();
			GC.Collect();
		}

		private void CalcEachBattlerFiveTurns()
		{
			for (int i = 0; i < allBattlePositions.Length; i++)
			{
				var currentClock = allBattlePositions[i].currentSpotInClock;
				var battlersFullTurnLength = CalculateBattlerFullTurnLength(i);
				CalculateNextFiveTurnsForBattler(currentClock, battlersFullTurnLength, i);
			}
			GC.Collect();
		}

		private void CalculateNextFiveTurnsForBattler(int currentClock, int battlersFullTurnLength, int battlerPosToSearch)
		{
			for (int j = 0; j < 5; j++)
			{
				//calculate the next 5 turns if he were to use regular attacks
				var newClock = currentClock + battlersFullTurnLength;
				currentClock = newClock;
				var newTurnOrder = new TempTime(battlerPosToSearch, newClock);
				turnOrderList.Add(newTurnOrder);
			}
		}

		private void AddEachBattlerCurrentClockToList()
		{
			for (int i = 0; i < allBattlePositions.Length; i++)
			{
				var newTurnOrder = new TempTime(i, allBattlePositions[i].currentSpotInClock);
				turnOrderList.Add(newTurnOrder);
			}
		}

		private void UpdateUiTurnSprites()
		{
			for (int i = 0; i < 10; i++)
			{
				_battleUi.UpdateImageInTurnList(i,
					allBattlePositions[turnOrderList[i].RetrievePosition()].positionStats.characterPortrait,turnOrderList[i].RetrieveSpeed());
				//print($"The sorted Players turn number {i} is going to be {allBattlePositions[turnOrderList[i].RetrievePosition()].positionStats.characterName} cause his value is {turnOrderList[i].RetrieveSpeed()}");
			}
		}

		private int CalculateBattlerFullTurnLength(int i, float abilitySpeedOffset = 1)
		{
			return (int) (_maxClockValue / (allBattlePositions[i].positionStats.currentSpeed) *
			                        abilitySpeedOffset);
		}
		
		#endregion
	}

}

#region oldstuff

// public void CalculateTurnOrder()
// {
// turnOrderList.Clear();
// for (int i = 0; i < allBattlePositions.Length; i++)
// {
// 	var newTurnOrder = new TempTime(i,allBattlePositions[i].currentSpotInClock);
// 	turnOrderList.Add(newTurnOrder);
// }
//
// 	// for (int i = 0; i < turnOrderList.Count; i++)
// 	// {
// 	// 	print($"The unsorted Players turn is {allBattlePositions[turnOrderList[i].RetrievePosition()].positionStats.characterName} cause his value is {turnOrderList[i].RetrieveSpeed()}");
// 	// }
//
// 	turnOrderList.Sort();
// 	
// 	// for (int i = 0; i < turnOrderList.Count; i++)
// 	// {
// 	// 	print($"The sorted Players turn is {allBattlePositions[turnOrderList[i].RetrievePosition()].positionStats.characterName} cause his value is {turnOrderList[i].RetrieveSpeed()}");
// 	// }
// 	CalculateNextFiveTurnsForEachPlayer();
// 	
// 	turnOrderList.Sort();
// 	for (int i = 0; i < 10; i++)
// 	{
// 		print($"The sorted Players turn number {i} is going to be {allBattlePositions[turnOrderList[i].RetrievePosition()].positionStats.characterName} cause his value is {turnOrderList[i].RetrieveSpeed()}");
// 	}
//
// }

// public void DecideInitialTickValues()
// {
// 	var speedAttribute = 0;
// 	var randomNumberHigh = 0;
// 	for (int i = 0; i < allBattlePositions.Length; i++)
// 	{
// 		speedAttribute = allBattlePositions[i].positionStats.currentSpeed;
// 		randomNumberHigh = maxClockValue - speedAttribute;
// 		allBattlePositions[i].currentSpotInClock =  Random.Range(1, randomNumberHigh);
// 		
// 	}
// 	print("calculated order");
// }
// public void CalculateNextFiveTurnsForEachPlayerBattleStart()
// { //first attempt
// 	//clear list
// 	turnOrderList.Clear();
// 	//add Initial Values, this is only for the battle start
// 	for (int i = 0; i < allBattlePositions.Length; i++)
// 	{
// 		var newTurnOrder = new TempTime(i,allBattlePositions[i].currentSpotInClock);
// 		turnOrderList.Add(newTurnOrder);
// 	}
// 	
// 	//this will callculate the next 5 turns for each player
// 	for (int i = 0; i < allBattlePositions.Length; i++)
// 	{
// 		//grab their current clock speed for each player
// 		var currentClock = allBattlePositions[i].currentSpotInClock;
// 		//calculate the players regular speed value when using a regular attack
// 		var playerFullSpeedBar = (maxClockValue - allBattlePositions[i].positionStats.currentSpeed) / 2;
// 		for (int j = 0; j < 5; j++)
// 		{
// 			//calculate the next 5 turns if he were to use regular attacks
// 			var newClock = currentClock + playerFullSpeedBar;
// 			currentClock = newClock;
// 			var newTurnOrder = new TempTime(i,newClock);	
// 			turnOrderList.Add(newTurnOrder);
// 		}
//
// 	}
// 	//after all players have been calculated, sort them
// 	turnOrderList.Sort();
// 	
// 	//display the next 10 turns in the turn bar
// 	for (int i = 0; i < 10; i++)
// 	{
// 		print($"The sorted Players turn number {i} is going to be {allBattlePositions[turnOrderList[i].RetrievePosition()].positionStats.characterName} cause his value is {turnOrderList[i].RetrieveSpeed()}");
// 		_battleUi.UpdateImageInTurnList(i,allBattlePositions[turnOrderList[i].RetrievePosition()].positionStats.characterPortrait);
// 	}
// }


#endregion
