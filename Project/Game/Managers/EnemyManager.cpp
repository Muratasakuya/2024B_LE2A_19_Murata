#include "EnemyManager.h"

#include "Engine/Base/NewMoonGame.h"

#include "Game/Editor/RailEditor.h"

/*////////////////////////////////////////////////////////////////////////////////
*							EnemyManager classMethods
////////////////////////////////////////////////////////////////////////////////*/

void EnemyManager::Init(RailEditor* railEditor) {

	railEditor_ = railEditor;
	
	// 生成
	Generate();

}

void EnemyManager::Update() {

	for (const auto& enemy : enemies_) {

		enemy->Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());
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

	// 地点をこっちで設定し、その位置に出す
	std::vector<float> generatePointIndices = {
		0.38f,0.6f,0.63f,0.74f
	};
	// Yを上げる
	const float yOffset = 3.0f;

	for (uint32_t index = 0; index < generatePointIndices.size(); ++index) {

		Vector3 pos = railEditor_->SetCatmullRomPos(railEditor_->GetControlPoints(), generatePointIndices[index]);
		pos.y = pos.y + yOffset;

		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
		newEnemy->Init(pos);

		enemies_.push_back(std::move(newEnemy));

	}

}