#include "BaseParticle.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

//* Tasks *//
//・ImGuiでparameter調整できるようにする
//・イージング処理の追加
//・Editorの作成
//・ひたすら種類を増やす

/*////////////////////////////////////////////////////////////////////////////////
*							BaseParticle classMethods
////////////////////////////////////////////////////////////////////////////////*/

void BaseParticle::Create(const std::string& modelName, ParticleBehaviorType behaviorType) {

	particleSystem_ = std::make_unique<ParticleSystem>();
	particleSystem_->CreateParticle(modelName, modelName, behaviorType, parameter_);
	// 値の保存
	name_ = modelName;
	behaviorType_ = behaviorType;

}

void BaseParticle::PresetCreate(const std::string& modelName, ParticleBehaviorType behaviorType) {

	particleSystem_ = std::make_unique<ParticleSystem>();
	particleSystem_->PresetCreateParticle(modelName, modelName, behaviorType, parameter_);
	// 値の保存
	name_ = modelName;
	behaviorType_ = behaviorType;

}

void BaseParticle::Draw(BlendMode blendMode) {

	particleSystem_->Draw(name_, blendMode);

}

void BaseParticle::ImGui() {

	//* PtrAdress ++index *//
	uintptr_t ptrAdress = reinterpret_cast<uintptr_t>(this);

	if (ImGui::TreeNode("Transform")) {

		ImGui::DragFloat3(("Translate##" + std::to_string(ptrAdress)).c_str(), &parameter_.translate.x, 0.01f);
		ImGui::DragFloat3(("Scale##" + std::to_string(ptrAdress)).c_str(), &parameter_.scale.x, 0.01f);
		ImGui::TreePop();
	}

	DerivedImGui();

}