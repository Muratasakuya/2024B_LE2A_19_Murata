#include "PlayerChaseParticle.h"

#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*						PlayerChaseParticle classMethods
////////////////////////////////////////////////////////////////////////////////*/

void PlayerChaseParticle::Init() {

	BaseParticle::SetName("playerChaseParticle");

	parameter_.scale.min.SetInit(0.15f);
	parameter_.scale.max.SetInit(0.2f);

	parameter_.isUniform = false;   //* 非均一
	parameter_.count = 3;           //* ~秒置きに出る数
	parameter_.frequency = 0.1f;    //* ~秒置き
	parameter_.lifeTime = 1.5f;     //* 生存時間
	parameter_.color = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
	parameter_.easingType = EasingType::EaseInSine;

	// EmitterのAABBのサイズと座標
	parameter_.aabb.center.Init();
	parameter_.aabb.extent = Vector3(0.1f, 0.1f, 0.1f);

	BaseParticle::Create("cube.obj"); //* 使用するModelの名前で初期化

}

void PlayerChaseParticle::Update() {

	BaseParticle::UpdateFrequencyEmit(NewMoonGame::GetDeltaTime());

	parameter_.prePos = parameter_.aabb.center;
	particleSystem_->Update();
}

void PlayerChaseParticle::SetPos(const Vector3& pos) {

	parameter_.aabb.center = pos;
	parameter_.aabb.center.y = 0.0f;
}