#include "WorldTransform.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"

void WorldTransform::Init() {

	// SRT
	scale.SetInit(1.0f);
	rotation.Init();
	translation.Init();

	// 定数バッファ生成
	DXConstBuffer::Init(NewMoon::GetDXDevice());
}

void WorldTransform::Update(const Matrix4x4& viewPro) {

	// アニメーションを使用しない普通の更新
	if (!isPlayAnimation_) {

		Matrix4x4 worldMatrix =
			Matrix4x4::MakeAffineMatrix(scale, rotation, translation);
		if (parent_) {

			worldMatrix = Matrix4x4::Multiply(worldMatrix, parent_->matrix.World);
		}
		Matrix4x4 wvpMatrix = worldMatrix * viewPro;
		Matrix4x4 worldInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(worldMatrix));

		matrix.World = worldMatrix;
		matrix.WVP = wvpMatrix;
		matrix.WorldInverseTranspose = worldInverseTranspose;
	}
	// アニメーション再生
	else if (isPlayAnimation_) {

		// 時刻を進める
		animationtime_ += 1.0f / 60.0f;
		// リピート再生
		animationtime_ = std::fmod(animationtime_, animationData_.duration);

		// rootNodeのAnimationを取得
		// keyframesがなければデフォルト値
		NodeAnimation& rootNodeAnimation = animationData_.nodeAnimations[modelData_.rootNode.name];

		/*-------------------------------------------------------------------------------*/
		/// ↓ Animationのところが汚いので綺麗にする(いつか)

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

	// 定数バッファにデータを転送
	TransferData(matrix);

}

/*////////////////////////////////////////////////////////////////////////////////
*									Setter
////////////////////////////////////////////////////////////////////////////////*/
void WorldTransform::SetAnimationData(const std::string& modelName, const std::string& animationName) {

	modelData_ = NewMoonGame::GetModelMangager()->GetModelData(modelName);
	animationData_ = NewMoonGame::GetModelMangager()->GetAnimationData(animationName);
	skeleton_ = NewMoonGame::GetModelMangager()->GetSkeletonData(animationName);
	skeleton_.value().name = animationName;
	skinCluster_ = NewMoonGame::GetModelMangager()->GetSkinClusterData(animationName);

}
void WorldTransform::SetPlayAnimation(bool isPlayAnimation) { isPlayAnimation_ = isPlayAnimation; }

UINT WorldTransform::GetRootParameterIndex() const {

	return 1;
}