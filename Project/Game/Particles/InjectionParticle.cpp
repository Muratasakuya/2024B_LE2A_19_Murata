#include "InjectionParticle.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*						InjectionParticle classMethods
////////////////////////////////////////////////////////////////////////////////*/

void InjectionParticle::Init() {

	BaseParticle::SetName("injectionParticle");

	parameter_.scale.uniform.SetInit(0.4f);

	parameter_.scale.min.SetInit(0.15f);
	parameter_.scale.max.SetInit(0.25f);
	parameter_.isUniform = true;

	parameter_.lifeTime = 6.0f;
	parameter_.speed = 5.0f;

	parameter_.count = 32;       //* 一回で出る個数
	parameter_.frequency = 8.0f; //* 出る間隔

	// 上に出るようにする
	parameter_.injectionDirection = Direction::Up();
	parameter_.physics.gravityDirection = Direction::Down();
	// 反射
	parameter_.physics.reflectFace.y = 0.25f;
	parameter_.physics.restitution = 0.8f;
	parameter_.physics.reflectEnable = true;

	parameter_.easingType = EasingType::EaseOutCubic;

	// EmitterのAABBのサイズと座標
	parameter_.aabb.center = Vector3(-10.0f, 0.26f, 20.0f);
	parameter_.aabb.extent = Vector3(0.1f, 0.1f, 0.1f);

	BaseParticle::Create("sphere.obj"); //* 使用するModelの名前で初期化

	particleSystem_->SetTexture("injectionParticle", "circle");

}

void InjectionParticle::Update() {

	// 時間を加算
	parameter_.frequencyTime += NewMoonGame::GetDeltaTime();

	// frequencyTime が frequency を超えていたら
	if (parameter_.frequency <= parameter_.frequencyTime) {

		emitTimer_ += NewMoonGame::GetDeltaTime();
		if (emitTimer_ >= 0.25f) {

			if (emitCount_ == 0) {

				parameter_.color = Color::Red();
			} else if (emitCount_ == 1) {

				parameter_.color = Color::Green();
			} else if (emitCount_ == 2) {

				parameter_.color = Color::Blue();
			} else if (emitCount_ == 3) {

				parameter_.color = Color::White();
			}

			particleSystem_->EmitParticle(IBaseParticle::GetName(), particleType_, parameter_);
			emitTimer_ = 0.0f; // タイマーをリセット
			++emitCount_;      // 実行回数をカウント
		}

		if (emitCount_ >= 4) {

			parameter_.frequencyTime -= parameter_.frequency;
			emitCount_ = 0;    // カウントをリセット
		}
	}

	// 毎フレームの更新処理
	particleSystem_->Update();
}

void InjectionParticle::DerivedImGui() {
#ifdef _DEBUG

	ImGui::DragFloat3("aabbCenter", &parameter_.aabb.center.x, 0.01f);
	ImGui::DragFloat3("aabbExtent", &parameter_.aabb.extent.x, 0.01f);
	ImGui::Separator();
	ImGui::Text("physics");
	ImGui::Checkbox("reflectEnable", &parameter_.physics.reflectEnable);
	ImGui::DragFloat("reflectFaceY", &parameter_.physics.reflectFace.y, 0.01f);
	ImGui::DragFloat("restitution", &parameter_.physics.restitution, 0.01f);

#endif // _DEBUG
}