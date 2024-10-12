#include "RailEditor.h"

#include "Engine/Managers/ImGuiManager.h"
#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*									Main
////////////////////////////////////////////////////////////////////////////////*/
void RailEditor::Init() {

	// 使用するテクスチャの読み込み
	NewMoonGame::LoadTexture(sphereTexture_);

	// スプライン曲線の頂点表示用の球の共通マテリアル
	sphereMaterial_.Init();
}
void RailEditor::Update() {

	// スプライン曲線の頂点設定
	SetCatmullRomVertices();

	for (auto& spherePair : spheres_) {
		auto& sphereWorldTransform = spherePair.second;

		sphereWorldTransform.Update();
	}
	sphereMaterial_.Update();
}
void RailEditor::Draw() {

	for (auto& spherePair : spheres_) {
		auto& sphere = spherePair.first;

		sphere->Draw();
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*							スプライン曲線の頂点設定
////////////////////////////////////////////////////////////////////////////////*/
void RailEditor::SetCatmullRomVertices() {
#ifdef _DEBUG
	ImGui::Begin("RailEditor");

	// 頂点座標入力
	ImGui::DragFloat3("RailPoints", &spherePos_.x, 0.05f);
	if (spherePos_ != Vector3(0.0f, 0.0f, 0.0f)) {
		if (ImGui::Button("PointCreate")) {

			// レール座標に追加
			railPoints_.push_back(spherePos_);

			// 頂点表示用球の作成
			WorldTransform worldTransform;
			worldTransform.Init();
			// ST設定
			worldTransform.translation = spherePos_;
			worldTransform.scale.SetInit(sphereScale_);

			auto sphere = std::make_unique<Sphere>();
			sphere->Init(sphereTexture_);
			sphere->SetConstBuffers(worldTransform, sphereMaterial_);

			// 追加
			spheres_.push_back(std::make_pair(std::move(sphere), worldTransform));

			// 入力座標リセット
			spherePos_.Init();
		}
	}

	ImGui::End();
#endif // DEBUG
}
