#include "RailCamera.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

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
}

void RailCamera::Update() {

	if (isStart_) {

		timer_ += NewMoonGame::GetDeltaTime();

		float t = timer_ / durationtime_;
		if (timer_ > durationtime_ - 5.0f) {
			isStart_ = false;
			timer_ = 0.0f;
		}

		reparamT_ = railEditor_->GetReparameterizedT(t);

		// 視点
		transform_.translation = railEditor_->SetCatmullRomPos(railEditor_->GetControlPoints(), reparamT_);

		float nextT = reparamT_ + (0.01f / railEditor_->GetArcLengths().back());

		// 注視点
		Vector3 nextPosition = railEditor_->SetCatmullRomPos(railEditor_->GetControlPoints(), nextT);

		forward_ = Vector3::Normalize(nextPosition - transform_.translation);

		// 差分ベクトルから回転角を求める
		// Y軸
		transform_.eulerRotate_.y = std::atan2(forward_.x, forward_.z);
		// X軸
		float length = Vector3::Length({ forward_.x, 0.0f, forward_.z });
		transform_.eulerRotate_.x = std::atan2(-forward_.y, length);

		transform_.rotation = Quaternion::EulerToQuaternion(transform_.eulerRotate_);

		const float yOffset = 0.5f;

		Vector3 setTranslate = transform_.translation;
		setTranslate.y = setTranslate.y + yOffset;
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
#endif
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
