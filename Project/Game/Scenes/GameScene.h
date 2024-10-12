#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Scenes/Methods/IScene.h"
#include "Game/Editor/RailEditor.h"

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

	GameScene() = default;
	~GameScene() = default;

	// Main -> IScene
	void Init()override;
	void Update()override;
	void Draw()override;

	void ImGui();

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	// test
	Vector3 pointA_;
	Vector3 pointB_;

	Vector4 color_;
	
};