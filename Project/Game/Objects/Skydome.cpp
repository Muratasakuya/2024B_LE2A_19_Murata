#include "Skydome.h"

#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Skydome classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Skydome::Init() {

	const std::string modelName = "skydome.obj";
	NewMoonGame::LoadModel("./Resources/Obj/", modelName);

	BaseGameObject::Init(modelName);
	
	SetName("skydome");
}

void Skydome::Update(const Matrix4x4& viewPro) {

	BaseGameObject::Update(viewPro);
}

void Skydome::Draw() {

	BaseGameObject::Draw();
}