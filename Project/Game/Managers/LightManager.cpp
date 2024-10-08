#include "LightManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*									 Main
////////////////////////////////////////////////////////////////////////////////*/
void LightManager::Initialize() {

	// ConstBuffer初期化
	light_.Initialize();
}
void LightManager::Update() {

	// ConstBuffer転送
	light_.Update();
}

/*////////////////////////////////////////////////////////////////////////////////
*									 Getter
////////////////////////////////////////////////////////////////////////////////*/
LightObject LightManager::GetLightBuffer() const { return light_; }