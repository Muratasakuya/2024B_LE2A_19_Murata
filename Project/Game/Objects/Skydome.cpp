#include "Skydome.h"

#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Skydome classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Skydome::Init() {

	BaseGameObject::Init("skydome.obj");
	
	SetName("skydome");
}

void Skydome::Update(const Matrix4x4& viewPro) {

	BaseGameObject::Update(viewPro);
}

void Skydome::Draw() {

	BaseGameObject::Draw();
}