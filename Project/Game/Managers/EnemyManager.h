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

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	RailEditor* railEditor_ = nullptr;

	std::list<std::unique_ptr<Enemy>> enemies_;

	//===================================================================*/
	///* functions

	void Generate();

};