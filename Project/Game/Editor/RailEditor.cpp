#include "RailEditor.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"
#include "Engine/Managers/ModelManager.h"
#include "Lib/Adapter/JsonAdapter.h"

/*////////////////////////////////////////////////////////////////////////////////
*									Main
////////////////////////////////////////////////////////////////////////////////*/
void RailEditor::Init() {

	// レールサイズ
	railSize_ = 1.0f;

	// スプライン曲線の頂点表示用の球の共通マテリアル
	sphereMaterial_.Init();
	sphereMaterial_.color = { 1.0f,0.0f,0.0f,1.0f };

	// Defaultのレール
	defaultRailPoints_.resize(8); // 適当な数
	for (size_t index = 0; index < defaultRailPoints_.size(); ++index) {

		defaultRailPoints_[index] = Vector3(0.0f, 0.0f + index * 0.4f, 0.0f + index * 4.0f);
	}

	NewMoonGame::SetToEditor(this);

	LoadRailPoints();
}
void RailEditor::Update() {

	if (rail_) {

		rail_->Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());
	}

	for (auto& spherePair : spheres_) {
		auto& sphereWorldTransform = spherePair.second;

		sphereWorldTransform.Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());
	}
	sphereMaterial_.Update();
}
void RailEditor::Draw() {

	if (rail_) {

		rail_->Draw();
	}

	if (NewMoonGame::GameCamera()->GetRailCamera()->IsStart()) {
		return;
	}

	DrawRailLine();

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
		if (railPoints_.empty()) {

			ImGui::Text("set position for the next vertex:");
		} else {

			ImGui::Text("preRailPoint : %4.1f, %4.1f, %4.1f",railPoints_.back().x, railPoints_.back().y, railPoints_.back().z);
		}
		ImGui::DragFloat3("RailPoints", &spherePos_.x, 0.05f);
		if (ImGui::Button("SetDefaultPoints")) {

			railPoints_ = defaultRailPoints_;

			for (size_t index = 0; index < railPoints_.size(); ++index) {

				// 頂点表示用球の作成
				WorldTransform worldTransform;
				worldTransform.Init();
				// ST設定
				worldTransform.translation = railPoints_[index];
				worldTransform.scale.SetInit(sphereScale_);

				auto sphere = std::make_unique<Sphere>();
				sphere->Init(sphereTexture_);

				// 追加
				spheres_.push_back(std::make_pair(std::move(sphere), worldTransform));
			}
		}
		ImGui::SameLine();
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
		if (!rail_) {
			if (3 < spheres_.size()) {
				if (ImGui::Button("Create RailModel")) {

					CreateRailModel();
				}
			}
		}
		if (rail_) {
			if (ImGui::Button("Reset Rail")) {

				rail_.reset();
				railPoints_.clear();
			}
		}
		ImGui::SameLine();
		if (!railPoints_.empty()) {
			if (ImGui::Button("Save Rail")) {

				SaveRailPoints();
			}
		}
		ImGui::SameLine();
	}

	ImGui::Separator();

	if (1 < spheres_.size()) {
		if (ImGui::CollapsingHeader("Edit Vertices", ImGuiTreeNodeFlags_DefaultOpen)) {

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

		NewMoonGame::DrawLine3D(start, end, LineColor::Red);
	}
}

const std::vector<Vector3>& RailEditor::GetControlPoints() const { return railPoints_; }

/*////////////////////////////////////////////////////////////////////////////////
*							RailModelの自作
////////////////////////////////////////////////////////////////////////////////*/
void RailEditor::CreateRailModel() {

	// 頂点データを保持
	std::vector<VertexData3D> allVertexData;

	Vector3 prevTopVertex2 = Vector3::Zero();    // 前回の上面の左上頂点
	Vector3 prevTopVertex3 = Vector3::Zero();    // 前回の上面の右上頂点
	Vector3 prevBottomVertex2 = Vector3::Zero(); // 前回の下面の左上頂点
	Vector3 prevBottomVertex3 = Vector3::Zero(); // 前回の下面の右上頂点
	bool isFirstSegment = true;                  // 最初のモデルかどうか

	const float railHeight = 0.25f;               // レールの厚み

	for (size_t index = 0; index < segmentCount_; ++index) {

		// セグメントの始点と終点を取得
		float t0 = 1.0f / segmentCount_ * index;
		float t1 = 1.0f / segmentCount_ * (index + 1);

		Vector3 p0 = SetCatmullRomPos(railPoints_, t0);
		Vector3 p1 = SetCatmullRomPos(railPoints_, t1);

		// 方向ベクトル
		Vector3 direction = p1 - p0;
		direction = Vector3::Normalize(direction);

		// 上向きのベクトル
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
		Vector3 right = Vector3::Cross(up, direction);
		right = Vector3::Normalize(right);

		// 上面の頂点位置を計算
		Vector3 topVertex0 = p0 + right * (railSize_ * 0.5f); // 左下（上面）
		Vector3 topVertex1 = p0 - right * (railSize_ * 0.5f); // 右下（上面）
		Vector3 topVertex2 = p1 - right * (railSize_ * 0.5f); // 右上（上面）
		Vector3 topVertex3 = p1 + right * (railSize_ * 0.5f); // 左上（上面）

		// 下面の頂点位置を計算
		Vector3 bottomVertex0 = topVertex0 - up * railHeight; // 左下（下面）
		Vector3 bottomVertex1 = topVertex1 - up * railHeight; // 右下（下面）
		Vector3 bottomVertex2 = topVertex2 - up * railHeight; // 右上（下面）
		Vector3 bottomVertex3 = topVertex3 - up * railHeight; // 左上（下面）

		// 法線ベクトルの計算
		Vector3 edge1 = topVertex1 - topVertex0;
		Vector3 edge2 = topVertex3 - topVertex0;
		Vector3 normal = Vector3::Cross(edge1, edge2);
		normal = Vector3::Normalize(normal);

		Vector2 texcoord0(0.0f, 0.0f); // 左下
		Vector2 texcoord1(1.0f, 0.0f); // 右下
		Vector2 texcoord2(1.0f, 1.0f); // 右上
		Vector2 texcoord3(0.0f, 1.0f); // 左上

		if (isFirstSegment) {

			// 最初はそのまま頂点座標を設定する
			allVertexData.push_back(VertexData3D{ Vector4(topVertex0.x, topVertex0.y, topVertex0.z, 1.0f), texcoord0, normal });
			allVertexData.push_back(VertexData3D{ Vector4(topVertex1.x, topVertex1.y, topVertex1.z, 1.0f), texcoord1, normal });
			allVertexData.push_back(VertexData3D{ Vector4(topVertex2.x, topVertex2.y, topVertex2.z, 1.0f), texcoord2, normal });
			allVertexData.push_back(VertexData3D{ Vector4(topVertex3.x, topVertex3.y, topVertex3.z, 1.0f), texcoord3, normal });

			allVertexData.push_back(VertexData3D{ Vector4(bottomVertex0.x, bottomVertex0.y, bottomVertex0.z, 1.0f), texcoord0, normal });
			allVertexData.push_back(VertexData3D{ Vector4(bottomVertex1.x, bottomVertex1.y, bottomVertex1.z, 1.0f), texcoord1, normal });
			allVertexData.push_back(VertexData3D{ Vector4(bottomVertex2.x, bottomVertex2.y, bottomVertex2.z, 1.0f), texcoord2, normal });
			allVertexData.push_back(VertexData3D{ Vector4(bottomVertex3.x, bottomVertex3.y, bottomVertex3.z, 1.0f), texcoord3, normal });

			isFirstSegment = false;
		} else {

			// 2個目のモデル以降は前回の頂点を使う
			allVertexData.push_back(VertexData3D{ Vector4(prevTopVertex2.x, prevTopVertex2.y, prevTopVertex2.z, 1.0f), texcoord0, normal });
			allVertexData.push_back(VertexData3D{ Vector4(prevTopVertex3.x, prevTopVertex3.y, prevTopVertex3.z, 1.0f), texcoord1, normal });
			allVertexData.push_back(VertexData3D{ Vector4(topVertex2.x, topVertex2.y, topVertex2.z, 1.0f), texcoord2, normal });
			allVertexData.push_back(VertexData3D{ Vector4(topVertex3.x, topVertex3.y, topVertex3.z, 1.0f), texcoord3, normal });

			allVertexData.push_back(VertexData3D{ Vector4(prevBottomVertex2.x, prevBottomVertex2.y, prevBottomVertex2.z, 1.0f), texcoord0, normal });
			allVertexData.push_back(VertexData3D{ Vector4(prevBottomVertex3.x, prevBottomVertex3.y, prevBottomVertex3.z, 1.0f), texcoord1, normal });
			allVertexData.push_back(VertexData3D{ Vector4(bottomVertex2.x, bottomVertex2.y, bottomVertex2.z, 1.0f), texcoord2, normal });
			allVertexData.push_back(VertexData3D{ Vector4(bottomVertex3.x, bottomVertex3.y, bottomVertex3.z, 1.0f), texcoord3, normal });
		}

		// 今回のセグメントの終点を次回のために保存
		prevTopVertex2 = topVertex3;
		prevTopVertex3 = topVertex2;
		prevBottomVertex2 = bottomVertex3;
		prevBottomVertex3 = bottomVertex2;
	}

	// モデルを作成
	std::string modelName = "Rail";
	uint32_t modelId = 0;
	NewMoonGame::GetModelMangager()->MakeRailModel(modelName, modelId, allVertexData);

	// モデル追加
	std::unique_ptr<Rail> rail = std::make_unique<Rail>();
	rail->Init(modelName + std::to_string(modelId), railTextureName_);
	rail_ = std::move(rail);
}

void RailEditor::SaveRailPoints() {

	Json data;
	for (const auto& point : railPoints_) {

		data[railPointsLabel_].push_back(JsonAdapter::FromVector3(point));
	}
	JsonAdapter::Save(railPointsJsonPath_, data);
}

void RailEditor::LoadRailPoints() {

	railPoints_.clear();

	Json data = JsonAdapter::Load(railPointsJsonPath_);

	if (data.contains(railPointsLabel_)) {

		for (const auto& item : data[railPointsLabel_]) {

			railPoints_.push_back(JsonAdapter::ToVector3(item));
		}
	}

	for (size_t index = 0; index < railPoints_.size(); ++index) {

		// 頂点表示用球の作成
		WorldTransform worldTransform;
		worldTransform.Init();
		// ST設定
		worldTransform.translation = railPoints_[index];
		worldTransform.scale.SetInit(sphereScale_);

		auto sphere = std::make_unique<Sphere>();
		sphere->Init(sphereTexture_);

		// 追加
		spheres_.push_back(std::make_pair(std::move(sphere), worldTransform));
	}

	// レール作成
	CreateRailModel();
}