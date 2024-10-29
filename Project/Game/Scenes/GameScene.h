#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Scenes/Methods/IScene.h"
#include "Game/Editor/RailEditor.h"
#include "Game/Entities/Player.h"
#include "Game/Objects/TestCollisionObject.h"

//* object
#include "Game/3D/Particle.h"

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

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	void Load();

	void Draw();

};