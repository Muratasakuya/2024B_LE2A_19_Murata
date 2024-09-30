#pragma once

// DirectX
#include <d3d12.h>
#include <dxcapi.h>
#include "ComPtr.h"
#include "Logger.h"
#include "PipelineStateStructure.h"

// C++
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

	void Initialize(DXCommon* dxCommon, const PipelineType& pipelineType);
	void Initialize(DXCommon* dxCommon, const ComputePipelineType& cspipelineType);

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