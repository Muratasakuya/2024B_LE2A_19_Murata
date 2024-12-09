#include "StaticMeshModel.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/SrvManager.h"
#include "Engine/Managers/ImGuiManager.h"
#include "Lib/Adapter/Random.h"
#include "Lib/Adapter/Easing.h"

/*////////////////////////////////////////////////////////////////////////////////
*							StaticMeshModel classMethods
////////////////////////////////////////////////////////////////////////////////*/

void StaticMeshModel::CreateVertexData(const std::string& name) {

	size_t meshNum = modelData_.meshes.size();

	for (uint32_t index = 0; index < meshNum; ++index) {

		auto& mesh = modelData_.meshes[index];

		// 三角形データ
		for (const auto& triangle : mesh.triangles) {

			trianglePositions_.push_back(triangle.v0);
			trianglePositions_.push_back(triangle.v1);
			trianglePositions_.push_back(triangle.v2);

			triangleNormals_.push_back(triangle.n0);
			triangleNormals_.push_back(triangle.n1);
			triangleNormals_.push_back(triangle.n2);
		}

		inputAssembler_.Init(mesh);

		modelData_.meshes[index].material.textureName = "white";
	}

	size_t numTriangles = trianglePositions_.size() / 3;
	for (size_t i = 0; i < numTriangles; ++i) {

		TriangleDataForGPU triangle;
		triangle.vertex0 = trianglePositions_[i * 3 + 0];
		triangle.vertex1 = trianglePositions_[i * 3 + 1];
		triangle.vertex2 = trianglePositions_[i * 3 + 2];

		triangleData_.push_back(triangle);
	}

}

void StaticMeshModel::Init() {

	name_ = "teapot";

	// ModelDataをもとに頂点生成
	modelData_ = NewMoonGame::GetModelManager()->GetModelData("teapot.obj");
	CreateVertexData("teapot.obj");

	// 三角形の総数と頂点数
	size_t numTriangles = trianglePositions_.size() / 3;
	size_t numVertices = trianglePositions_.size();

	// 初期化
	numInstance_ = static_cast<int>(numTriangles);

	animationState_ = AnimationState::Begin;

	beginMoveOnce_ = false;

	beginWaitTimer_.elapsed = 0.0f;
	beginWaitTimer_.target = 4.0f;

	beginTimer_.elapsed = 0.0f;
	beginTimer_.target = 2.0f;

	playMoveOnce_ = false;

	playTimer_.elapsed = 0.0f;
	playTimer_.target = 2.0f;

	endTimer_.elapsed = 0.0f;
	endTimer_.target = 4.0f;

	color_ = Vector4(0.9f, 0.8f, 0.8f, 1.0f);

	//===================================================================*/
	// MeshBuffer

	instancingMeshSrvIndex_ = NewMoon::GetSrvManagerPtr()->Allocate("staticMeshInstancingResource");
	meshBuffer_.Init(static_cast<UINT>(numTriangles));
	NewMoon::GetSrvManagerPtr()->CreateSRVForStructureBuffer(
		instancingMeshSrvIndex_, meshBuffer_.GetResource(),
		static_cast<UINT>(numTriangles), sizeof(StaticMeshForGPU));

	meshBuffer_.properties.resize(numTriangles);

	transforms_.resize(numTriangles);
	velocity_.resize(numTriangles);
	acceleration_.resize(numTriangles);

	Vector4 startColor = Color::Blue();
	Vector4 endColor = Color::Red();


	for (uint32_t index = 0; index < numTriangles; ++index) {

		transforms_[index].scale.SetInit(1.0f);
		transforms_[index].rotate.Init();
		transforms_[index].translate = Vector3(10.0f, 1.0f, 12.0f);

		velocity_[index] = triangleNormals_[index];
		acceleration_[index].Init();
		acceleration_[index].y;

	}

	//===================================================================*/
	// TriangleBuffer

	instancingTriangleSrvIndex_ = NewMoon::GetSrvManagerPtr()->Allocate("triangleInstancingResource");
	triangleVertexBuffer_.Init(static_cast<UINT>(numVertices));
	NewMoon::GetSrvManagerPtr()->CreateSRVForStructureBuffer(
		instancingTriangleSrvIndex_, triangleVertexBuffer_.GetResource(),
		static_cast<UINT>(numVertices), sizeof(TriangleDataForGPU));

	triangleVertexBuffer_.properties.resize(numVertices);
}

void StaticMeshModel::Update() {

	const Matrix4x4 viewProjectionMatrix = NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();
	const Vector4 defaultColor = Color::White();
	size_t numTriangles = trianglePositions_.size() / 3;

	switch (animationState_) {
	case StaticMeshModel::AnimationState::Begin: {

		beginWaitTimer_.elapsed += NewMoonGame::GetDeltaTime();
		if (beginWaitTimer_.elapsed >= beginWaitTimer_.target) {

			for (size_t index = 0; index < numTriangles; ++index) {

				if (!beginMoveOnce_) {

					transforms_[index].translate += triangleNormals_[index] / 12.0f;
				}

				Matrix4x4 worldMatrix =
					Matrix4x4::MakeAffineMatrix(
						transforms_[index].scale, transforms_[index].rotate, transforms_[index].translate);

				meshBuffer_.properties[index].WVP = worldMatrix * viewProjectionMatrix;
				meshBuffer_.properties[index].color = color_;
			}
		} else {

			for (size_t index = 0; index < numTriangles; ++index) {

				Matrix4x4 worldMatrix =
					Matrix4x4::MakeAffineMatrix(
						transforms_[index].scale, transforms_[index].rotate, transforms_[index].translate);

				meshBuffer_.properties[index].WVP = worldMatrix * viewProjectionMatrix;
				meshBuffer_.properties[index].color = color_;
			}

			break;
		}

		beginTimer_.elapsed += NewMoonGame::GetDeltaTime();
		if (beginTimer_.elapsed >= beginTimer_.target) {

			animationState_ = AnimationState::Play;
		}

		// すでに少し動いた
		beginMoveOnce_ = true;

		break;
	}
	case StaticMeshModel::AnimationState::Play: {

		playTimer_.elapsed += NewMoonGame::GetDeltaTime();
		float t = playTimer_.elapsed / playTimer_.target;
		float easedT = EaseOutExpo(t);
		if (playTimer_.elapsed > playTimer_.target) {

			animationState_ = AnimationState::End;
			break;
		}

		for (size_t index = 0; index < numTriangles; ++index) {

			// どんどん小さく
			transforms_[index].scale.SetInit(1.0f - easedT);

			// 外側に一瞬広がってそこから上に上がる
			if (!playMoveOnce_) {

				velocity_[index] = velocity_[index] / 10.0f;
			} else {

				// 加速度による減速
				acceleration_[index].y = std::lerp(-0.002f, 0.001f, easedT);
				velocity_[index] += acceleration_[index];
			}

			transforms_[index].translate += velocity_[index] * easedT;

			transforms_[index].rotate.x += Random::Generate(0.01f, 0.02f);
			transforms_[index].rotate.y += Random::Generate(0.01f, 0.02f);
			transforms_[index].rotate.z += Random::Generate(0.01f, 0.02f);

			Matrix4x4 worldMatrix =
				Matrix4x4::MakeAffineMatrix(
					transforms_[index].scale, transforms_[index].rotate, transforms_[index].translate);

			meshBuffer_.properties[index].WVP = worldMatrix * viewProjectionMatrix;
			meshBuffer_.properties[index].color = color_;
		}

		playMoveOnce_ = true;

		break;
	}
	case StaticMeshModel::AnimationState::End: {

		endTimer_.elapsed += NewMoonGame::GetDeltaTime();
		float t = endTimer_.elapsed / endTimer_.target;
		float easedT = EaseOutExpo(t);
		if (endTimer_.elapsed > endTimer_.target) {

			// 初めから
			animationState_ = AnimationState::Begin;

			playMoveOnce_ = false;
			beginMoveOnce_ = false;

			beginTimer_.elapsed = 0.0f;
			beginWaitTimer_.elapsed = 0.0f;
			playTimer_.elapsed = 0.0f;
			endTimer_.elapsed = 0.0f;

			break;
		}

		for (size_t index = 0; index < numTriangles; ++index) {

			velocity_[index] = triangleNormals_[index];

			transforms_[index].scale.SetInit(easedT);

			transforms_[index].rotate.Init();
			transforms_[index].translate = Vector3(10.0f, 1.0f, 12.0f);

			Matrix4x4 worldMatrix =
				Matrix4x4::MakeAffineMatrix(
					transforms_[index].scale, transforms_[index].rotate, transforms_[index].translate);

			meshBuffer_.properties[index].WVP = worldMatrix * viewProjectionMatrix;
			meshBuffer_.properties[index].color = color_;
		}

		break;
	}
	}

	for (size_t i = 0; i < numTriangles; ++i) {

		triangleVertexBuffer_.properties[i].vertex0 = trianglePositions_[i * 3 + 0];
		triangleVertexBuffer_.properties[i].vertex1 = trianglePositions_[i * 3 + 1];
		triangleVertexBuffer_.properties[i].vertex2 = trianglePositions_[i * 3 + 2];
	}

	meshBuffer_.Update();
	triangleVertexBuffer_.Update();

}

void StaticMeshModel::Draw() {

	auto commandList = NewMoon::GetCommandList();

	for (uint32_t index = 0; index < modelData_.meshes.size(); ++index) {

		NewMoon::SetGraphicsPipeline(commandList, pStaticMesh, kBlendModeNormal);
		inputAssembler_.SetBuffer(commandList, index);
		NewMoon::SetGraphicsRootDescriptorTable(commandList, 0, modelData_.meshes[index].material.textureName.value());
		commandList->SetGraphicsRootDescriptorTable(1, NewMoon::GetSrvManagerPtr()->GetGPUHandle(instancingMeshSrvIndex_));
		commandList->SetGraphicsRootDescriptorTable(2, NewMoon::GetSrvManagerPtr()->GetGPUHandle(instancingTriangleSrvIndex_));
		inputAssembler_.DrawCall(commandList, numInstance_, index);
	}

}

void StaticMeshModel::ImGui() {
#ifdef _DEBUG

	ImGui::ColorEdit4("color", &color_.x);

#endif // _DEBUG
}