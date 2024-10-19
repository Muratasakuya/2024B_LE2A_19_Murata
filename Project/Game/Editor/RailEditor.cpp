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
	sphereMaterial_.color = { 1.0f,0.0f,0.0f,1.0f };

	NewMoonGame::SetToEditor(this);
}
void RailEditor::Update() {

	for (auto& spherePair : spheres_) {
		auto& sphereWorldTransform = spherePair.second;

		sphereWorldTransform.Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());
	}
	sphereMaterial_.Update();
}
void RailEditor::Draw() {

	DrawRailLine();

	if (NewMoonGame::GameCamera()->GetRailCamera()->IsStart()) {
		return;
	}

	for (auto& spherePair : spheres_) {
		auto& sphere = spherePair.first;
		auto& transform = spherePair.second;

		sphere->Draw(transform, sphereMaterial_);
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*							スプライン曲線の頂点設定
////////////////////////////////////////////////////////////////////////////////*/
void RailEditor::ImGui() {
#ifdef _DEBUG
	ImGui::Separator();

	// 頂点座標の入力
	if (ImGui::CollapsingHeader("Vertex Input", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text("Set position for the next vertex:");
		ImGui::DragFloat3("RailPoints", &spherePos_.x, 0.05f);
		if (ImGui::Button("Create Point")) {
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

			// 追加
			spheres_.push_back(std::make_pair(std::move(sphere), worldTransform));

			// 入力座標リセット
			spherePos_.Init();
		}
	}

	ImGui::Separator();

	if (1 < spheres_.size()) {
		if (ImGui::CollapsingHeader("Edit Vertices", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Text("Adjust existing vertices:");

			int index = 0;
			for (auto& sphere : spheres_) {
				std::string label = "Vertex " + std::to_string(index);

				if (ImGui::TreeNode(label.c_str())) {
					ImGui::DragFloat3("Position", &sphere.second.translation.x, 0.01f);
					railPoints_[index] = sphere.second.translation;
					ImGui::TreePop();
				}
				index++;
			}
		}
	}
#endif // _DEBUG
}

/*////////////////////////////////////////////////////////////////////////////////
*							Catmull-Rom Methods
////////////////////////////////////////////////////////////////////////////////*/
Vector3 RailEditor::Interpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {

	const float s = 0.5f;

	float t2 = t * t;
	float t3 = t2 * t;

	Vector3 e3 = -p0 + 3.0f * p1 - 3.0f * p2 + p3;
	Vector3 e2 = 2.0f * p0 - 5.0f * p1 + 4 * p2 - p3;
	Vector3 e1 = -p0 + p2;
	Vector3 e0 = 2.0f * p1;

	return s * (e3 * t3 + e2 * t2 + e1 * t + e0);
}
Vector3 RailEditor::SetCatmullRomPos(const std::vector<Vector3>& points, float t) {

	// エラーチェック
	assert(points.size() >= 4 && "制御点は4点以上必要です");

	// 区間数は制御点の数-1
	size_t division = points.size() - 1;
	// 1区間の長さ (全体を1.0fとした割合)
	float areaWidth = 1.0f / division;

	// 区間内の始点を0.0f、終点を1.0fとしたときの現在位置
	float t_2 = std::fmod(t, areaWidth) * division;
	// 下限(0.0f)と上限(1.0f)の範囲に収める
	t_2 = std::clamp(t_2, 0.0f, 1.0f);

	// 区間番号
	size_t index = static_cast<size_t>(t / areaWidth);
	// 区間番号が上限を超えないように収める
	index = std::clamp(index, static_cast<size_t>(0), points.size() - 2);

	// 4点分のインデックス
	size_t index0 = index - 1;
	size_t index1 = index;
	size_t index2 = index + 1;
	size_t index3 = index + 2;

	// 最初の区間のp0はp1を重複使用する
	if (index == 0) {

		index0 = index;
	}

	// 最後の区間のp3はp2を重複使用する
	if (index3 >= points.size()) {

		index3 = index2;
	}

	// 4点の座標
	const Vector3& p0 = points[index0];
	const Vector3& p1 = points[index1];
	const Vector3& p2 = points[index2];
	const Vector3& p3 = points[index3];

	// 4点を指定して補完
	return Interpolation(p0, p1, p2, p3, t_2);
}

void RailEditor::DrawRailLine() {

	// 4以上のみ
	if (spheres_.size() < 4) {
		return;
	}

	std::vector<Vector3> pointsDrawing;

	// 線分の数+1個分の頂点座標を計算
	for (size_t i = 0; i < segmentCount_ + 1; ++i) {

		float t = 1.0f / segmentCount_ * i;
		Vector3 pos = SetCatmullRomPos(railPoints_, t);

		// 描画用頂点リストに追加
		pointsDrawing.push_back(pos);
	}

	// 先頭から2点ずつ取り出してラインを描画
	for (size_t i = 0; i < segmentCount_; ++i) {

		Vector3 start = pointsDrawing[i];
		Vector3 end = pointsDrawing[i + 1];

		NewMoonGame::DrawLine(start, end, LineColor::Red);
	}
}

const std::vector<Vector3>& RailEditor::GetControlPoints() const { return railPoints_; }