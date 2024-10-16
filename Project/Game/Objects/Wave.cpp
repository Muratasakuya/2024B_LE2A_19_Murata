#include "Wave.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							Model classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Wave::Init() {

	const std::string modelName = "wavePlane.obj";
	NewMoonGame::LoadModel("./Resources/Obj", modelName);
	const std::string blueTexture = "waveBlue";
	const std::string whiteTexture = "waveWhite2";
	const std::string baseWhite = "waveBase";
	NewMoonGame::LoadTexture(baseWhite);
	NewMoonGame::LoadTexture(whiteTexture);
	NewMoonGame::LoadTexture(blueTexture);

	BaseModel::Init(modelName);

	BaseModel::waveBuffer_.properties.time = 0.0f;
	BaseModel::waveBuffer_.properties.amplitude = 0.05f;
	BaseModel::waveBuffer_.properties.frequency = 0.175f;
	BaseModel::waveBuffer_.properties.speed = 0.2f;
	BaseModel::waveBuffer_.properties.wavelength = 0.65f;

	for (auto& material : materials_) {

		material.properties.enableLighting = true;
		material.properties.enableHalfLambert = true;
	}
}

void Wave::Update() {

	BaseModel::waveBuffer_.properties.time += NewMoonGame::GetDeltaTime() * BaseModel::waveBuffer_.properties.speed;

	BaseModel::Update();
}

void Wave::Draw() {

	BaseModel::Draw();
}

void Wave::ImGui() {
#ifdef _DEBUG
	ImGui::Begin("Wave");

	ImGui::ColorEdit4("Color", &BaseModel::materials_.front().color.x);
	ImGui::DragFloat3("Translate", &BaseModel::transform_.translation.x, 0.01f);
	ImGui::DragFloat("Amplitude", &BaseModel::waveBuffer_.properties.amplitude, 0.01f);
	ImGui::DragFloat("Frequency", &BaseModel::waveBuffer_.properties.frequency, 0.01f);
	ImGui::DragFloat("Speed", &BaseModel::waveBuffer_.properties.speed, 0.01f);
	ImGui::DragFloat("Qavelength", &BaseModel::waveBuffer_.properties.wavelength, 0.01f);

	ImGui::End();
#endif // _DEBUG
}
