#include "BaseGameObject.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*	BaseModel ClassMethods
////////////////////////////////////////////////////////////////////////////////*/

void BaseGameObject::Init(const std::string& modelName) {

	model_ = std::make_unique<Model>();
	model_->Init(modelName);

	transform_.Init();
	color_ = Color::White();

	materials_.resize(model_->GetMeshNum());
	for (auto& material : materials_) {

		material.Init();
		material.color = color_;
		material.properties.enableLighting = true;
		material.properties.enableHalfLambert = true;
	}
}

void BaseGameObject::Update() {

	transform_.Update();
	for (auto& material : materials_) {

		material.color = color_;
		material.Update();
	}
}

void BaseGameObject::Draw(BlendMode blendMode) {

	model_->Draw(transform_, materials_, blendMode);
}

void BaseGameObject::TransformImGui() {

	if (ImGui::TreeNode("Transform")) {

		if (ImGui::Button("Save")) {
			IBaseGameObject::SaveJsonForTransform(transform_);
		}

		ImGui::DragFloat3("Scale", &transform_.scale.x, 0.01f);
		ImGui::DragFloat3("Translate", &transform_.translation.x, 0.01f);

		ImGui::TreePop();
	}
}

void BaseGameObject::SetWorldTransform(const WorldTransform& transform) {

	transform_.translation = transform.translation;
	transform_.scale = transform.scale;
	transform_.rotation = transform.rotation;
}