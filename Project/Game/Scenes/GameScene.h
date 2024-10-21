#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Scenes/Methods/IScene.h"
#include "Game/Editor/RailEditor.h"
#include "Game/Enetities/Player.h"

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

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	void BackDraw();  // 背景
	void FrontDraw(); // 前景

};