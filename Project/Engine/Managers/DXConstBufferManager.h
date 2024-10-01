#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/DXConstBuffer.h"
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"
#include "Game/Components/WorldTransform.h"
#include "Game/Components/MaterialObject.h"
#include "Game/Components/LightObject.h"
#include "Game/Components/CameraObject.h"

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