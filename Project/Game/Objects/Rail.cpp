
#include "Rail.h"

#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Rail classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Rail::Init(const std::string& modelName, const std::string& textureName) {

	BaseGameObject::Init(modelName);
	model_->SetTexture(textureName);
	model_->SetPipelineType(PipelineType::RailNoneBackCulling);

	color_ = { 0.498f,0.498f ,0.498f ,1.0f };

	SetName(modelName);
}

void Rail::Update(const Matrix4x4& viewPro) {

	BaseGameObject::Update(viewPro);
}

void Rail::Draw() {

	BaseGameObject::Draw();
}
