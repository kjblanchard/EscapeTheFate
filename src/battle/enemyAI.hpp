#pragma once
#include <string>
#include <vector>

namespace Etf {

class Battler;
struct BattlerData;

struct AIAction {
	int AbilityID = -1;
	Battler* Target = nullptr;
};

class EnemyAIStrategy {
   public:
	virtual ~EnemyAIStrategy() = default;
	virtual AIAction SelectAction(Battler* self, const std::vector<Battler*>& allies, const std::vector<Battler*>& enemies) = 0;
};

class BasicEnemyAI : public EnemyAIStrategy {
   public:
	AIAction SelectAction(Battler* self, const std::vector<Battler*>& allies, const std::vector<Battler*>& enemies) override;
};

EnemyAIStrategy* CreateEnemyAI(const std::string& name);

}  // namespace Etf
