#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Components/VertexObject.h"
#include "Game/Components/IndexObject.h"

// c++
#include <vector>

/*////////////////////////////////////////////////////////////////////////////////
*							InputAssembler Class
////////////////////////////////////////////////////////////////////////////////*/
class InputAssembler {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	InputAssembler() = default;
	~InputAssembler() = default;

	void Init(const MeshModelData& meshData);

	void SetBuffer(ID3D12GraphicsCommandList* commandList, uint32_t meshIndex);

	void DrawCall(ID3D12GraphicsCommandList* commandList, uint32_t meshIndex);
	void DrawCall(ID3D12GraphicsCommandList* commandList, uint32_t instanceNum, uint32_t meshIndex);

	// Getter
	VertexObject<VertexData3D>& GetVertexData() { return vertices.front(); }
	IndexObject& GetIndexData() { return indices.front(); }

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	std::vector<VertexObject<VertexData3D>> vertices;

	std::vector<IndexObject> indices;

};