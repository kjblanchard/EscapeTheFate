using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.Battle
{

	public class TempTime : IComparable<TempTime>
	{
		private int _currentSpeed;
		private int _currentPosition;

		#region Configuration

		public TempTime(int currentPosition,int currentSpeed)
		{
			_currentPosition = currentPosition;
			_currentSpeed = currentSpeed	;

		}
		
		public int CompareTo(TempTime compareTime)
		{       // A null value means that this object is greater.
			if (compareTime == null){
				return 1;  
			}
			else {
				return _currentSpeed.CompareTo(compareTime._currentSpeed);
			}
		}
		

		#endregion


		#region Functions
		
		
		public int RetrievePosition()
		{
			return _currentPosition;
		}

		public int RetrieveSpeed()
		{
			return _currentSpeed;
		}

		public int ModifySpeed(float skillSpeedModifier = 1.0f)
		{
			var goalSpeed = (int) (_currentSpeed * skillSpeedModifier);
			var clockDifference = (int) (_currentSpeed - _currentSpeed * skillSpeedModifier);

				_currentSpeed = goalSpeed;
				return clockDifference;
				
		}

		public void ModifySpeedByValue(int clockTimeToModifyBy)
		{
			_currentSpeed += clockTimeToModifyBy;
		}

		#endregion
	}
}
