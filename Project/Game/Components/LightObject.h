#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/DXConstBuffer.h"
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"

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

	void ImGui();

	// Getter
	UINT GetRootParameterIndex(const PipelineType& pipelineType) const;

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	DirectionalLight directional;
	PointLight point;
	SpotLight spot;

	PunctualLight light;

};