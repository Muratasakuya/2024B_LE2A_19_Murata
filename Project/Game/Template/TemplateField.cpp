#include "TemplateField.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"
#include "Lib/Adapter/JsonAdapter.h"

/*////////////////////////////////////////////////////////////////////////////////
*							TemplateField classMethods
////////////////////////////////////////////////////////////////////////////////*/

void TemplateField::Init() {

	CreateModel(16);

	BaseGameObject::Init(modelName_);
	BaseGameObject::SetName("templateField");

	BaseGameObject::model_->SetTexture("templateField");

	parentFolderName_ = "./TemplateField/";

	BaseGameObject::ApplyJsonForColor();
	BaseGameObject::ApplyJsonForTransform(transform_);
	ApplyJson();

	BaseGameObject::SetUVTransform(uvScale_);

}

void TemplateField::Update() {

	BaseGameObject::Update();

}

void TemplateField::Draw() {

	BaseGameObject::Draw();

}

void TemplateField::DerivedImGui() {

	if (ImGui::Button("Save")) {

		SaveJson();
	}

	ImGui::DragFloat3("uvScale", &uvScale_.x, 0.025f);

}

void TemplateField::CreateModel(int division) {

	float halfWidth = 1.0f;
	float halfDepth = 1.0f;

	std::vector<VertexData3D> vertices;

	for (int z = 0; z <= division; ++z) {
		for (int x = 0; x <= division; ++x) {

			VertexData3D vertex;

			// X, Z方向の位置
			float xPos = -halfWidth + (x / static_cast<float>(division)) * halfWidth * 2.0f;
			float zPos = -halfDepth + (z / static_cast<float>(division)) * halfWidth * 2.0f;
			vertex.pos = Vector4(xPos, 0.0f, zPos, 1.0f);

			// UV座標
			vertex.texcoord = Vector2(x / static_cast<float>(division), z / static_cast<float>(division));
			vertex.texcoord.y *= -1.0f;
			// 上固定
			vertex.normal = Vector3(0.0f, 1.0f, 0.0f);

			vertices.push_back(vertex);

		}
	}

	std::vector<uint32_t> indices;

	for (int z = 0; z < division; ++z) {
		for (int x = 0; x < division; ++x) {

			int topLeft = z * (division + 1) + x;
			int topRight = topLeft + 1;
			int bottomLeft = (z + 1) * (division + 1) + x;
			int bottomRight = bottomLeft + 1;

			// 左上三角形
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			// 右下三角形
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}

	// モデル追加
	NewMoonGame::GetModelManager()->MakeOriginalModel(modelName_, vertices, indices);

}

void TemplateField::SaveJson() {

	Json data;

	data["uvScale"] = JsonAdapter::FromVector3(uvScale_);

	JsonAdapter::Save(parentFolderName_.value() + "templateFieldUVScale.json", data);

}

void TemplateField::ApplyJson() {

	Json data = JsonAdapter::Load(parentFolderName_.value() + "templateFieldUVScale.json");

	uvScale_ = JsonAdapter::ToVector3(data["uvScale"]);

}