
#pragma once
#include <string>
#include <vector>
namespace Etf {
struct BattleZoneData {
	int ID;
	float EncounterTime;
	std::vector<int> BattleGroups;
	std::vector<std::string> Maps;
};

}  // namespace Etf
