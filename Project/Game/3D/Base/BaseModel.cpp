#include "BaseModel.h"

#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*						BaseModel classMethods
////////////////////////////////////////////////////////////////////////////////*/

void BaseModel::Init(const std::string& modelName) {

	model_ = std::make_unique<Model>();
	model_->Init(modelName);

	transform_.Init();

	materials_.resize(model_->GetMeshNum());
	for (auto& material : materials_) {

		material.Init();
	}

}
void BaseModel::Update() {

	transform_.Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());
	for (auto& material : materials_) {

		material.Update();
	}
}
void BaseModel::Draw(BlendMode blendMode) {

	model_->Draw(transform_, materials_, blendMode);
}
