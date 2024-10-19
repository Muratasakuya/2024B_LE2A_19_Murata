#include "AnimationTransform.h"

#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*						AnimationTransform classMethods
////////////////////////////////////////////////////////////////////////////////*/

void AnimationTransform::Init(const std::string& modelName, const std::string& animationName) {

	BaseTransform::Init();

	animationData_ = NewMoonGame::GetModelMangager()->GetAnimationData(animationName);
	skeleton_ = NewMoonGame::GetModelMangager()->GetSkeletonData(animationName);
	skeleton_.value().name = animationName;
	skinCluster_ = NewMoonGame::GetModelMangager()->GetSkinClusterData(animationName);

	modelData_ = NewMoonGame::GetModelMangager()->GetModelData(modelName);

	isPlayAnimation_ = false;
	animationtime_ = 0.0f;
}

void AnimationTransform::Update(const Matrix4x4& viewPro) {

	if (isPlayAnimation_) {

		//* repeat
		animationtime_ += NewMoonGame::GetDeltaTime();
		animationtime_ = std::fmod(animationtime_, animationData_.duration);

		NodeAnimation& rootNodeAnimation = animationData_.nodeAnimations[modelData_.rootNode.name];

		Vector3 animationTranslate;
		Quaternion animationRotate;
		Vector3 animationScale;

		// keyframesがないときはデフォルト値を入れる
		if (!rootNodeAnimation.translate.keyframes.empty()) {

			animationTranslate = Vector3::CalculateValue(rootNodeAnimation.translate.keyframes, animationtime_);
		} else if (rootNodeAnimation.translate.keyframes.empty()) {

			animationTranslate = Vector3(0.0f, 0.0f, 0.0f);
		}
		if (!rootNodeAnimation.translate.keyframes.empty()) {

			animationRotate = Quaternion::CalculateValue(rootNodeAnimation.rotate.keyframes, animationtime_);
		} else if (rootNodeAnimation.rotate.keyframes.empty()) {

			animationRotate = Quaternion::IdentityQuaternion();
		}
		if (!rootNodeAnimation.scale.keyframes.empty()) {

			animationScale = Vector3::CalculateValue(rootNodeAnimation.scale.keyframes, animationtime_);
		} else if (rootNodeAnimation.scale.keyframes.empty()) {

			animationScale = Vector3(1.0f, 1.0f, 1.0f);
		}

		Matrix4x4 localMatrix = Matrix4x4::MakeAxisAffineMatrix(animationScale, animationRotate, animationTranslate);

		// スケルトンが存在する場合
		if (skeleton_) {

			// アニメーション適応
			NewMoonGame::ApplyAnimation(skeleton_.value().name, animationtime_);
			// スケルトンの更新
			NewMoonGame::SkeletonUpdate(skeleton_.value().name);
			// スキンクラスターの更新
			NewMoonGame::SkinClusterUpdate(skeleton_.value().name);
		}

		Matrix4x4 worldMatrix = Matrix4x4::MakeAffineMatrix(scale, rotation, translation);
		Matrix4x4 wvpMatrix = worldMatrix * viewPro;
		Matrix4x4 worldInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(worldMatrix));

		matrix.World = worldMatrix;
		matrix.WVP = wvpMatrix;
		matrix.WorldInverseTranspose = worldInverseTranspose;
	}

	TransferData(matrix);

}

void AnimationTransform::SetPlayAnimation(bool isPlayAnimation) {
	isPlayAnimation_ = isPlayAnimation;
}
