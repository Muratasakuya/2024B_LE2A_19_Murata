#include "RailScore.h"

#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Cylinder classMethods
////////////////////////////////////////////////////////////////////////////////*/

void RailScore::Init() {

	score_ = std::make_unique<Sprite>();
	score_->Init("score");
	score_->SetCenterAnchor();
	scorePos_ = Vector2(940.0f, 620.0f);
	score_->SetPos(scorePos_);

	for (uint32_t index = 0; index < scoreNumbers_.size(); ++index) {

		scoreNumbers_[index] = std::make_unique<Sprite>();
		scoreNumbers_[index]->Init("scoreNumber"); // 0123456789の連番画像

		scoreNumbers_[index]->AjustTextureSize(false); // テクスチャ本来の画像サイズではなくする
	}

	scoreBack_ = std::make_unique<Sprite>();
	scoreBack_->Init("scoreBack");

	scoreBackPos_ = Vector2(1024.0f, 620.0f);
	scoreBack_->SetPos(scoreBackPos_);

}

void RailScore::Update() {

	ImGui();

	score_->Update();

	// スコアを4桁の文字列に変換
	int scoreInt = static_cast<int>(scoreValue_);
	std::string scoreStr = std::to_string(scoreInt);
	while (scoreStr.length() < 4) {
		scoreStr = "0" + scoreStr;  // 4桁になるように0埋め
	}

	// 数字1つ分のテクスチャサイズ
	const float texWidth = 416.0f / 10.0f; // 数字1つ分の幅
	const float texHeight = 96.0f;         // 画像の高さ
	const Vector2 texSize = { texWidth, texHeight };

	// 各桁ごとに表示位置とテクスチャを設定
	for (size_t i = 0; i < 4; ++i) {  // 4桁に固定
		int digit = scoreStr[i] - '0';

		// 各桁の数字に対応するテクスチャの左上座標を計算
		Vector2 texBaseMin = { texWidth * digit, 0.0f };
		scoreNumbers_[i]->SetTextureLeftTop(texBaseMin);
		scoreNumbers_[i]->SetOriginalTextureSize(texSize);
		scoreNumbers_[i]->SetTextureSize(texSize);

		// 各数字の表示位置を設定
		Vector2 numbersPos = Vector2(1060.0f + i * texWidth, 624.0f);
		scoreNumbers_[i]->SetPos(numbersPos);

		// 各数字のスプライトを更新
		scoreNumbers_[i]->Update();
	}

	scoreBack_->Update();

}

void RailScore::Draw() {

	scoreBack_->Draw();

	score_->Draw();

	for (uint32_t index = 0; index < scoreNumbers_.size(); ++index) {

		scoreNumbers_[index]->Draw();
	}
}

void RailScore::ImGui() {
#ifdef _DEBUG
	ImGui::Begin("HUD");

	ImGui::DragFloat2("scoreBackPos_", &scoreBackPos_.x, 1.0f);

	ImGui::End();
#endif // _DEBUG
}

void RailScore::SetScore(float score) { scoreValue_ = score; }