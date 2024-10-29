#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"
#include "Game/Components/VertexObject.h"
#include "Game/Components/IndexObject.h"

// c++
#include <memory>
#include <vector>

/*////////////////////////////////////////////////////////////////////////////////
*							BaseModel Class
////////////////////////////////////////////////////////////////////////////////*/
class BaseModel {
private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//* IA *//
	struct InputAssembler {

		std::vector<VertexObject<VertexData3D>> vertices;
		std::vector<UINT> verticesNum;

		std::vector<IndexObject> indices;
		std::vector<UINT> indicesNum;

		void SetBuffer(ID3D12GraphicsCommandList* commandList, uint32_t meshIndex = 0) {

			commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			commandList->IASetVertexBuffers(0, 1, &vertices[meshIndex].GetVertexBuffer());
			commandList->IASetIndexBuffer(&indices[meshIndex].GetIndexBuffer());
		}
		void DrawCall(ID3D12GraphicsCommandList* commandList, uint32_t meshIndex = 0) {

			commandList->DrawIndexedInstanced(static_cast<UINT>(indices[meshIndex].data.size()), 1, 0, 0, 0);
		}
	};

public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	BaseModel() = default;
	virtual ~BaseModel() = default;

	virtual void Init(const std::string& modelName);

	// Setter
	void SetPipelineType(const PipelineType& pipelineType);

protected:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	size_t meshNum_;
	ModelData modelData_;
	PipelineType pipelineType_;
	std::optional<PipelineType> setPipelineType_;

	InputAssembler inputAssembler_;

};