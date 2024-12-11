#include "BaseAnimationObject.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*	BaseAnimationObject ClassMethods
////////////////////////////////////////////////////////////////////////////////*/

void BaseAnimationObject::Init(const std::string& modelName, const std::string& animationName) {

	model_ = std::make_unique<AnimationModel>();
	model_->Init(modelName, animationName);

	transform_.Init(modelName, animationName);
	color_ = Color::White();

	materials_.emplace_back();
	for (auto& material : materials_) {

		material.Init();
		material.color = color_;
		material.properties.enableLighting = true;
		material.properties.enableHalfLambert = true;
	}
}

void BaseAnimationObject::Update() {

	transform_.Update();
	for (auto& material : materials_) {

		material.color = color_;
		material.Update();
	}
}

void BaseAnimationObject::Draw(BlendMode blendMode) {

	model_->Draw(transform_, materials_.front(), blendMode);
}

void BaseAnimationObject::TransformImGui() {

	if (ImGui::TreeNode("Transform")) {

		if (ImGui::Button("Save")) {
			IBaseGameObject::SaveJsonForTransform(transform_);
		}

		ImGui::DragFloat3("Scale", &transform_.scale.x, 0.01f);
		ImGui::DragFloat3("Translate", &transform_.translation.x, 0.01f);

		ImGui::TreePop();
	}
}

void BaseAnimationObject::SetAnimation(const std::string& animationName, bool play) {

	transform_.SetPlayAnimation(play, animationName);
	model_->SetAnimationName(animationName);
}

void BaseAnimationObject::SetWorldTransform(const AnimationTransform& transform) {

	transform_.translation = transform.translation;
	transform_.scale = transform.scale;
	transform_.rotation = transform.rotation;
}
