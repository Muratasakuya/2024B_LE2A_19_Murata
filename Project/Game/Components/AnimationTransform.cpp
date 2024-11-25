#include "AnimationTransform.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*						AnimationTransform classMethods
////////////////////////////////////////////////////////////////////////////////*/

void AnimationTransform::Init(const std::string& modelName, const std::string& animationName) {

	BaseTransform::Init();

	animationData_[animationName] = NewMoonGame::GetModelManager()->GetAnimationData(animationName);
	skeleton_[animationName] = NewMoonGame::GetModelManager()->GetSkeletonData(animationName);
	skeleton_[animationName].value().name = animationName;
	skinCluster_[animationName] = NewMoonGame::GetModelManager()->GetSkinClusterData(animationName);

	modelData_ = NewMoonGame::GetModelManager()->GetModelData(modelName);

	animationController_.first = false;
	animationTime_ = 0.0f;

}

void AnimationTransform::Update() {

	if (animationController_.first) {

		//* repeat
		animationTime_ += NewMoonGame::GetDeltaTime();
		animationTime_ = std::fmod(animationTime_, animationData_[animationController_.second].duration);

		ImGui::Text("animationDuration: %f", animationTime_);

		// スケルトンが存在する場合
		if (skeleton_[animationController_.second]) {

			// アニメーション適応
			NewMoonGame::ApplyAnimation(skeleton_[animationController_.second].value().name, animationTime_);
			// スケルトンの更新
			NewMoonGame::SkeletonUpdate(skeleton_[animationController_.second].value().name);
			// スキンクラスターの更新
			NewMoonGame::SkinClusterUpdate(skeleton_[animationController_.second].value().name);
		}
	}

	Matrix4x4 worldMatrix = Matrix4x4::MakeAxisAffineMatrix(scale, rotation, translation);
	Matrix4x4 wvpMatrix = worldMatrix * NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();
	Matrix4x4 worldInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(worldMatrix));

	matrix.World = worldMatrix;
	matrix.WVP = wvpMatrix;
	matrix.WorldInverseTranspose = worldInverseTranspose;

	TransferData(matrix);

}

void AnimationTransform::SetPlayAnimation(bool isPlayAnimation, const std::string& animationName) {

	animationController_.first = isPlayAnimation;
	animationController_.second = animationName;
}

void AnimationTransform::SetNewAnimationData(const std::string& animationName) {

	animationData_[animationName] = NewMoonGame::GetModelManager()->GetAnimationData(animationName);
	skeleton_[animationName] = NewMoonGame::GetModelManager()->GetSkeletonData(animationName);
	skeleton_[animationName].value().name = animationName;
	skinCluster_[animationName] = NewMoonGame::GetModelManager()->GetSkinClusterData(animationName);

}