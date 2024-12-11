#include "IBaseGameObject.h"

#include "Engine/Base/NewMoonGame.h"
#include "Lib/Adapter/JsonAdapter.h"
#include <imgui.h>

/*////////////////////////////////////////////////////////////////////////////////
*	IBaseGameObject ClassMethods
////////////////////////////////////////////////////////////////////////////////*/

IBaseGameObject::~IBaseGameObject() {
	NewMoonGame::EraseToImGui(this);
}

void IBaseGameObject::ImGui() {
#ifdef _DEBUG

	std::string materialHeader;
	if (materials_.size() == 1) {
		materialHeader = "Material";
	} else {
		materialHeader = "Materials";
	}

	if (ImGui::TreeNode(materialHeader.c_str())) {

		if (ImGui::Button("Save")) {
			SaveJsonForColor();
		}
		for (size_t i = 0; i < materials_.size(); ++i) {

			std::string materialLabel;
			if (materials_.size() == 1) {
				materialLabel = "Color";
			} else {
				materialLabel = "Color " + std::to_string(i);
			}

			if (ImGui::TreeNode(materialLabel.c_str())) {

				ImGui::ColorEdit4("", &color_.x);
				ImGui::Text("R:%4.2f G:%4.2f B:%4.2f A:%4.2f", color_.x, color_.y, color_.z, color_.w);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	TransformImGui();

	ImGui::Separator();
	DerivedImGui();

#endif // _DEBUG
}

void IBaseGameObject::ApplyJsonForColor() {

	std::string jsonPath = parentFolderName_.value_or("") + name_ + "Color.json";
	Json data = JsonAdapter::Load(jsonPath);

	color_ = JsonAdapter::ToVector4(data["color"]);
}

void IBaseGameObject::SaveJsonForColor() {

	Json data;
	data["color"] = JsonAdapter::FromVector4(color_);

	std::string jsonPath = parentFolderName_.value_or("") + name_ + "Color.json";
	JsonAdapter::Save(jsonPath, data);
}

void IBaseGameObject::ApplyJsonForTransform(BaseTransform& transform) {

	std::string jsonPath = parentFolderName_.value_or("") + name_ + "Transform.json";
	Json data = JsonAdapter::Load(jsonPath);

	transform.translation = JsonAdapter::ToVector3(data["translation"]);
	transform.scale = JsonAdapter::ToVector3(data["scale"]);
}

void IBaseGameObject::SaveJsonForTransform(const BaseTransform& transform) {

	Json data;
	data["translation"] = JsonAdapter::FromVector3(transform.translation);
	data["scale"] = JsonAdapter::FromVector3(transform.scale);

	std::string jsonPath = parentFolderName_.value_or("") + name_ + "Transform.json";
	JsonAdapter::Save(jsonPath, data);
}

void IBaseGameObject::SetMeshRenderer(const std::string& name, uint32_t index) {

	name_ = name;
	if (index != 0) {

		name_ = name_ + std::to_string(index);
	}

	NewMoonGame::SetToImGui(this);
}

void IBaseGameObject::SetUVTransform(
	const std::optional<Vector3>& scale, const std::optional<Vector3>& rotate, const std::optional<Vector3>& translate) {

	for (auto& material : materials_) {

		material.properties.SetUVTransform(scale, rotate, translate);
	}
}

void IBaseGameObject::SetLightingEnable(bool enable) {

	for (auto& material : materials_) {
		material.properties.enableLighting = enable;
	}
}