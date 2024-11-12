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

	color_ = Vector4(0.16f, 0.16f, 0.16f, 1.0f);

}

void Rail::Update(const Matrix4x4& viewPro) {

	BaseGameObject::Update(viewPro);
}

void Rail::Draw() {

	BaseGameObject::Draw();
}