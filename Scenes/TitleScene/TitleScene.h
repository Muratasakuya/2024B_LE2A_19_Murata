#pragma once

// Base
#include "IScene.h"

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

	void Initialize()override;
	void Update()override;
	void Draw()override;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/



};