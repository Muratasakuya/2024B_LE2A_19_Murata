#include "WorldTransform.h"

#include "Engine/Base/NewMoon.h"

void WorldTransform::Initialize() {

	// SRT
	scale.SetInitialize(1.0f);
	rotation.Initialize();
	translation.Initialize();

	// 定数バッファ生成
	DXConstBuffer::Initialize(NewMoon::GetDXDevice());
}

void WorldTransform::Update(MainCamera3D* camera3D) {

	// アニメーションを使用しない普通の更新
	if (!isPlayAnimation_) {

		Matrix4x4 worldMatrix =
			Matrix4x4::MakeAffineMatrix(scale, rotation, translation);
		Matrix4x4 wvpMatrix = worldMatrix * camera3D->GetViewProjectionMatrix();
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
			NewMoon::ApplyAnimation(skeleton_.value().name, animationtime_);
			// スケルトンの更新
			NewMoon::SkeletonUpdate(skeleton_.value().name);
			// スキンクラスターの更新
			NewMoon::SkinClusterUpdate(skeleton_.value().name);
		}

		Matrix4x4 worldMatrix = Matrix4x4::MakeAffineMatrix(scale, rotation, translation);
		Matrix4x4 wvpMatrix = worldMatrix * camera3D->GetViewProjectionMatrix();
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

	modelData_ = NewMoon::GetModelData(modelName);
	animationData_ = NewMoon::GetAnimationData(animationName);
	skeleton_ = NewMoon::GetSkeletonData(animationName);
	skeleton_.value().name = animationName;
	skinCluster_ = NewMoon::GetSkinClusterData(animationName);

}
void WorldTransform::SetPlayAnimation(bool isPlayAnimation) { isPlayAnimation_ = isPlayAnimation; }