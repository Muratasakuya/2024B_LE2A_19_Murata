#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/DXConstBuffer.h"

/*////////////////////////////////////////////////////////////////////////////////
*							ParticleBuffer Class
////////////////////////////////////////////////////////////////////////////////*/
class ParticleBuffer :
	public DXConstBuffer<ParticleForGPU> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	ParticleBuffer() = default;
	~ParticleBuffer() = default;

	// Main
	void Init(UINT instanceCount);
	void Update();

	// Getter
	UINT GetRootParameterIndex() const;

public:
	//===================================================================*/
	//							public Variable
	//===================================================================*/

	std::vector<ParticleForGPU> properties;

};