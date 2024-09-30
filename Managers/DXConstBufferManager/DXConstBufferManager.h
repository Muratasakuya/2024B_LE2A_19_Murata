#pragma once

// DXLib
#include "DXConstBuffer.h"
#include "PipelineStateStructure.h"

// MainObject
#include "WorldTransform.h"
#include "MaterialObject.h"
#include "LightObject.h"
#include "CameraObject.h"

/*////////////////////////////////////////////////////////////////////////////////
*								DXConstBufferManager Class
////////////////////////////////////////////////////////////////////////////////*/
class DXConstBufferManager {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	DXConstBufferManager() = default;
	~DXConstBufferManager() = default;

	template<typename... Args>
	void SetCommands(ID3D12GraphicsCommandList* commandList, const PipelineType& pipelineType, Args&&... args);

private:
	//===================================================================*/
	//							private Functions
	//===================================================================*/

	template<typename T>
	constexpr UINT GetRootParameterIndex(const PipelineType& pipelineType);
	template<typename T>
	void ProcessBuffer(ID3D12GraphicsCommandList* commandList, const PipelineType& pipelineType, DXConstBuffer<T>& buffer);

};

///===============================================================================
/// template関数定義

template<typename ...Args>
void DXConstBufferManager::SetCommands(ID3D12GraphicsCommandList* commandList, const PipelineType& pipelineType, Args&&... args) {

	(ProcessBuffer(commandList, pipelineType, std::forward<Args>(args)), ...);
}

///===============================================================================