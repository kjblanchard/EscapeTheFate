#include <Supergoon/camera.h>
#include <imgui.h>

#include <debug/DebugCamera.hpp>
using namespace Etf;
using namespace std;

void Etf::DisplayCameraTab() {
	if (ImGui::CollapsingHeader("Camera")) {
		static float* camPosX = nullptr;
		static float* camPosY = nullptr;
		CameraGetPositionHandle(&camPosX, &camPosY);
		ImGui::SliderFloat("CameraX", camPosX, 0, 1000);
		ImGui::SliderFloat("CameraY", camPosY, 0, 1000);
		static float* previousCamFollowX = nullptr;
		static float* previousCamFollowY = nullptr;
		float* camFollowX = nullptr;
		float* camFollowY = nullptr;
		CameraGetFollow(&camFollowX, &camFollowY);
		static auto cameraFollow = camFollowX && camFollowY;
		if (ImGui::Checkbox("IsFollowing", &cameraFollow)) {
			if (cameraFollow) {
				SetCameraFollowTarget(previousCamFollowX, previousCamFollowY);
			} else {
				previousCamFollowX = camFollowX;
				previousCamFollowY = camFollowY;
				ResetCameraFollow();
			}
		}
	}
}
