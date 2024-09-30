#include "DXConstBufferManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*									RootParameterの取得
////////////////////////////////////////////////////////////////////////////////*/
template<typename T>
constexpr UINT DXConstBufferManager::GetRootParameterIndex(const PipelineType& pipelineType) {

	if constexpr (std::is_same_v<T, TransformationMatrix>) {
		if (pipelineType == PipelineType::pObject3D||
			pipelineType == PipelineType::Object3DUnTex||
			pipelineType == PipelineType::SkinningObject3D) {

			return 1;
		}
	} else if constexpr (std::is_same_v<T, Material3D>) {
		if (pipelineType == PipelineType::pObject3D ||
			pipelineType == PipelineType::Object3DUnTex ||
			pipelineType == PipelineType::SkinningObject3D) {

			return 0;
		}
	} else if constexpr (std::is_same_v<T, PunctualLight>) {
		if (pipelineType == PipelineType::pObject3D||
			pipelineType == PipelineType::SkinningObject3D) {

			return 3;
		}
		if (pipelineType == PipelineType::Object3DUnTex) {

			return 2;
		}
	} else if constexpr (std::is_same_v<T, CameraForGPU>) {
		if (pipelineType == PipelineType::pObject3D ||
			pipelineType == PipelineType::SkinningObject3D) {

			return 4;
		}
		if (pipelineType == PipelineType::Object3DUnTex) {

			return 3;
		}
	}

	return UINT_MAX;
}

/*////////////////////////////////////////////////////////////////////////////////
*									templateMembers
////////////////////////////////////////////////////////////////////////////////*/

template void DXConstBufferManager::SetCommands<DXConstBuffer<TransformationMatrix>&>
(ID3D12GraphicsCommandList*, const PipelineType& pipelineType, DXConstBuffer<TransformationMatrix>&);

template void DXConstBufferManager::SetCommands<DXConstBuffer<Material3D>&>
(ID3D12GraphicsCommandList*, const PipelineType& pipelineType, DXConstBuffer<Material3D>&);

template void DXConstBufferManager::SetCommands<DXConstBuffer<PunctualLight>&>
(ID3D12GraphicsCommandList*, const PipelineType& pipelineType, DXConstBuffer<PunctualLight>&);

template void DXConstBufferManager::SetCommands<DXConstBuffer<CameraForGPU>&>
(ID3D12GraphicsCommandList*, const PipelineType& pipelineType, DXConstBuffer<CameraForGPU>&);

template<>
void DXConstBufferManager::ProcessBuffer(ID3D12GraphicsCommandList* commandList,
	const PipelineType& pipelineType, DXConstBuffer<TransformationMatrix>& buffer) {

	UINT rootParameterIndex = GetRootParameterIndex<TransformationMatrix>(pipelineType);
	buffer.SetCommand(commandList, rootParameterIndex);
}

template<>
void DXConstBufferManager::ProcessBuffer(ID3D12GraphicsCommandList* commandList,
	const PipelineType& pipelineType, DXConstBuffer<Material3D>& buffer) {

	UINT rootParameterIndex = GetRootParameterIndex<Material3D>(pipelineType);
	buffer.SetCommand(commandList, rootParameterIndex);
}

template<>
void DXConstBufferManager::ProcessBuffer(ID3D12GraphicsCommandList* commandList,
	const PipelineType& pipelineType, DXConstBuffer<PunctualLight>& buffer) {

	UINT rootParameterIndex = GetRootParameterIndex<PunctualLight>(pipelineType);
	buffer.SetCommand(commandList, rootParameterIndex);
}

template<>
void DXConstBufferManager::ProcessBuffer(ID3D12GraphicsCommandList* commandList,
	const PipelineType& pipelineType, DXConstBuffer<CameraForGPU>& buffer) {

	UINT rootParameterIndex = GetRootParameterIndex<CameraForGPU>(pipelineType);
	buffer.SetCommand(commandList, rootParameterIndex);
}