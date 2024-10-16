#include "LightManager.h"

#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*									 Main
////////////////////////////////////////////////////////////////////////////////*/
void LightManager::Init() {

	// ConstBuffer初期化
	light_.Init();
}
void LightManager::Update() {

	// ConstBuffer転送
	light_.Update();
}

void LightManager::ImGui() {

	if (ImGui::BeginTabBar("Light Tabs")) {
		// Directional Light
		if (ImGui::BeginTabItem("Directional")) {
			ImGui::ColorEdit3("Color", &light_.directional.color.x);
			ImGui::DragFloat3("Direction", &light_.directional.direction.x, 0.01f);
			ImGui::DragFloat("Intensity", &light_.directional.intensity, 0.01f);
			ImGui::EndTabItem();
		}

		// Spot Light
		if (ImGui::BeginTabItem("Spot")) {
			ImGui::ColorEdit3("Color", &light_.spot.color.x);
			ImGui::DragFloat3("Pos", &light_.spot.pos.x, 0.01f);
			ImGui::DragFloat3("Direction", &light_.spot.direction.x, 0.01f);
			ImGui::DragFloat("Distance", &light_.spot.distance, 0.01f);
			ImGui::DragFloat("Decay", &light_.spot.decay, 0.01f);
			ImGui::DragFloat("CosAngle", &light_.spot.cosAngle, 0.01f);
			ImGui::DragFloat("CosFalloffStart", &light_.spot.cosFalloffStart, 0.01f);
			ImGui::EndTabItem();
		}

		// Point Light
		if (ImGui::BeginTabItem("Point")) {
			ImGui::ColorEdit3("Color", &light_.point.color.x);
			ImGui::DragFloat3("Pos", &light_.point.pos.x, 0.01f);
			ImGui::DragFloat("Intensity", &light_.point.intensity, 0.01f);
			ImGui::DragFloat("Radius", &light_.point.radius, 0.01f);
			ImGui::DragFloat("Decay", &light_.point.decay, 0.01f);
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*									 Getter
////////////////////////////////////////////////////////////////////////////////*/
LightObject LightManager::GetLightBuffer() const { return light_; }