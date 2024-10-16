#include "BaseModel.h"

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

	waveBuffer_.Init();
}
void BaseModel::Update() {

	transform_.Update();
	for (auto& material : materials_) {

		material.Update();
	}
	waveBuffer_.Update();
}
void BaseModel::Draw(BlendMode blendMode) {

	model_->DrawWave(transform_, materials_, waveBuffer_, blendMode);
}
