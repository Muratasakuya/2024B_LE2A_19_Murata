#include "DXRootSignature.h"

#include "Engine/Base/DXCommon.h"

/*////////////////////////////////////////////////////////////////////////////////
*							DXRootSignatureの生成
////////////////////////////////////////////////////////////////////////////////*/
void DXRootSignature::CreateGraphicsRootSignature(DXCommon* dxCommon, PipelineType pipelineType) {

	HRESULT hr;

	if (pipelineType == Primitive) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[2]{};

		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;                             // レジスタ番号0とバインド

		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // VertexShaderで使う
		rootParameters[1].Descriptor.ShaderRegister = 0;                              // レジスタ番号0とバインド

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&graphicsRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	} else if (pipelineType == Object2D) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// descriptorRangeの設定
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0;                      // 0から始まる t0
		descriptorRange[0].NumDescriptors = 1;                          // 数は1つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[3]{};

		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;    // PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド

		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;  // VertexShaderで使う
		rootParameters[1].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド

		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;      // DescriptorTableを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                // PixelShaderで使う
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;             // Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// Samplerの設定
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         // バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       // 0~1の範囲外をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       // ありったけのMipMapを使う
		staticSamplers[0].ShaderRegister = 0;                               // レジスタ番号0を使う
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		descriptionRootSignature.pStaticSamplers = staticSamplers;
		descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&graphicsRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	} else if (pipelineType == pObject3D || pipelineType == RailNoneBackCulling) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// descriptorRangeの設定
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0;                      // 0から始まる t0
		descriptorRange[0].NumDescriptors = 1;                          // 数は1つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[5]{};

		// gMaterial
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;    // PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド
		// gTransformationMatrix
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;  // VertexShaderで使う
		rootParameters[1].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド
		// gTexture
		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;      // DescriptorTableを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                // PixelShaderで使う
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;             // Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数
		// gPunctual
		rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;   // PixelShaderで使う
		rootParameters[3].Descriptor.ShaderRegister = 1;                      // レジスタ番号1とバインド
		// gCamera
		rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;   // PixelShaderで使う
		rootParameters[4].Descriptor.ShaderRegister = 2;                      // レジスタ番号2とバインド

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// Samplerの設定
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         // バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       // 0~1の範囲外をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       // ありったけのMipMapを使う
		staticSamplers[0].ShaderRegister = 0;                               // レジスタ番号0を使う
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		descriptionRootSignature.pStaticSamplers = staticSamplers;
		descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&graphicsRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	} else if (pipelineType == Object3DUnTex) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[4]{};

		// gMaterial
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;    // PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド
		// gTransformationMatrix
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;  // VertexShaderで使う
		rootParameters[1].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド
		// gPunctual
		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;   // PixelShaderで使う
		rootParameters[2].Descriptor.ShaderRegister = 1;                      // レジスタ番号1とバインド
		// gCamera
		rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;   // PixelShaderで使う
		rootParameters[3].Descriptor.ShaderRegister = 2;                      // レジスタ番号2とバインド

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&graphicsRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	} else if (pipelineType == SkinningObject3D) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// descriptorRangeの設定
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0;                      // 0から始まる t0
		descriptorRange[0].NumDescriptors = 1;                          // 数は1つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

		// descriptorRangeForInstancingの設定
		D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
		descriptorRangeForInstancing[0].BaseShaderRegister = 0;                      // 0から始まる t0
		descriptorRangeForInstancing[0].NumDescriptors = 1;                          // 数は1つ
		descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                                    // Offsetを自動計算

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[6]{};

		// gMaterial
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;    // PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド
		// gTransformationMatrix
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;  // VertexShaderで使う
		rootParameters[1].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド
		// gTexture
		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;      // DescriptorTableを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                // PixelShaderで使う
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;             // Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数
		// gPunctual
		rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;   // PixelShaderで使う
		rootParameters[3].Descriptor.ShaderRegister = 1;                      // レジスタ番号1とバインド
		// gCamera
		rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;   // PixelShaderで使う
		rootParameters[4].Descriptor.ShaderRegister = 2;                      // レジスタ番号2とバインド
		// gMatrixPalette
		rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;                   // DescriptorTableを使う
		rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;                            // VertexShaderで使う
		rootParameters[5].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;             // Tableの中身の配列を指定
		rootParameters[5].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing); // Tableで利用する数

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// Samplerの設定
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         // バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       // 0~1の範囲外をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       // ありったけのMipMapを使う
		staticSamplers[0].ShaderRegister = 0;                               // レジスタ番号0を使う
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		descriptionRootSignature.pStaticSamplers = staticSamplers;
		descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&graphicsRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	} else if (pipelineType == pParticle) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// descriptorRangeの設定
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0;                      // 0から始まる t0
		descriptorRange[0].NumDescriptors = 1;                          // 数は1つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

		// descriptorRangeForInstancingの設定
		D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
		descriptorRangeForInstancing[0].BaseShaderRegister = 0;                      // 0から始まる t0
		descriptorRangeForInstancing[0].NumDescriptors = 1;                          // 数は1つ
		descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[3]{};

		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;  // VertexShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド

		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;      // DescriptorTableを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                // PixelShaderで使う
		rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRange;             // Tableの中身の配列を指定
		rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数

		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;                   // DescriptorTableを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;                            // VertexShaderで使う
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;             // Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing); // Tableで利用する数

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// Samplerの設定
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         // バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       // 0~1の範囲外をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       // ありったけのMipMapを使う
		staticSamplers[0].ShaderRegister = 0;                               // レジスタ番号0を使う
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		descriptionRootSignature.pStaticSamplers = staticSamplers;
		descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&graphicsRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	} else if (pipelineType == PrimitiveLine) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[2]{};

		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;    // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;                    // レジスタ番号0とバインド

		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;     // CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // VertexShaderで使う
		rootParameters[1].Descriptor.ShaderRegister = 0;                     // レジスタ番号0とバインド

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&graphicsRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	} else if (
		pipelineType == OffscreenCopy || pipelineType == OffscreenGrayscale ||
		pipelineType == OffscreenSepiaTone || pipelineType == OffscreenVignette ||
		pipelineType == OffscreenSmoothing || pipelineType == OffscreenGaussianFilter ||
		pipelineType == OffscreenLuminanceOutline || pipelineType == OffscreenRadialBlur) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// descriptorRangeの設定
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0;                      // 0から始まる t0
		descriptorRange[0].NumDescriptors = 1;                          // 数は1つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[1]{};

		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;      // DescriptorTableを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                // PixelShaderで使う
		rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;             // Tableの中身の配列を指定
		rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// Samplerの設定
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         // バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       // 0~1の範囲外をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       // ありったけのMipMapを使う
		staticSamplers[0].ShaderRegister = 0;                               // レジスタ番号0を使う
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		descriptionRootSignature.pStaticSamplers = staticSamplers;
		descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&graphicsRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	} else if (pipelineType == OffscreenOutline) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// descriptorRangeの設定
		D3D12_DESCRIPTOR_RANGE descriptorRange[2] = {};
		descriptorRange[0].BaseShaderRegister = 0;                      // 0から始まる t0
		descriptorRange[0].NumDescriptors = 1;                          // 数は1つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

		descriptorRange[1].BaseShaderRegister = 1;                      // 0から始まる t1
		descriptorRange[1].NumDescriptors = 1;                          // 数は1つ
		descriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange[1].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[2]{};

		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;      // DescriptorTableを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                // PixelShaderで使う
		rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;             // Tableの中身の配列を指定
		rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数

		// gMaterial
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;    // CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		rootParameters[1].Descriptor.ShaderRegister = 0;                    // レジスタ番号0とバインド

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// Samplerの設定
		D3D12_STATIC_SAMPLER_DESC staticSamplers[2] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         // バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       // 0~1の範囲外をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       // ありったけのMipMapを使う
		staticSamplers[0].ShaderRegister = 0;                               // レジスタ番号0を使う
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		descriptionRootSignature.pStaticSamplers = staticSamplers;

		staticSamplers[1].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;         // ポイントフィルタ
		staticSamplers[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;      // 0~1の範囲外をリピート
		staticSamplers[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[1].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[1].MaxLOD = D3D12_FLOAT32_MAX;                       // ありったけのMipMapを使う
		staticSamplers[1].ShaderRegister = 1;                               // レジスタ番号1を使う
		staticSamplers[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		descriptionRootSignature.pStaticSamplers = staticSamplers;

		descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&graphicsRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	} else if (pipelineType == OffscreenDissolve) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// descriptorRangeの設定
		D3D12_DESCRIPTOR_RANGE descriptorRangeRender[1] = {};
		descriptorRangeRender[0].BaseShaderRegister = 0;                      // 0から始まる t0
		descriptorRangeRender[0].NumDescriptors = 1;                          // 数は1つ
		descriptorRangeRender[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRangeRender[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                             // Offsetを自動計算

		D3D12_DESCRIPTOR_RANGE descriptorRangeMaskTexture[1] = {};
		descriptorRangeMaskTexture[0].BaseShaderRegister = 1;                      // 0から始まる t1
		descriptorRangeMaskTexture[0].NumDescriptors = 1;                          // 数は1つ
		descriptorRangeMaskTexture[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRangeMaskTexture[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                                  // Offsetを自動計算

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[3]{};

		// RenderTexture
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;            // DescriptorTableを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                      // PixelShaderで使う
		rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRangeRender;             // Tableの中身の配列を指定
		rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeRender); // Tableで利用する数

		//MaskTexture
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;                 // DescriptorTableを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                           // PixelShaderで使う
		rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRangeMaskTexture;             // Tableの中身の配列を指定
		rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeMaskTexture); // Tableで利用する数

		// gMaterial
		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;    // CBVを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		rootParameters[2].Descriptor.ShaderRegister = 0;                    // レジスタ番号0とバインド

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// Samplerの設定
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         // バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       // 0~1の範囲外をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       // ありったけのMipMapを使う
		staticSamplers[0].ShaderRegister = 0;                               // レジスタ番号0を使う
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う

		descriptionRootSignature.pStaticSamplers = staticSamplers;
		descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&graphicsRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	} else if (pipelineType == WaveBort) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// descriptorRangeの設定
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0;                      // 0から始まる t0
		descriptorRange[0].NumDescriptors = 1;                          // 数は1つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

		D3D12_DESCRIPTOR_RANGE descriptorRange2[1] = {};
		descriptorRange2[0].BaseShaderRegister = 1;                      // 0から始まる t1
		descriptorRange2[0].NumDescriptors = 1;                          // 数は1つ
		descriptorRange2[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange2[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

		D3D12_DESCRIPTOR_RANGE descriptorRange3[1] = {};
		descriptorRange3[0].BaseShaderRegister = 2;                      // 0から始まる t2
		descriptorRange3[0].NumDescriptors = 1;                          // 数は1つ
		descriptorRange3[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange3[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[8]{};

		// gMaterial
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;    // PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド
		// gTransformationMatrix
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;  // VertexShaderで使う
		rootParameters[1].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド
		// gTextureBaseTexture
		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;      // DescriptorTableを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                // PixelShaderで使う
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;             // Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数
		// gPunctual
		rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;   // PixelShaderで使う
		rootParameters[3].Descriptor.ShaderRegister = 1;                      // レジスタ番号1とバインド
		// gCamera
		rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;   // PixelShaderで使う
		rootParameters[4].Descriptor.ShaderRegister = 2;                      // レジスタ番号2とバインド
		// gWaveParameters
		rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;  // VertexShaderで使う
		rootParameters[5].Descriptor.ShaderRegister = 1;                      // レジスタ番号1とバインド
		// gTextureBlueTexture
		rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;      // DescriptorTableを使う
		rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                // PixelShaderで使う
		rootParameters[6].DescriptorTable.pDescriptorRanges = descriptorRange2;             // Tableの中身の配列を指定
		rootParameters[6].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange2); // Tableで利用する数
		// gTextureWhiteTexture
		rootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;      // DescriptorTableを使う
		rootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                // PixelShaderで使う
		rootParameters[7].DescriptorTable.pDescriptorRanges = descriptorRange3;             // Tableの中身の配列を指定
		rootParameters[7].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange3); // Tableで利用する数


		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// Samplerの設定
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         // バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       // 0~1の範囲外をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       // ありったけのMipMapを使う
		staticSamplers[0].ShaderRegister = 0;                               // レジスタ番号0を使う
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		descriptionRootSignature.pStaticSamplers = staticSamplers;
		descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&graphicsRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	}
}
void DXRootSignature::CreateComputeRootSignature(DXCommon* dxCommon, ComputePipelineType csPipelineType) {

	HRESULT hr;

	if (csPipelineType == SkinningCS) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// descriptorRangeの設定
		// gMatrixPalette t0
		D3D12_DESCRIPTOR_RANGE matrixPaletteDescRange[1] = {};
		matrixPaletteDescRange[0].BaseShaderRegister = 0;                      // t0
		matrixPaletteDescRange[0].NumDescriptors = 1;                          // 1つ
		matrixPaletteDescRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRV
		matrixPaletteDescRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                              // Offsetを自動計算
		// gInputVertices t1
		D3D12_DESCRIPTOR_RANGE inputVerticesDescRange[1] = {};
		inputVerticesDescRange[0].BaseShaderRegister = 1;                      // t1
		inputVerticesDescRange[0].NumDescriptors = 1;                          // 1つ
		inputVerticesDescRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRV
		inputVerticesDescRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                              // Offsetを自動計算
		// gInfluences t2
		D3D12_DESCRIPTOR_RANGE influencesDescRange[1] = {};
		influencesDescRange[0].BaseShaderRegister = 2;                      // t2
		influencesDescRange[0].NumDescriptors = 1;                          // 1つ
		influencesDescRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRV
		influencesDescRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                           // Offsetを自動計算
		// gOutputVertices u0
		D3D12_DESCRIPTOR_RANGE outputVerticesDescRange[1] = {};
		outputVerticesDescRange[0].BaseShaderRegister = 0;                      // u0
		outputVerticesDescRange[0].NumDescriptors = 1;                          // 1つ
		outputVerticesDescRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV; // UAV
		outputVerticesDescRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                               // Offsetを自動計算

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[5]{};

		// gMatrixPalette t0
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[0].DescriptorTable.pDescriptorRanges = matrixPaletteDescRange;
		rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(matrixPaletteDescRange);
		// gInputVertices t1
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[1].DescriptorTable.pDescriptorRanges = inputVerticesDescRange;
		rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(inputVerticesDescRange);
		// gInfluences t2
		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[2].DescriptorTable.pDescriptorRanges = influencesDescRange;
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(influencesDescRange);
		// gOutputVertices u0
		rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[3].DescriptorTable.pDescriptorRanges = outputVerticesDescRange;
		rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(outputVerticesDescRange);
		// gSkinningInformation b0
		rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[4].Descriptor.ShaderRegister = 0;

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&computeRootSignature_[csPipelineType]));
		assert(SUCCEEDED(hr));
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
ID3D12RootSignature* DXRootSignature::GetGraphicsRootSignature(PipelineType pipelineType) const { return graphicsRootSignature_[pipelineType].Get(); }
ID3D12RootSignature* DXRootSignature::GetComputeRootSignature(ComputePipelineType csPipelineType) const { return computeRootSignature_[csPipelineType].Get(); }
