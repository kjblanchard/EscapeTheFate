#pragma once
#include <string>
#include <ui/uiAnimation.hpp>
#include <ui/uiProgressBar.hpp>
namespace Etf {
class Battler;

class EnemyBattlerUI {
   public:
	EnemyBattlerUI(unsigned int enemyIndex, Battler* battler);
	~EnemyBattlerUI();
	void UpdateHP(float percent);
	void UpdateATBBar(float percent);
	void StartATBIdleAnim();
	void StartATBTurnAnim();
	void SetVisible(bool visible);

   private:
	UIProgressBar* _hpBar = nullptr;
	UIAnimation* _atbBarAnim = nullptr;
	UIProgressBar* _atbProgressBar = nullptr;
	std::string _hpBarName;
	std::string _atbBarName;
};

}  // namespace Etf
