#include "Cylinder.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"
#include "Lib/Adapter/JsonAdapter.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Cylinder classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Cylinder::Init(uint32_t index, const Vector3& pos, const Vector3& scale) {

	index;

	BaseGameObject::Init("cylinder.obj");

	transform_.translation = pos;
	transform_.scale = scale;
	// 0.5f固定
	transform_.scale.x = 0.5f;
	transform_.scale.z = 0.5f;

	color_= Vector4(0.4118f, 0.4118f, 0.4118f, 1.0f);

}

void Cylinder::Update() {

	BaseGameObject::Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());
}

void Cylinder::Draw() {

	BaseGameObject::Draw();
}

/*////////////////////////////////////////////////////////////////////////////////
*							CylinderCollection classMethods
////////////////////////////////////////////////////////////////////////////////*/

void CylinderCollection::Init() {

	index_ = 0;

	// Json適応
	ApplyJson();
}

void CylinderCollection::Update() {

	for (const auto& cylinder : cylinders_) {

		cylinder->Update();
	}
}

void CylinderCollection::Draw() {

	for (const auto& cylinder : cylinders_) {

		cylinder->Draw();
	}
}

void CylinderCollection::ImGui() {
#ifdef _DEBUG
	ImGui::Begin("CylinderCollection");

	if (ImGui::Button("AddCylinder")) {

		std::unique_ptr<Cylinder> newCylinder = std::make_unique<Cylinder>();

		Vector3 pos{};
		if (!cylinders_.empty()) {

			pos = cylinders_.back()->GetWorldPos();
		}

		newCylinder->Init(index_, pos, Vector3(0.5f, 1.0f, 0.5f));

		cylinders_.push_back(std::move(newCylinder));

		++index_;
	}

	if (ImGui::Button("Save Cylinders")) {
		SaveJson();
	}

	ImGui::End();
#endif // _DEBUG
}

void CylinderCollection::ApplyJson() {

	Json data = JsonAdapter::Load("cylinder.json");

	if (data.is_array()) {
		for (const auto& item : data) {

			Vector3 pos = JsonAdapter::ToVector3(item["pos"]);
			Vector3 scale = JsonAdapter::ToVector3(item["scale"]);

			auto newCylinder = std::make_unique<Cylinder>();
			newCylinder->Init(index_, pos, scale);

			cylinders_.push_back(std::move(newCylinder));
			++index_;
		}
	}
}

void CylinderCollection::SaveJson() {

	Json data = Json::array();

	for (size_t i = 0; i < cylinders_.size(); ++i) {

		Json cylinderData;
		cylinderData["pos"] = JsonAdapter::FromVector3(cylinders_[i]->GetWorldPos());
		cylinderData["scale"] = JsonAdapter::FromVector3(cylinders_[i]->GetScale());
		data.push_back(cylinderData);
	}

	JsonAdapter::Save("cylinder.json", data);

}