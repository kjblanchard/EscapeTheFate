#include <imgui.h>
#include <sgtools/log.h>

#include <debug/DebugSettings.hpp>
#include <gameConfig.hpp>
using namespace Etf;

void Etf::DisplaySettingsTab() {
#ifdef imgui
	if (ImGui::CollapsingHeader("Debug Settings")) {
		auto& config = GameConfig::GetGameConfig();
		ImGui::Checkbox("Interactions", &config.debug.interactions);
		ImGui::Checkbox("Map Exits", &config.debug.mapExits);
		ImGui::Checkbox("Battle Zones", &config.debug.battleZones);
	}
#else
	return;
#endif
}
