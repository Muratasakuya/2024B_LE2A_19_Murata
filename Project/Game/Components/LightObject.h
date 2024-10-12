#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/DXConstBuffer.h"

/*////////////////////////////////////////////////////////////////////////////////
*								LightObject Class
////////////////////////////////////////////////////////////////////////////////*/
class LightObject :
	public DXConstBuffer<PunctualLight> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	LightObject() = default;
	~LightObject() = default;

	void Init();
	void Update();

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	DirectionalLight directional;
	PointLight point;
	SpotLight spot;

	PunctualLight light;

};