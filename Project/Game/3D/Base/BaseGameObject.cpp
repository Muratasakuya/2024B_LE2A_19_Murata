#include "BaseGameObject.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

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
}

void BaseGameObject::Update(const Matrix4x4& viewPro) {

	transform_.Update(viewPro);
	for (auto& material : materials_) {

		material.color = color_;
		material.Update();
	}
}

void BaseGameObject::Draw(BlendMode blendMode) {

	model_->Draw(transform_, materials_, blendMode);
}

void BaseGameObject::ImGui() {
#ifdef _DEBUG

	//* PtrAdress ++index *//

	std::string materialHeader;
	if (materials_.size() == 1) {
		materialHeader = "Material";
	} else {
		materialHeader = "Materials";
	}

	if (ImGui::TreeNode(materialHeader.c_str())) {
		for (size_t i = 0; i < materials_.size(); ++i) {
			ImGui::PushID(static_cast<int>(i));

			std::string materialLabel;
			if (materials_.size() == 1) {
				materialLabel = "Material";
			} else {
				materialLabel = "Material " + std::to_string(i);
			}

			materialLabel += "##" + std::to_string(reinterpret_cast<uintptr_t>(&materials_[i]));
			if (ImGui::TreeNode(materialLabel.c_str())) {
				ImGui::ColorEdit4("Color", &color_.x);
				ImGui::TreePop();
			}

			ImGui::PopID();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Transform")) {

		ImGui::DragFloat3(("Translate##" + std::to_string(reinterpret_cast<uintptr_t>(this))).c_str(), &transform_.translation.x, 0.01f);
		ImGui::DragFloat3(("Rotate##" + std::to_string(reinterpret_cast<uintptr_t>(this))).c_str(), &transform_.rotation.x, 0.01f);
		ImGui::DragFloat3(("Scale##" + std::to_string(reinterpret_cast<uintptr_t>(this))).c_str(), &transform_.scale.x, 0.01f);
		ImGui::TreePop();
	}

	ImGui::Separator();
#endif // _DEBUG
}

void BaseGameObject::SetName(const std::string& name) {
	name_ = name;
	NewMoonGame::SetToImGui(this);
}

void BaseGameObject::SetLightingEnable(bool enable) {
	for (auto& material : materials_) {
		material.properties.enableLighting = enable;
	}
}

std::string BaseGameObject::GetName() const {
	return name_;
}
