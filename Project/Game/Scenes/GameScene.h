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
	//							public Functions
	//===================================================================*/

	GameScene();
	~GameScene();

	// Main -> IScene
	void Init()override;
	void Update()override;
	void Draw()override;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	std::unique_ptr<RailEditor> railEditor_;

	std::unique_ptr<Player> player_;

};