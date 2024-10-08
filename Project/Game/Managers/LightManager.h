#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Components/LightObject.h"

/*////////////////////////////////////////////////////////////////////////////////
*								LightManager Class
////////////////////////////////////////////////////////////////////////////////*/
class LightManager {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	LightManager() = default;
	~LightManager() = default;

	void Initialize();
	void Update();

	// Getter
	LightObject GetLightBuffer() const;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	LightObject light_;

};