#include "Skydome.h"

#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Skydome classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Skydome::Init() {

	BaseGameObject::Init("skydome.obj");

	transform_.scale.SetInit(16.0f);

	color_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	BaseGameObject::SetLightingEnable(false);
	
	SetName("skydome");
}

void Skydome::Update(const Matrix4x4& viewPro) {

	BaseGameObject::Update(viewPro);
}

void Skydome::Draw() {

	BaseGameObject::Draw();
}