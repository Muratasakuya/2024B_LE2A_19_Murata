#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Entities/Enemy.h"
#include "Game/Editor/RailEditor.h"

// c++
#include <vector>
#include <memory>
#include <list>
#include <optional>

//* front
class RailCamera;

/*////////////////////////////////////////////////////////////////////////////////
*							EnemyManager Class
////////////////////////////////////////////////////////////////////////////////*/
class EnemyManager {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	EnemyManager() = default;
	~EnemyManager() = default;

	void Init(RailEditor* railEditor);

	void Update();

	void Draw();

	void Reset() {
		score_ = 0.0f;
	}

	// Getter
	float GetScore() const { return score_; }

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	struct SpawnInfo {

		EnemyType enemyType;            //* 敵のタイプ
		uint32_t typeId;                //* 敵のタイプのID
		std::optional<float> spawnTime; //* 出現する時間
		float spawnTStart;              //* 出現する間隔
		float spawnTEnd;                //* 出現する間隔
		uint32_t spawnNum;              //* 出現する数
	};

	//===================================================================*/
	///* variables

	RailCamera* railCamera_ = nullptr;
	RailEditor* railEditor_ = nullptr;

	std::list<std::unique_ptr<Enemy>> enemies_;

	std::vector<SpawnInfo> spawnInfo_;

	std::optional<float> spawn_;

	float score_;

	//===================================================================*/
	///* functions

	void Generate();

	// 各タイプごとの出現設定
	void SetGeneratePos(const SpawnInfo& info, uint32_t index, Vector3& pos);

};