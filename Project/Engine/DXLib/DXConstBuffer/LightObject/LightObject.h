#pragma once

// DXLib
#include "DXConstBuffer.h"

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

	void Initialize();
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