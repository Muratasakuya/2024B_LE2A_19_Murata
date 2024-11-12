#include "RailCamera.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/// レールの配置によってスピードを変える。 // △だが最低限これでいいだろう。
/// フィールドの作成                   // これもまた最低限。ここからはほぼ演出の域に入るだろう
/// 敵の配置とスコア表記
/// 演出

/*////////////////////////////////////////////////////////////////////////////////
*							RailCamera classMethods
////////////////////////////////////////////////////////////////////////////////*/

void RailCamera::Init(RailEditor* railEditor, const Vector3& worldPos) {

	railEditor_ = railEditor;

	transform_.Init();
	transform_.translation = worldPos;

	camera_ = std::make_unique<Camera3D>();
	camera_->Init();

	// ConstBuffer初期化
	cameraBuffer_.Init();
	viewProBuffer_.Init();

	isStart_ = false;
	guiUpdate_ = false;

	t_ = 0.0f;

	deltaRate_ = 1.0f; // デフォで1.0f

	CameraInit();

}

void RailCamera::Update() {

	GuiCameraUpdate();

	if (isStart_) {

		float currentHeight = transform_.translation.y;

		float nextT = reparamT_ + (0.01f / railEditor_->GetArcLengths().back());
		Vector3 nextPosition = railEditor_->SetCatmullRomPos(railEditor_->GetControlPoints(), nextT);
		float nextHeight = nextPosition.y;

		float heightDifference = nextHeight - currentHeight;
		if (heightDifference > 0.0f) {

			targetDeltaRate_ = 0.6f;
		} else if (heightDifference < 0.0f) {

			targetDeltaRate_ = 2.25f;
		} else {

			targetDeltaRate_ = 0.78f;
		}

		// 現在の deltaRate_ を目標値へ滑らかに補完
		deltaRate_ = std::lerp(deltaRate_, targetDeltaRate_, 0.025f);

		// deltaRate_ を利用して timer_ を更新
		timer_ += NewMoonGame::GetDeltaTime() * deltaRate_;

		// 以下、既存の処理
		t_ = timer_ / durationtime_;
		if (timer_ > durationtime_ - 2.0f) {
			isStart_ = false;
			timer_ = 0.0f;
		}

		reparamT_ = railEditor_->GetReparameterizedT(t_);

		// 視点の設定
		transform_.translation = railEditor_->SetCatmullRomPos(railEditor_->GetControlPoints(), reparamT_);

		// 注視点を計算
		forward_ = Vector3::Normalize(nextPosition - transform_.translation);

		// 回転の設定
		transform_.eulerRotate_.y = std::atan2(forward_.x, forward_.z);
		float length = Vector3::Length({ forward_.x, 0.0f, forward_.z });
		transform_.eulerRotate_.x = std::atan2(-forward_.y, length);
		transform_.rotation = Quaternion::EulerToQuaternion(transform_.eulerRotate_);

		// カメラ位置と回転の更新
		const float yOffset = 0.5f;
		Vector3 setTranslate = transform_.translation;
		setTranslate.y += yOffset;
		NewMoonGame::GameCamera()->GetCamera3D()->SetTranslate(setTranslate);
		NewMoonGame::GameCamera()->GetCamera3D()->SetRotate(transform_.eulerRotate_);

	}

	transform_.Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());

	camera_->SetViewMatrix(Matrix4x4::Inverse(transform_.matrix.World));
	camera_->Update();

	// ConstBuffer転送
	cameraBuffer_.Update(camera_->GetWorldPos());
	viewProBuffer_.Update(camera_->GetViewProjectionMatrix());

}

void RailCamera::ImGui() {
#ifdef _DEBUG

	float elapsedSeconds = timer_;
	float totalDurationSeconds = durationtime_;
	ImGui::Text("Time: %.2f s / %.2f s", elapsedSeconds, totalDurationSeconds);
	ImGui::Text("T: %.2f / 1.0f", t_);
	ImGui::Text("ReparamT: %.4f / 1.0f", reparamT_);
	ImGui::DragFloat("deltaRate", &deltaRate_, 0.01f);
	ImGui::DragFloat3("Translate##Rail", &transform_.translation.x, 0.01f);
	if (!isStart_) {
		if (ImGui::Button("Start")) {
			isStart_ = true;
		}
	}
	if (isStart_) {
		if (ImGui::Button("STOP")) {
			isStart_ = false;
			timer_ = 0.0f;

			NewMoonGame::GameCamera()->GetCamera3D()->Reset();
			transform_.Init();
			camera_->Init();
		}
	}
	if (!guiUpdate_) {
		if (ImGui::Button("Gui Update")) {
			guiUpdate_ = true;
		}
	}
	if (guiUpdate_) {
		ImGui::DragFloat("Time", &timer_, 0.01f);
	}
#endif
}

void RailCamera::CameraInit() {

	timer_ = 0.0f;

	float t = timer_ / durationtime_;

	reparamT_ = railEditor_->GetReparameterizedT(t);

	// 視点の設定
	transform_.translation = railEditor_->SetCatmullRomPos(railEditor_->GetControlPoints(), reparamT_);
	float nextT = reparamT_ + (0.01f / railEditor_->GetArcLengths().back());
	Vector3 nextPosition = railEditor_->SetCatmullRomPos(railEditor_->GetControlPoints(), nextT);

	// 注視点を計算
	forward_ = Vector3::Normalize(nextPosition - transform_.translation);

	// 回転の設定
	transform_.eulerRotate_.y = std::atan2(forward_.x, forward_.z);
	float length = Vector3::Length({ forward_.x, 0.0f, forward_.z });
	transform_.eulerRotate_.x = std::atan2(-forward_.y, length);
	transform_.rotation = Quaternion::EulerToQuaternion(transform_.eulerRotate_);

	// カメラ位置と回転の更新
	const float yOffset = 0.5f;
	Vector3 setTranslate = transform_.translation;
	setTranslate.y += yOffset;
	NewMoonGame::GameCamera()->GetCamera3D()->SetTranslate(setTranslate);
	NewMoonGame::GameCamera()->GetCamera3D()->SetRotate(transform_.eulerRotate_);

}

void RailCamera::GuiCameraUpdate() {

	if (!guiUpdate_) {
		return;
	}

	float nextT = reparamT_ + (0.01f / railEditor_->GetArcLengths().back());
	Vector3 nextPosition = railEditor_->SetCatmullRomPos(railEditor_->GetControlPoints(), nextT);

	// 以下、既存の処理
	t_ = timer_ / durationtime_;

	reparamT_ = railEditor_->GetReparameterizedT(t_);

	// 視点の設定
	transform_.translation = railEditor_->SetCatmullRomPos(railEditor_->GetControlPoints(), reparamT_);

	// 注視点を計算
	forward_ = Vector3::Normalize(nextPosition - transform_.translation);

	// 回転の設定
	transform_.eulerRotate_.y = std::atan2(forward_.x, forward_.z);
	float length = Vector3::Length({ forward_.x, 0.0f, forward_.z });
	transform_.eulerRotate_.x = std::atan2(-forward_.y, length);
	transform_.rotation = Quaternion::EulerToQuaternion(transform_.eulerRotate_);

	// カメラ位置と回転の更新
	const float yOffset = 0.5f;
	Vector3 setTranslate = transform_.translation;
	setTranslate.y += yOffset;
	NewMoonGame::GameCamera()->GetCamera3D()->SetTranslate(setTranslate);
	NewMoonGame::GameCamera()->GetCamera3D()->SetRotate(transform_.eulerRotate_);

	transform_.Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());

	camera_->SetViewMatrix(Matrix4x4::Inverse(transform_.matrix.World));
	camera_->Update();

	// ConstBuffer転送
	cameraBuffer_.Update(camera_->GetWorldPos());
	viewProBuffer_.Update(camera_->GetViewProjectionMatrix());

}

/*///////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
Vector3 RailCamera::GetWorldPos() const {
	Vector3 worldPos{};
	worldPos.x = transform_.matrix.World.m[3][0];
	worldPos.y = transform_.matrix.World.m[3][1];
	worldPos.z = transform_.matrix.World.m[3][2];

	return worldPos;
}
Vector3 RailCamera::GetFoward() const {
	return forward_;
}

Matrix4x4 RailCamera::GetViewProjectionMatrix() const {
	return camera_->GetViewProjectionMatrix();
}
WorldTransform& RailCamera::GetTransform() {
	return transform_;
}

bool RailCamera::IsStart() const {
	return isStart_;
}
