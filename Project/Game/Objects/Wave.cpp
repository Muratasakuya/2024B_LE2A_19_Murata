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

	for (auto& material : materials_) {

		material.properties.enableLighting = true;
		material.properties.enableHalfLambert = true;
	}
}

void Wave::Update() {

	BaseModel::Update();
}

void Wave::Draw() {

	BaseModel::Draw();
}

void Wave::ImGui() {
#ifdef _DEBUG
	ImGui::Begin("Wave");
	ImGui::End();
#endif // _DEBUG
}
