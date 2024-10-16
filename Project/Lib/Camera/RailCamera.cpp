#include "RailCamera.h"

#include "Engine/Base/NewMoonGame.h"

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
}

void RailCamera::Update() {

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
		Vector3 forward = target - eye;
		forward = Vector3::Normalize(forward);

		Vector3 rotate{};

		// 差分ベクトルから回転角を求める
		// Y軸
		rotate.y = std::atan2(forward.x, forward.z);
		// X軸
		float length = Vector3::Length({ forward.x, 0.0f, forward.z });
		rotate.x = std::atan2(-forward.y, length);

		worldTransform_.rotation = rotate;
	}

	worldTransform_.Update();

	camera_->SetViewMatrix(Matrix4x4::Inverse(worldTransform_.matrix.World));
	camera_->Update();
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

Matrix4x4 RailCamera::GetViewProjectionMatrix() const {
	return camera_->GetViewProjectionMatrix();
}
