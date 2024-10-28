#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Scenes/Methods/IScene.h"
#include "Game/Editor/RailEditor.h"
#include "Game/Enetities/Player.h"
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

	void Cleanup() override;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	std::unique_ptr<RailEditor> railEditor_;

	std::unique_ptr<Player> player_;

	std::unique_ptr<TestCollisionObject> collisionObject_;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	void Load();

	void Draw();

};