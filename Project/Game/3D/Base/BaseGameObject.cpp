#include "BaseGameObject.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

#include "Lib/Adapter/JsonAdapter.h"

/*////////////////////////////////////////////////////////////////////////////////
*						BaseModel classMethods
////////////////////////////////////////////////////////////////////////////////*/

void BaseGameObject::Init(const std::string& modelName) {

	model_ = std::make_unique<Model>();
	model_->Init(modelName);

	transform_.Init();
	color_.SetInit(1.0f);

	materials_.resize(model_->GetMeshNum());
	for (auto& material : materials_) {

		material.Init();
		material.color = color_;
		material.properties.enableLighting = true;
		material.properties.enableHalfLambert = true;
	}

	isAnimationModel_ = false;

	index_ = -1;

}

void BaseGameObject::Init(const std::string& modelName, const std::string& animationName) {

	animationModel_ = std::make_unique<AnimationModel>();
	animationModel_->Init(modelName, animationName);

	animationTransform_.Init(modelName, animationName);
	color_.SetInit(1.0f);

	materials_.emplace_back();
	for (auto& material : materials_) {

		material.Init();
		material.color = color_;
		material.properties.enableLighting = true;
		material.properties.enableHalfLambert = true;
	}

	isAnimationModel_ = true;

}

void BaseGameObject::Update() {

	transform_.Update();
	for (auto& material : materials_) {

		material.color = color_;
		material.Update();
	}
}

void BaseGameObject::UpdateAnimation() {

	animationTransform_.Update();
	for (auto& material : materials_) {

		material.color = color_;
		material.Update();
	}
}

void BaseGameObject::Draw(BlendMode blendMode) {

	model_->Draw(transform_, materials_, blendMode);
}

void BaseGameObject::DrawAnimation(BlendMode blendMode) {

	animationModel_->Draw(animationTransform_, materials_.front(), blendMode);
}

void BaseGameObject::ImGui() {
#ifdef _DEBUG

	uintptr_t ptrAddress = reinterpret_cast<uintptr_t>(this);

	std::string materialHeader;
	if (materials_.size() == 1) {
		materialHeader = std::format("Material##{}", ptrAddress);
	} else {
		materialHeader = std::format("Materials##{}", ptrAddress);
	}

	if (ImGui::TreeNode(materialHeader.c_str())) {

		if (ImGui::Button("Save")) {
			SaveJsonForColor();
		}
		for (size_t i = 0; i < materials_.size(); ++i) {
			ImGui::PushID(static_cast<int>(i));

			std::string materialLabel;
			if (materials_.size() == 1) {
				materialLabel = "Color";
			} else {
				materialLabel = "Color " + std::to_string(i);
			}

			materialLabel += "##" + std::to_string(reinterpret_cast<uintptr_t>(&materials_[i]));
			if (ImGui::TreeNode(materialLabel.c_str())) {
				ImGui::ColorEdit4("", &color_.x);
				ImGui::Text("R:%4.2f G:%4.2f B:%4.2f A:%4.2f", color_.x, color_.y, color_.z, color_.w);
				ImGui::TreePop();
			}

			ImGui::PopID();
		}
		ImGui::TreePop();
	}

	std::string transformLabel = std::format("Transform##{}", ptrAddress);
	if (ImGui::TreeNode(transformLabel.c_str())) {

		if (isAnimationModel_) {

			if (ImGui::Button("Save")) {
				SaveJsonForTransform(animationTransform_);
			}

			std::string translateLabel = std::format("Pos##{}", ptrAddress);
			std::string scaleLabel = std::format("Scale##{}", ptrAddress);
			ImGui::DragFloat3(scaleLabel.c_str(), &animationTransform_.scale.x, 0.01f);
			ImGui::DragFloat3(translateLabel.c_str(), &animationTransform_.translation.x, 0.01f);

		} else {

			if (ImGui::Button("Save")) {
				SaveJsonForTransform(transform_);
			}

			std::string translateLabel = std::format("Pos##{}", ptrAddress);
			std::string scaleLabel = std::format("Scale##{}", ptrAddress);
			ImGui::DragFloat3(scaleLabel.c_str(), &transform_.scale.x, 0.01f);
			ImGui::DragFloat3(translateLabel.c_str(), &transform_.translation.x, 0.01f);
		}

		ImGui::TreePop();
	}

	ImGui::Separator();
	DerivedImGui();
#endif // _DEBUG
}

void BaseGameObject::ApplyJsonForColor() {

	Json data = JsonAdapter::Load(GetName() + "Color.json");
	color_ = JsonAdapter::ToVector4(data["color"]);
}

void BaseGameObject::SaveJsonForColor() {

	Json data;
	data["color"] = JsonAdapter::FromVector4(color_);
	JsonAdapter::Save(GetName() + "Color.json", data);
}

void BaseGameObject::ApplyJsonForTransform(BaseTransform& transform) {

	Json data = JsonAdapter::Load(GetName() + "Transform.json");
	transform.translation = JsonAdapter::ToVector3(data["translation"]);
	transform.scale = JsonAdapter::ToVector3(data["scale"]);
}

void BaseGameObject::SaveJsonForTransform(const BaseTransform& transform) {

	Json data;
	data["translation"] = JsonAdapter::FromVector3(transform.translation);
	data["scale"] = JsonAdapter::FromVector3(transform.scale);
	JsonAdapter::Save(GetName() + "Transform.json", data);

}

void BaseGameObject::SetName(const std::string& name) {

	name_ = name;
	if (index_ != 0) {
		name_ = name_ + std::to_string(index_);
	}

	NewMoonGame::SetToImGui(this);
}

void BaseGameObject::SetAnimation(const std::string& animationName, bool play) {

	animationTransform_.SetPlayAnimation(play, animationName);
	animationModel_->SetAnimationName(animationName);
}

void BaseGameObject::SetUVTransform(const std::optional<Vector3>& scale,
	const std::optional<Vector3>& rotate, const std::optional<Vector3>& translate) {

	for (auto& material : materials_) {
		material.properties.SetUVTransform(scale, rotate, translate);
	}
}

void BaseGameObject::SetWorldTransform(const WorldTransform& transform) {

	transform_.translation = transform.translation;
	transform_.scale = transform.scale;
	transform_.eulerRotate_ = transform.eulerRotate_;
	transform_.rotation = Quaternion::EulerToQuaternion(transform_.eulerRotate_);
}

void BaseGameObject::SetLightingEnable(bool enable) {
	for (auto& material : materials_) {
		material.properties.enableLighting = enable;
	}
}

std::string BaseGameObject::GetName() const {
	return name_;
}

Vector3 BaseGameObject::GetWorldPos() const {
	return transform_.GetWorldPos();
}

const WorldTransform& BaseGameObject::GetWorldTransform() const {
	return transform_;
}