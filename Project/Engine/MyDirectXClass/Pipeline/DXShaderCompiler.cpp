#include "DXShaderCompiler.h"

#include "Engine/Base/DXCommon.h"

IDxcBlob* DXShaderCompiler::CompileShader(const std::wstring& filePath,
	const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler) {

	HRESULT hr;

	///////////////////////////////////////////////////////////
	// 1.hlslファイルを読み込む
	///////////////////////////////////////////////////////////

	// hlslファイルを読み込む
	IDxcBlobEncoding* shaderSouce = nullptr;
	hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSouce);
	// 読めなかったら止める
	assert(SUCCEEDED(hr));
	// 読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSouce->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSouce->GetBufferSize();
	// UTF8の文字コードであることを通知
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	///////////////////////////////////////////////////////////
	// 2.Compileする
	///////////////////////////////////////////////////////////

	LPCWSTR arguments[] = {
		filePath.c_str(),          // コンパイル対象のファイル名
		L"-E",L"main",             // エントリーポイントの指定、基本的にmain以外にはしない
		L"-T",profile,             // ShaderProfileの設定
		L"-Zi",L"-Qembed_debug",   // デバッグ用の情報を埋め込む
		L"-Od",                    // 最適化を外しておく
		L"-Zpr",                   // メモリレイアウトは行優先
	};
	// 実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,          // 読み込んだファイル
		arguments,                    // コンパイルオプション
		_countof(arguments),          // コンパイルオプションの数
		includeHandler,               // includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)   // コンパイル結果
	);
	// コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));

	///////////////////////////////////////////////////////////
	// 3.警告、エラーが出ていないか確認する
	///////////////////////////////////////////////////////////

	// 警告、エラーが出たらログに出して止める
	ComPtr<IDxcBlobUtf8> shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		// 警告、エラー
		assert(false);
	}

	///////////////////////////////////////////////////////////
	// 4.Complie結果を受け取って返す
	///////////////////////////////////////////////////////////

	// コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	// もう使わないリソースを解放
	shaderSouce->Release();
	shaderResult->Release();
	shaderError.Reset();
	// 実行用のバイナリを返却
	return shaderBlob;
}

/*////////////////////////////////////////////////////////////////////////////////
*								 初期化
////////////////////////////////////////////////////////////////////////////////*/
void DXShaderCompiler::Init(DXCommon* dxCommon, const PipelineType& pipelineType) {

	if (pipelineType == Primitive) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//												Primitive Shader
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Primitive.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Primitive.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == Object2D) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//												 Object2D Shader
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Object2D.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Object2D.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == pObject3D || pipelineType == RailNoneBackCulling) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//												 Object3D Shader
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Object3D.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Object3D.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == Object3DUnTex) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//												Object3DUnTex Shader
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Object3D.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Object3DUnTex.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == SkinningObject3D) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//												SkinningObject3D Shader
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/SkinningObject3D.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Object3D.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == NormalParticle) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//												  Particle Shader
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Particle.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Particle.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == DissolveParticle) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//												DissolveParticle Shader
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Particle.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/DissolveParticle.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == PrimitiveLine) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//												  PrimitiveLine Shader
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/PrimitiveLine.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/PrimitiveLine.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == OffscreenCopy) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//												 opyImage Shader /Fullscreen.VS
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Fullscreen.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/CopyImage.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == OffscreenGrayscale) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//												Grayscale Shader /Fullscreen.VS
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Fullscreen.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Grayscale.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == OffscreenSepiaTone) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//												SepiaTone Shader /Fullscreen.VS
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Fullscreen.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/SepiaTone.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == OffscreenVignette) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//												Vignette Shader /Fullscreen.VS
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Fullscreen.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Vignette.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == OffscreenSmoothing) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//												BoxFilter Shader /Fullscreen.VS
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Fullscreen.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/BoxFilter.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == OffscreenGaussianFilter) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//												GaussianFilter Shader /Fullscreen.VS
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Fullscreen.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/GaussianFilter.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == OffscreenLuminanceOutline) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//											LuminanceBasedOutline Shader /Fullscreen.VS
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Fullscreen.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/LuminanceBasedOutline.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == OffscreenOutline) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//											DepthBasedOutline Shader /Fullscreen.VS
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Fullscreen.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/DepthBasedOutline.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == OffscreenRadialBlur) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//											RadialBlur Shader /Fullscreen.VS
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Fullscreen.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/RadialBlur.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == OffscreenDissolve) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//											Dissolve Shader /Fullscreen.VS
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Fullscreen.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Dissolve.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == WaveBort) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//												WakeBoard Shader
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/WakeBoard.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/WakeBoard.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == pStaticMesh) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//												StaticMesh Shader
		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/StaticMesh.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/StaticMesh.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);

		// 成功したログを出す
		Log(ConvertWString(std::format(L"Complie Succeeded\n")));
	}
}
void DXShaderCompiler::Init(DXCommon* dxCommon, const ComputePipelineType& cspipelineType) {

	if (cspipelineType == SkinningCS) {

		/*------------------------------------------------------------------------------------------------------------------------*/
		//											Skinning.CS
		/*------------------------------------------------------------------------------------------------------------------------*/

		// コンピュートシェーダ
		csBlob_[cspipelineType] = CompileShader(L"./Resources/Shaders/Skinning.CS.hlsl", L"cs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(csBlob_[cspipelineType] != nullptr);
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
IDxcBlob* DXShaderCompiler::GetVSBlob(const PipelineType& pipelineType) const { return vsBlob_[pipelineType].Get(); }
IDxcBlob* DXShaderCompiler::GetPSBlob(const PipelineType& pipelineType) const { return psBlob_[pipelineType].Get(); }
IDxcBlob* DXShaderCompiler::GetCSBlob(const ComputePipelineType& cspipelineType) const { return csBlob_[cspipelineType].Get(); }