#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Scenes/Methods/IScene.h"

//* gameObjects
#include "Game/Editor/RailEditor.h"
#include "Game/Entities/Player.h"
#include "Game/Objects/TestCollisionObject.h"
#include "Game/Managers/EnemyManager.h"
#include "Game/Objects/Skydome.h"

// c++
#include <memory>

/*////////////////////////////////////////////////////////////////////////////////
*								GameScene Class
////////////////////////////////////////////////////////////////////////////////*/
class GameScene :
	public IScene {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	GameScene();
	~GameScene();

	void Run() override;

	void Init() override;

	void Update() override;

	void Cleanup() override;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	const std::string& baseModelDirectory_ = "./Resources/Obj";

	std::unique_ptr<RailEditor> railEditor_;

	std::unique_ptr<Player> player_;

	std::unique_ptr<EnemyManager> enemyManager_;

	std::unique_ptr<Skydome> skydome_;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	void Load();

	void Draw();

};