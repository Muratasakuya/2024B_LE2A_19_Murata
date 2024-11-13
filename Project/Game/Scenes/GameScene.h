#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Scenes/Methods/IScene.h"

//* gameObjects
#include "Game/Objects/TestCollisionObject.h"

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

	void Draw();

	void Cleanup() override;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	const std::string& baseModelDirectory_ = "./Resources/Obj";

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	void Load();

};