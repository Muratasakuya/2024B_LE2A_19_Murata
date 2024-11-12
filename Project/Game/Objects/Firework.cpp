#include "Firework.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							Firework classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Firework::Init() {

	//* mustSetting *//

	BaseParticle::SetName();
	parameter_.scale.SetInit(0.1f);

	//* mainSetting *//

	parameter_.isUniform = true;

	parameter_.count = 96;

	parameter_.color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
	parameter_.easingType = EasingType::EaseInSine;

	parameter_.lifeTime = 5.0f;

	//* subSetting *//

	parameter_.sphereScale = 0.5f;

	BaseParticle::Create("cube.obj");
}

void Firework::Update() {

	particleSystem_->Update();
}

void Firework::DerivedImGui() {


}

/*////////////////////////////////////////////////////////////////////////////////
*						FireworkCollection classMethods
////////////////////////////////////////////////////////////////////////////////*/

void FireworkCollection::Init(RailEditor* railEditor) {

	railEditor_ = railEditor;

	// 出現させる予定の数
	const uint32_t fireworkNum = 4;
	fireworks_.resize(fireworkNum);

	// 色の設定：黄色、水色、薄い赤、薄緑
	std::vector<Vector4> colors = {
		Vector4(1.0f, 1.0f, 0.0f, 1.0f), // 黄色
		Vector4(0.0f, 1.0f, 1.0f, 1.0f), // 水色
		Vector4(1.0f, 0.5f, 0.5f, 1.0f), // 薄い赤
		Vector4(0.5f, 1.0f, 0.5f, 1.0f)  // 薄緑
	};

	for (uint32_t index = 0; index < fireworkNum; ++index) {

		fireworks_[index] = std::make_unique<Firework>();
		fireworks_[index]->Init();

		// 色の設定
		fireworks_[index]->SetColor(colors[index % colors.size()]);
	}

	emitEnable_.front() = true;
	emitEnable_.back() = true;
}

void FireworkCollection::Update() {

	FirstEmit();

	for (const auto& firework : fireworks_) {

		firework->Update();
	}
}

void FireworkCollection::Draw() {

	for (const auto& firework : fireworks_) {

		firework->Draw();
	}
}

void FireworkCollection::FirstEmit() {

	if (!emitEnable_.front()) {
		return;
	}

	// 第一陣
	if (NewMoonGame::GameCamera()->GetRailCamera()->GetReparamT() >= 0.025f && emitEnable_.front()) {

		// 座標の設定
		Vector3 pos = railEditor_->GetControlPointsByIndex(2);
		fireworks_[0]->SetPos({ pos.x - 2.0f,pos.y + 2.0f,pos.z });
		fireworks_[1]->SetPos({ pos.x - 3.0f,pos.y - 2.0f,pos.z });
		fireworks_[2]->SetPos({ pos.x + 2.0f,pos.y + 2.0f,pos.z });
		fireworks_[3]->SetPos({ pos.x + 3.0f,pos.y - 2.0f,pos.z });

		for (const auto& firework : fireworks_) {

			firework->Emit();
		}

		// 発射完了
		emitEnable_.front() = std::nullopt;
	}
}

void FireworkCollection::SecondEmit() {

	if (!emitEnable_.back()) {
		return;
	}

	// 第二陣
	if (NewMoonGame::GameCamera()->GetRailCamera()->GetReparamT() >= 0.025f && emitEnable_.back()) {

		// 座標の設定
		Vector3 pos = railEditor_->GetControlPointsByIndex(2);
		fireworks_[0]->SetPos({ pos.x - 2.0f,pos.y + 2.0f,pos.z });
		fireworks_[1]->SetPos({ pos.x - 3.0f,pos.y - 2.0f,pos.z });
		fireworks_[2]->SetPos({ pos.x + 2.0f,pos.y + 2.0f,pos.z });
		fireworks_[3]->SetPos({ pos.x + 3.0f,pos.y - 2.0f,pos.z });

		for (const auto& firework : fireworks_) {

			firework->Emit();
		}

		// 発射完了
		emitEnable_.back() = std::nullopt;
	}
}
