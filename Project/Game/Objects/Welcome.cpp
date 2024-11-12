#include "Welcome.h"

#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Welcome classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Welcome::Init() {

	BaseGameObject::Init("welcome.obj");

	transform_.translation = Vector3(0.0f, -1.69f, 33.2f);
	//transform_.rotation = Quaternion::EulerToQuaternion(Vector3(0.0f, std::numbers::pi_v<float>, 0.0f));

	color_ = Vector4(0.4118f, 0.4118f, 0.4118f, 1.0f);

}

void Welcome::Update() {

	BaseGameObject::Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());
}

void Welcome::Draw() {

	BaseGameObject::Draw();
}