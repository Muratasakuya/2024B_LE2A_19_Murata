#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/ComPtr.h"
#include "Engine/MyDirectXClass/Logger.h"
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"

// directX
#include <d3d12.h>
#include <dxcapi.h>

// c++
#include <array>
#include <string>
#include <cassert>

///===============================================================================
/// クラス前方宣言
class DXCommon;
///===============================================================================

/*////////////////////////////////////////////////////////////////////////////////
*							DXShaderCompiler Class
////////////////////////////////////////////////////////////////////////////////*/
class DXShaderCompiler {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	DXShaderCompiler() = default;
	~DXShaderCompiler() = default;

	void Init(DXCommon* dxCommon, const PipelineType& pipelineType);
	void Init(DXCommon* dxCommon, const ComputePipelineType& cspipelineType);

	// Getter
	IDxcBlob* GetVSBlob(const PipelineType& pipelineType) const;
	IDxcBlob* GetPSBlob(const PipelineType& pipelineType) const;
	IDxcBlob* GetCSBlob(const ComputePipelineType& cspipelineType) const;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	std::array<ComPtr<IDxcBlob>, pipelineTypeNum> vsBlob_;
	std::array<ComPtr<IDxcBlob>, pipelineTypeNum> psBlob_;
	std::array<ComPtr<IDxcBlob>, computePipelineTypeNum> csBlob_;

private:
	//===================================================================*/
	//							private Function
	//===================================================================*/

	IDxcBlob* CompileShader(
		// CompilerするShaderファイルへのパス
		const std::wstring& filePath,
		// Compilerに使用するProfile
		const wchar_t* profile,
		// 初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

};