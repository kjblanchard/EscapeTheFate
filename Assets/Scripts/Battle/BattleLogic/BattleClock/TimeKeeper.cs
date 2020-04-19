using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.Battle
{

	public class TimeKeeper : MonoBehaviour
	{

		#region Configuration

		private BattleUi _battleUi;
		
		public PositionHolder[] allBattlePositions;
		public int maxClockValue = 256;
		public List<TempTime> turnOrderList = new List<TempTime>();
		
		public List<int> tempFiveTurnList = new List<int>();


		#endregion

		void Awake()
		{
			_battleUi = FindObjectOfType<BattleUi>();
		}
		
		void Start()
		{
			
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
		}

		#region Functions
		
		public void DecideInitialTickValues()
		{
			var speedAttribute = 0;
			var randomNumberHigh = 0;
			for (int i = 0; i < allBattlePositions.Length; i++)
			{
				speedAttribute = allBattlePositions[i].positionStats.currentSpeed;
				randomNumberHigh = maxClockValue - speedAttribute;
				allBattlePositions[i].currentSpotInClock =  Random.Range(1, randomNumberHigh);
				
			}
			print("calculated order");
		}
		
		public void CalculateNextFiveTurnsForEachPlayerBattleStart()
		{
			//clear list
			turnOrderList.Clear();
			//add Initial Values, this is only for the battle start
			for (int i = 0; i < allBattlePositions.Length; i++)
			{
				var newTurnOrder = new TempTime(i,allBattlePositions[i].currentSpotInClock);
				turnOrderList.Add(newTurnOrder);
			}
			
			//this will callculate the next 5 turns for each player
			for (int i = 0; i < allBattlePositions.Length; i++)
			{
				//grab their current clock speed for each player
				var currentClock = allBattlePositions[i].currentSpotInClock;
				//calculate the players regular speed value when using a regular attack
				var playerFullSpeedBar = (maxClockValue - allBattlePositions[i].positionStats.currentSpeed) / 2;
				for (int j = 0; j < 5; j++)
				{
					//calculate the next 5 turns if he were to use regular attacks
					var newClock = currentClock + playerFullSpeedBar;
					currentClock = newClock;
					var newTurnOrder = new TempTime(i,newClock);	
					turnOrderList.Add(newTurnOrder);
				}

			}
			//after all players have been calculated, sort them
			turnOrderList.Sort();
			
			//display the next 10 turns in the turn bar
			for (int i = 0; i < 10; i++)
			{
				print($"The sorted Players turn number {i} is going to be {allBattlePositions[turnOrderList[i].RetrievePosition()].positionStats.characterName} cause his value is {turnOrderList[i].RetrieveSpeed()}");
				_battleUi.UpdateImageInTurnList(i,allBattlePositions[turnOrderList[i].RetrievePosition()].positionStats.characterPortrait);
			}
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

#endregion
