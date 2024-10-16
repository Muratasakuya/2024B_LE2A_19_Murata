#include "RailCamera.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							RailCamera classMethods
////////////////////////////////////////////////////////////////////////////////*/

void RailCamera::Init(RailEditor* railEditor, const Vector3& worldPos) {

	railEditor_ = railEditor;

	worldTransform_.Init();
	worldTransform_.translation = worldPos;

	camera_ = std::make_unique<Camera3D>();
	camera_->Init();

	// t補完を少しずらす 注視点の方が先
	eyeT_ = 0;
	targetT_ = 0.01f;

	// ConstBuffer初期化
	cameraBuffer_.Init();
	viewProBuffer_.Init();

	isStart_ = false;
}

void RailCamera::Update() {

	if (isStart_) {

		// 進ませる速度
		const float speed = 0.0005f;

		// tを進める
		eyeT_ = eyeT_ + speed;
		targetT_ = targetT_ + speed;

		// レールが終わるまで
		if (targetT_ < 1.0f) {

			// レール上の視点（eye）と少し進んだ地点（target）を求める
			Vector3 eye = railEditor_->SetCatmullRomPos(railEditor_->GetControlPoints(), eyeT_);
			Vector3 target = railEditor_->SetCatmullRomPos(railEditor_->GetControlPoints(), targetT_);

			worldTransform_.translation = eye;

			// targetとeyeの差分ベクトル
			forward_ = target - eye;
			forward_ = Vector3::Normalize(forward_);

			Vector3 rotate{};

			// 差分ベクトルから回転角を求める
			// Y軸
			rotate.y = std::atan2(forward_.x, forward_.z);
			// X軸
			float length = Vector3::Length({ forward_.x, 0.0f, forward_.z });
			rotate.x = std::atan2(-forward_.y, length);

			worldTransform_.rotation = rotate;
		}
	}

	worldTransform_.Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());

	camera_->SetViewMatrix(Matrix4x4::Inverse(worldTransform_.matrix.World));
	camera_->Update();

	// ConstBuffer転送
	cameraBuffer_.Update(camera_->GetWorldPos());
	viewProBuffer_.Update(camera_->GetViewProjectionMatrix());
}

void RailCamera::ImGui() {
#ifdef _DEBUG
	if (!isStart_) {
		if (ImGui::Button("Start")) {
			isStart_ = true;
		}
	}
	if (isStart_) {
		if (ImGui::Button("STOP")) {
			isStart_ = false;
		}
	}
	ImGui::DragFloat3("Translate", &worldTransform_.translation.x, 0.01f);
	ImGui::DragFloat3("Rotation", &worldTransform_.rotation.x, 0.01f);
#endif
}

/*///////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
Vector3 RailCamera::GetWorldPos() const {
	Vector3 worldPos{};
	worldPos.x = worldTransform_.matrix.World.m[3][0];
	worldPos.y = worldTransform_.matrix.World.m[3][1];
	worldPos.z = worldTransform_.matrix.World.m[3][2];

	return worldPos;
}
Vector3 RailCamera::GetFoward() const {
	return forward_;
}

Matrix4x4 RailCamera::GetViewProjectionMatrix() const {
	return camera_->GetViewProjectionMatrix();
}
WorldTransform& RailCamera::GetWorldTransform() {
	return worldTransform_;
}