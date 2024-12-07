#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/DXConstBuffer.h"

//* bufferSize
struct TriangleDataForGPU {

	Vector4 vertex0;
	Vector4 vertex1;
	Vector4 vertex2;
};

/*////////////////////////////////////////////////////////////////////////////////
*							TriangleVertexBuffer Class
////////////////////////////////////////////////////////////////////////////////*/
class TriangleVertexBuffer :
	public DXConstBuffer<TriangleDataForGPU> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	TriangleVertexBuffer() = default;
	~TriangleVertexBuffer() = default;

	void Init(UINT instanceCount);

	void Update();

	UINT GetRootParameterIndex() const;

public:
	//===================================================================*/
	//							public Variable
	//===================================================================*/

	std::vector<TriangleDataForGPU> properties;

};