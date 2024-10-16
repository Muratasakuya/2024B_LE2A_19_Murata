#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Scenes/Methods/IScene.h"

// c++
#include <memory>

/*////////////////////////////////////////////////////////////////////////////////
*								TitleScene Class
////////////////////////////////////////////////////////////////////////////////*/
class TitleScene :
	public IScene {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	TitleScene();
	~TitleScene();

	void Init()override;
	void Update()override;
	void Draw()override;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/



};