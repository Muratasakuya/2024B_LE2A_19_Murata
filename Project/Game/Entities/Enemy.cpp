#include "Enemy.h"

#include "Engine/Base/NewMoonGame.h"
#include "Lib/Adapter/Random.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Enemy classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Enemy::TypeInit() {

	std::string modelName; //* モデルの名前
	float scale = 0.0f;    //* スケール
	switch (type_) {
	case EnemyType::kBalloon: {

		modelName = "balloon.obj";
		BaseGameObject::Init(modelName);
		BaseGameObject::SetLightingEnable(false);

		scale = 0.25f;
		color_ =
			Vector4(
				Random::Generate(0.2f, 0.8f),
				Random::Generate(0.2f, 0.8f),
				Random::Generate(0.2f, 0.8f), 1.0f);

		score_ = 50;

	}
							break;
	case EnemyType::kUFO: {

		modelName = "UFO.obj";
		BaseGameObject::Init(modelName);

		scale = 0.25f;
		color_ = Vector4(0.9f, 0.9f, 0.05f, 1.0f);

		score_ = 100;

	}
						break;
	case EnemyType::kGhost: {

		modelName = "ghost.obj";
		BaseGameObject::Init(modelName);
		BaseGameObject::SetLightingEnable(false);

		scale = 0.25f;
		color_.SetInit(1.0f);

		score_ = 150;

	}
						  break;
	}

	model_->SetTexture("white");

	transform_.scale.SetInit(scale);
}

void Enemy::Init(EnemyType type, const Vector3& pos) {

	type_ = type;

	initPos_ = pos;

	// タイプごとの初期化
	TypeInit();

	transform_.translation = pos;
	lerpPos_ = pos;
	targetPosition_ = pos;
	targetPosition_.y += 0.01f;

	// 余分にサイズを持たせる
	Collider::SetCollisionShapeSphere({ .radius = {1.0f * transform_.scale.x + 0.2f} });

	Collider::type_ = ColliderType::Type_Enemy;
	Collider::targetType_ = ColliderType::Type_PlayerBullet;

	Collider::centerPos_ = transform_.translation;

	isAlive_ = true;

}

void Enemy::Update(const Matrix4x4& viewPro) {

	// 移動処理
	Move();

	Collider::centerPos_ = transform_.translation;
	if (type_ == EnemyType::kBalloon) {

		Collider::centerPos_.y += 0.1f;
	}

	BaseGameObject::Update(viewPro);

}

void Enemy::Draw() {

	BaseGameObject::Draw();

}

void Enemy::OnCollisionEnter(Collider* collider) {

	if (collider->GetType() == ColliderType::Type_PlayerBullet) {

		// enemyManagerClass -> remove_if(delete)
		isAlive_ = false;
	}
}

void Enemy::OnCollisionStay(Collider* collider) {

	if (collider->GetType() == ColliderType::Type_PlayerBullet) {

		// enemyManagerClass -> remove_if(delete)
		isAlive_ = false;
	}
}

void Enemy::OnCollisionExit(Collider* collider) {

	if (collider->GetType() == ColliderType::Type_PlayerBullet) {

		// enemyManagerClass -> remove_if(delete)
		isAlive_ = false;
	}
}

bool Enemy::IsAlive() const {

	return isAlive_;
}

void Enemy::Move() {

	switch (type_) {
	case EnemyType::kBalloon: {

		// Y軸方向に一定速度で上昇する
		const float ySpeed = 0.0025f;
		transform_.translation.y += ySpeed;
		break;
	}
	case EnemyType::kUFO: {

		const float lerpSpeed = 0.02f;  // Lerpの進行速度
		lerpProgress_ += lerpSpeed;

		// Lerpで現在位置とターゲット位置を補間して移動
		transform_.translation = Vector3::Lerp(lerpPos_, targetPosition_, lerpProgress_);

		// ターゲット位置に到達したら新しいターゲット位置を設定
		if (lerpProgress_ >= 1.0f) {
			// lerpProgressをリセット
			lerpProgress_ = 0.0f;
			lerpPos_ = targetPosition_; // 現在の位置を新しい起点にする

			// initPos_を基準に範囲を設定
			targetPosition_ = Vector3(
				initPos_.x + Random::Generate(-1.2f, 1.2f),  // X方向の範囲
				initPos_.y + Random::Generate(-1.2f, 1.2f),  // Y方向の範囲
				initPos_.z + Random::Generate(-1.2f, 1.2f)   // Z方向の範囲
			);
		}

		break;
	}
	case EnemyType::kGhost: {

		// X軸方向にサイン波で揺れながら前進
		const float oscillationAmplitude = 0.1f;
		const float oscillationSpeed = 0.02f;
		float oscillation = oscillationAmplitude * sinf(transform_.translation.z * oscillationSpeed);
		transform_.translation.x += oscillation * 0.5f;
		transform_.translation.z += oscillation * 0.5f;

		transform_.eulerRotate_.y += 0.008f;

		transform_.rotation = Quaternion::EulerToQuaternion(transform_.eulerRotate_);

		break;
	}
	}
}