#include "DXOffscreenRender.h"

/*////////////////////////////////////////////////////////////////////////////////
*							RenderTextureの作成
////////////////////////////////////////////////////////////////////////////////*/
ComPtr<ID3D12Resource> DXOffscreenRender::CreateRenderTextureResource(ID3D12Device* device, uint32_t width, uint32_t height,
	DXGI_FORMAT format, const Vector4& clearColor) {

	HRESULT hr;

	// RenderTargetで設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width;                                   // 横幅
	resourceDesc.Height = height;                                 // 縦幅
	resourceDesc.Format = format;                                 // フォーマット設定
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; // RenderTargetとして利用
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;  // 2Dテクスチャで設定
	resourceDesc.SampleDesc.Count = 1;                            // サンプリングカウント。1固定
	resourceDesc.DepthOrArraySize = 1;                            // 配列サイズまたは深度を1に設定
	resourceDesc.MipLevels = 1;                                   // 1で設定、しなくてもdefaultで1になるらしい

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// 色
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;

	// Resourceの作成
	ComPtr<ID3D12Resource> resource = nullptr;
	hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_RENDER_TARGET, // これから描画することを前提にしたTexture
		&clearValue,                        // Clear最適値。ClearRenderTargetをこの色でClear
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));

	return resource;
}
