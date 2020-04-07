using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ETF.TripleTriad
{

	public class TurnController : MonoBehaviour
	{

		#region Configuration

		[SerializeField] private TripleTriadManager _ttMan;
		[SerializeField] private Animator fingerAnimator;

		#endregion

		void Awake()
		{
			
		}
		
		void Start()
		{
			
		}

		
		void Update()
		{
			
		}

		#region Functions

		private void ChangeToEnemyTurn()
		{
			_ttMan.SendStateChange(_ttMan.enemyTurnState);
		}

		public void PlayChangeToEnemyTurnAnimation()
		{
			fingerAnimator.SetTrigger("PlayerToEnemy");
		}

		private void ChangeFromPlayerToEnemyState()
		{
			_ttMan.ttUi.InitializeBetweenTurnUiPlayerToEnemy();
		}

		#endregion
	}
}
