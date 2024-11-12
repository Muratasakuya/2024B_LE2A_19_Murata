#include "Ground.h"

#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Ground classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Ground::Init() {

	BaseGameObject::Init("ground.obj");

	BaseGameObject::SetLightingEnable(false);
	BaseGameObject::SetUVScale({ 128.0f,128.0f,0.0f });

	transform_.translation.y = -33.5f;

	SetName("ground");

}

void Ground::Update() {

	BaseGameObject::Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());
}

void Ground::Draw() {

	BaseGameObject::Draw();
}