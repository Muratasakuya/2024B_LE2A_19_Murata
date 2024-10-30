#include "Player.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Player classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Player::Init() {

	BaseGameObject::Init("cube.obj");
	model_->SetTexture("white");

	transform_.scale = { 0.25f,0.25f,0.25f };

	transform_.parent_ = &NewMoonGame::GameCamera()->GetRailCamera()->GetTransform();

	BaseGameObject::SetName("Player");

	bulletReticle_ = std::make_unique<Sprite>();
	bulletReticle_->Init("bulletTargetReticle");

	bulletReticle_->AjustTextureSize(false);
	bulletReticle_->SetOriginalTextureSize(Vector2(48.0f, 48.0f));

	bulletReticleTransform_.Init();

}

void Player::Update(const Matrix4x4& viewPro) {

	Shoot();
	UpdateBullet(viewPro);
	UpdateBulletReticle(viewPro);

	BaseGameObject::Update(viewPro);
}

void Player::Draw() {

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {

		bullet->Draw();
	}
	bulletReticle_->Draw();
}

void Player::Shoot() {

	const float kBulletSpeed = 1.0f; //* 弾の速度

	if (NewMoonGame::TriggerKey(DIK_SPACE)) {

		Vector3 velocity = { 0.0f,0.0f,kBulletSpeed };
		velocity = bulletReticleTransform_.GetWorldPos() - transform_.GetWorldPos();
		velocity = Vector3::Normalize(velocity) * kBulletSpeed;

		//* Create NewBullet *//
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Init(transform_.GetWorldPos(), velocity, forward_);

		bullets_.push_back(std::move(newBullet));
	}
}

void Player::UpdateBullet(const Matrix4x4& viewPro) {

	bullets_.remove_if([](const std::unique_ptr<PlayerBullet>& bullet) {
		if (!bullet->IsAlive()) {

			NewMoonGame::RemoveCollider(bullet.get());
			return true;
		} else {
			return false;
		}
		});

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {

		bullet->Update(viewPro);
	}
}

void Player::UpdateBulletReticle(const Matrix4x4& viewPro) {

	bulletReticle_->SetPos(NewMoonGame::GetMousePos());

	Matrix4x4 matViewport =
		Matrix4x4::MakeViewportMatrix(0.0f, 0.0f,
			static_cast<float>(NewMoon::kWindowWidthf), static_cast<float>(NewMoon::kWindowHeightf), 0.0f, 1.0f);
	Matrix4x4 matVPV = viewPro * matViewport;
	Matrix4x4 matInverseVPV = Matrix4x4::Inverse(matVPV);

	Vector3 posNear = { NewMoonGame::GetMousePos().x, NewMoonGame::GetMousePos().y, 0.0f };
	Vector3 posFar = { NewMoonGame::GetMousePos().x, NewMoonGame::GetMousePos().y, 1.0f };

	// Screen -> World
	posNear = Vector3::Transform(posNear, matInverseVPV);
	posFar = Vector3::Transform(posFar, matInverseVPV);

	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Vector3::Normalize(mouseDirection);

	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 32.0f;
	bulletReticleTransform_.translation = posNear + mouseDirection * kDistanceTestObject;

	bulletReticle_->Update();
	bulletReticleTransform_.Update(viewPro);

}

/*////////////////////////////////////////////////////////////////////////////////
*										Setter
////////////////////////////////////////////////////////////////////////////////*/
void Player::SetForward(const Vector3& forward) {
	forward_ = forward;
}