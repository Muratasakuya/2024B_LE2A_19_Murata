#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/DXConstBuffer.h"

//* bufferSize
struct StaticMeshForGPU {

	Matrix4x4 WVP;
	Vector4 color;
};

/*////////////////////////////////////////////////////////////////////////////////
*							StaticMeshBuffer Class
////////////////////////////////////////////////////////////////////////////////*/
class StaticMeshBuffer :
	public DXConstBuffer<StaticMeshForGPU> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	StaticMeshBuffer() = default;
	~StaticMeshBuffer() = default;

	void Init(UINT instanceCount);

	void Update();

	UINT GetRootParameterIndex() const;

public:
	//===================================================================*/
	//							public Variable
	//===================================================================*/

	std::vector<StaticMeshForGPU> properties;

};