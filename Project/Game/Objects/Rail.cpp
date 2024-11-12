#include "Rail.h"

#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Rail classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Rail::Init(const std::string& modelName, const std::string& textureName) {

	BaseGameObject::Init(modelName);
	model_->SetTexture(textureName);
	model_->SetPipelineType(PipelineType::RailNoneBackCulling);

	color_ = { 0.64f,0.64f ,0.64f ,1.0f };

	SetName(modelName);
}

void Rail::Update(const Matrix4x4& viewPro) {

	BaseGameObject::Update(viewPro);
}

void Rail::Draw() {

	BaseGameObject::Draw();
}