#pragma once

// Base
#include "IScene.h"

/*////////////////////////////////////////////////////////////////////////////////
*								GameScene Class
////////////////////////////////////////////////////////////////////////////////*/
class GameScene :
	public IScene {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	GameScene() = default;
	~GameScene() = default;

	// Main -> IScene
	void Initialize()override;
	void Update()override;
	void Draw()override;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	
};