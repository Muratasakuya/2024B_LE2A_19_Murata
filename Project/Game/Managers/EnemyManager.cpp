#include "EnemyManager.h"

#include "Engine/Base/NewMoonGame.h"
#include "Game/Managers/CameraManager.h"
#include "Game/Editor/RailEditor.h"
#include "Lib/Adapter/Random.h"

/*////////////////////////////////////////////////////////////////////////////////
*							EnemyManager classMethods
////////////////////////////////////////////////////////////////////////////////*/

void EnemyManager::Init(RailEditor* railEditor) {

	railCamera_ = NewMoonGame::GameCamera()->GetRailCamera();

	railEditor_ = railEditor;
	spawnInfo_ = {

		{EnemyType::kBalloon,0,0.1f,0.148f,0.215f,12},

		{EnemyType::kGhost,0,0.24f,0.3f,0.31f,5},

		{EnemyType::kBalloon,1,0.227f,0.325f,0.325f,3},

		{EnemyType::kGhost,1,0.39f,0.405f,0.475f,3},

		{EnemyType::kGhost,2,0.48f,0.49f,0.51f,3},

		{EnemyType::kGhost,3,0.485f,0.494f,0.505f,3},

		{EnemyType::kUFO,0,0.515f,0.52f,0.55f,10},

		// ここからラストスパート
		// 第一陣
		{EnemyType::kBalloon,2,0.68f,0.7f,0.8f,6},
		{EnemyType::kGhost,4,0.68f,0.7f,0.8f,5},
		{EnemyType::kUFO,1,0.68f,0.7f,0.8f,6},

		// ラスト
		{EnemyType::kBalloon,3,0.8f,0.85f,0.9f,8},
		{EnemyType::kGhost,5,0.8f,0.85f,0.9f,9},
		{EnemyType::kUFO,2,0.8f,0.85f,0.9f,3},

	};

	spawn_ = std::nullopt;

	score_ = 0.0f;

}

void EnemyManager::Update() {

	// 発生処理
	Generate();

	for (const auto& enemy : enemies_) {

		enemy->Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());

		if (!enemy->IsAlive()) {

			score_ += enemy->GetScore();
		}
	}

	enemies_.remove_if([](const std::unique_ptr<Enemy>& enemy) {
		if (!enemy->IsAlive()) {

			NewMoonGame::RemoveCollider(enemy.get());
			return true;
		} else {
			return false;
		}
		});

}

void EnemyManager::Draw() {

	for (const auto& enemy : enemies_) {

		enemy->Draw();
	}
}

void EnemyManager::Generate() {

	for (auto& info : spawnInfo_) {

		if (railCamera_->GetReparamT() >= info.spawnTime && info.spawnTime) {
			for (uint32_t index = 0; index < info.spawnNum; ++index) {

				// 出現場所TのIndex
				float posIndex = Random::Generate(info.spawnTStart, info.spawnTEnd);
				Vector3 pos = railEditor_->SetCatmullRomPos(railEditor_->GetControlPoints(), posIndex);
				// 参照渡しで座標をセットする
				SetGeneratePos(info, index, pos);

				std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
				newEnemy->Init(info.enemyType, pos);
				enemies_.push_back(std::move(newEnemy));
			}

			// 1度だけ発生させる
			info.spawnTime = std::nullopt;

		}
	}
}

void EnemyManager::SetGeneratePos(const SpawnInfo& info, uint32_t index, Vector3& pos) {

	index;

	switch (info.enemyType) {
	case EnemyType::kBalloon: {

		// ほんの少しだけ上にしてそのあと風船自体を上昇させる
		const float yOffset = 0.4f;

		// 最初の出現座標
		if (info.typeId == 0 || info.typeId == 2 || info.typeId == 3) {

			pos.y += yOffset; //* 上にあげる
			pos.x += Random::Generate(-3.0f, 3.0f); //* 左右にずらす
			pos.z += Random::Generate(-1.0f, 1.0f);
		}

		// 二回目の出現座標
		if (info.typeId == 1) {

			pos.y += yOffset; //* 上にあげる
		}
	}
							break;
	case EnemyType::kUFO: {

		// 最初の出現座標
		if (info.typeId == 0 || info.typeId == 1 || info.typeId == 2) {

			pos.y += Random::Generate(0.4f, 1.5f);  //* 上にあげる、これもランダム
			pos.x += Random::Generate(-3.0f, 3.0f); //* 左右にずらす
			pos.z += Random::Generate(-1.0f, 1.0f);
		}
	}
						break;
	case EnemyType::kGhost: {

		// 最初の出現座標
		if (info.typeId == 0) {

			pos.y += Random::Generate(0.2f, 0.6f);  //* 上にあげる、これもランダム
			pos.x += Random::Generate(-4.0f, 0.0f); //* 左右にずらす
			pos.z += Random::Generate(-4.0f, 0.0f); //* 左右にずらす
		}

		// 2回目の出現座標
		if (info.typeId == 1) {

			pos.y += Random::Generate(0.2f, 0.6f);  //* 上にあげる、これもランダム
			pos.x += Random::Generate(0.0f, 0.3f); //* 左右にずらす
			pos.z += Random::Generate(0.0f, 0.3f); //* 左右にずらす
		}

		// 3回目の出現座標
		if (info.typeId == 2) {

			pos.y += Random::Generate(0.2f, 0.6f);  //* 上にあげる、これもランダム
			pos.x += Random::Generate(-0.3f, 0.0f); //* 左右にずらす
			pos.z += Random::Generate(-0.3f, 0.0f); //* 左右にずらす
		}

		// 3回目の出現座標
		if (info.typeId == 3) {

			pos.y += Random::Generate(0.2f, 0.6f);  //* 上にあげる、これもランダム
			pos.x += Random::Generate(-0.3f, 0.0f); //* 左右にずらす
			pos.z += Random::Generate(-0.3f, 0.0f); //* 左右にずらす
		}

		// 4,5回目の出現座標
		if (info.typeId == 4 || info.typeId == 5) {

			pos.y += Random::Generate(0.2f, 0.6f);  //* 上にあげる、これもランダム
			pos.x += Random::Generate(-1.0f, 1.0f); //* 左右にずらす
			pos.z += Random::Generate(-1.0f, 1.0f); //* 左右にずらす
		}
	}
						  break;
	}
}