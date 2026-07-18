#include <imgui.h>

#include <debug/DebugEngine.hpp>
#include <engine.hpp>
using namespace Etf;
using namespace std;

extern CurrentSceneLoadingState currentLoadingState_;
extern string currentBGM_;

string getEnumName(CurrentSceneLoadingState state) {
	switch (state) {
		case Etf::CurrentSceneLoadingState::NotLoading:
			return "Not Loading";
		case Etf::CurrentSceneLoadingState::NextSceneQueued:
			return "Next scene Queued";
		case Etf::CurrentSceneLoadingState::WaitingForFadeOut:
			return "Waiting for fadeout";
		case Etf::CurrentSceneLoadingState::LoadingStart:
			return "Loading start";
		case Etf::CurrentSceneLoadingState::LoadingMap:
			return "Loading Map";
		case Etf::CurrentSceneLoadingState::LoadingGameObjects:
			return "Loading gameobjects";
		case Etf::CurrentSceneLoadingState::LoadingUI:
			return "Loading UI";
		case Etf::CurrentSceneLoadingState::LoadingDialog:
			return "Loading Dialog";
		case Etf::CurrentSceneLoadingState::LoadingFinish:
			return "Loading Finished";
		case Etf::CurrentSceneLoadingState::JustLoaded:
			return "Finished Loading";
		case Etf::CurrentSceneLoadingState::FadingIn:
			return "Fading in, updates blocked";
		case Etf::CurrentSceneLoadingState::FadingInAllowUpdate:
			return "Fading in updates allowed";
		case Etf::CurrentSceneLoadingState::Count:
			return "Not implemented";
	}
}

void Etf::DisplayEngineTab() {
#ifdef imgui
	if (ImGui::CollapsingHeader("Engine")) {
		ImGui::Text("Current loading state: %s", getEnumName(currentLoadingState_).c_str());
		auto loadPercent = (float)currentLoadingState_ / (float)static_cast<int>(CurrentSceneLoadingState::Count) * 100;
		loadPercent = loadPercent == 0 ? 100 : loadPercent;
		ImGui::Text("Current Load percent %f", (double)loadPercent);
	}
#else
	return;
#endif
}
