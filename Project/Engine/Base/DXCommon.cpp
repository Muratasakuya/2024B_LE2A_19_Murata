#include "DXCommon.h"

// lib /* .hに書いてはいけない */
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

#include "Engine/Base/WinApp.h"
#include "Engine/Managers/SrvManager.h"

#pragma region /// Debug ///
/*////////////////////////////////////////////////////////////////////////////////
*							デバッグの表示、エラー警告
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::DebugLayer() {

#ifdef _DEBUG

	ComPtr<ID3D12Debug1> debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {

		// デバッグレイヤーを有効化する
		debugController->EnableDebugLayer();

		// さらにGPU側でもチェックを行うようにする
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif
}

/*////////////////////////////////////////////////////////////////////////////////
*							エラー、警告が出たら停止させる
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::DebugInfo() {

#ifdef _DEBUG

	ComPtr<ID3D12InfoQueue> infoQueue = nullptr;
	if (SUCCEEDED(device_->GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {

		// やばいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		// エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		// 警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		//抑制するメッセージのID
		D3D12_MESSAGE_ID denyIds[] = {

			// Windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			// https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};

		// 抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;

		// 指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
	}

#endif
}
#pragma endregion

/*////////////////////////////////////////////////////////////////////////////////
*							FenceとEventの生成
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::CreateFenceEvent() {

	HANDLE fenceEvent;

	// 初期値0でFenceを作る
	fence_ = nullptr;
	uint64_t fenceValue = 0;
	hr_ = device_->GetDevice()->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr_));

	// FenceのSignalを待つためのイベントの作成する
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
}

/*////////////////////////////////////////////////////////////////////////////////
*							 dxCompilerの初期化
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::InitDXCompiler() {

	dxcUtils_ = nullptr;
	dxcCompiler_ = nullptr;
	hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));

	// 現時点でincludeしないが、includeするための設定を行っておく
	includeHandler_ = nullptr;
	hr_ = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr_));
}

/*////////////////////////////////////////////////////////////////////////////////
*				commandQueue,commandAllocator,commandListの生成
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::CreateCommand() {

	commandQueue_ = nullptr;
	hr_ = device_->GetDevice()->CreateCommandQueue(&commandQueueDesc_, IID_PPV_ARGS(&commandQueue_));

	// コマンドキューの生成がうまくいったかどうか
	assert(SUCCEEDED(hr_));

	// コマンドアロケータを生成する
	commandAllocator_ = nullptr;
	hr_ = device_->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));

	// コマンドアロケータの生成がうまくいったかどうか
	assert(SUCCEEDED(hr_));

	// コマンドリストを生成する
	commandList_ = nullptr;
	hr_ = device_->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));

	// コマンドリストの生成がうまくいったかどうか
	assert(SUCCEEDED(hr_));
}

/*////////////////////////////////////////////////////////////////////////////////
*							TransitionBarrierの生成
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::TransitionBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter) {

	// 今回のバリアはTransition
	barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// Noneにしておく
	barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを貼る対象のリソース。引数で渡されたリソースに対して行う
	barrier_.Transition.pResource = resource;
	// 遷移前(現在)のResourceState
	barrier_.Transition.StateBefore = stateBefore;
	// 遷移後のResourceState
	barrier_.Transition.StateAfter = stateAfter;

	// TransitionBarrierを設定
	commandList_->ResourceBarrier(1, &barrier_);
}

/*////////////////////////////////////////////////////////////////////////////////
*								深度バッファクリア
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::ClearDepthBuffer() {

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = descriptor_->GetDSVDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();

	// 指定した深度で画面全体をクリアする、深度バッファクリア
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

/*////////////////////////////////////////////////////////////////////////////////
*								　画面のクリア
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::ClearWindow() {

	// 描画先のRTVとDSVを設定する

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	for (uint32_t i = 0; i < swapChain_->bufferCount; i++) {

		rtvHandles[i] = rtvManager_->GetCPUHandle(i);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = descriptor_->GetDSVDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();

	commandList_->OMSetRenderTargets(1, &rtvHandles[backBufferIndex_], false, &dsvHandle);
	// 指定した色で画面全体をクリアする
	//float clearColor[] = { 0.184f, 0.310f, 0.310f, 1.0f };
	float clearColor[] = { 0.016f, 0.016f, 0.016f, 1.0f };
	// RGBAの順
	commandList_->ClearRenderTargetView(rtvHandles[backBufferIndex_], clearColor, 0, nullptr);

	// 深度バッファクリア
	ClearDepthBuffer();
}

/*////////////////////////////////////////////////////////////////////////////////
*							RenderTextureの作成
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::CreateOffscreenRenderTexture(SrvManager* srvManager, uint32_t width, uint32_t height) {

	const Vector4 kRenderTargetClearColor = { 0.184f, 0.310f, 0.310f, 1.0f };
	auto device = device_->GetDevice();

	// Resourceの作成
	renderTextureResource_ =
		offscreenRender_->CreateRenderTextureResource(device, width, height,
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearColor);

	// RTVの設定
	uint32_t rtvIndex = rtvManager_->Allocate();
	rtvManager_->Create(rtvIndex, renderTextureResource_.Get());
	// メインループに向けてRTV使用インデックスリセット
	rtvManager_->Reset();

	// SRVの作成
	// Allocateは進めておく
	uint32_t srvIndex = srvManager->Allocate();
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = srvManager->GetCPUHandle(srvIndex);
	renderTextureGpuHandle_ = srvManager->GetGPUHandle(srvIndex);

	srvManager->CreateSRVForTexture2D(srvIndex, renderTextureResource_.Get(), DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);

	// Depth用のSRV
	uint32_t depthSrvIndex = srvManager->Allocate();
	D3D12_CPU_DESCRIPTOR_HANDLE depthCpuHandle = srvManager->GetCPUHandle(depthSrvIndex);
	depthGpuHandle_ = srvManager->GetGPUHandle(depthSrvIndex);

	srvManager->CreateSRVForDepthTexture2D(depthSrvIndex, descriptor_->GetDepthResource(), DXGI_FORMAT_R24_UNORM_X8_TYPELESS, 1);
}

/*////////////////////////////////////////////////////////////////////////////////
*						オフスクリーン描画前の準備処理
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::OffscreenPreDraw() {

	// RTVの設定
	uint32_t index = rtvManager_->Allocate();
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvManager_->GetCPUHandle(index);
	rtvManager_->Create(index, renderTextureResource_.Get());

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = descriptor_->GetDSVDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	commandList_->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	float kRenderTargetClearColor[] = { 0.184f, 0.310f, 0.310f, 1.0f };
	commandList_->ClearRenderTargetView(rtvHandle, kRenderTargetClearColor, 0, nullptr);
	// 指定した深度で画面全体をクリアする、深度バッファクリア
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// ビューポートの設定
	viewport_ =
		D3D12_VIEWPORT(0.0f, 0.0f, float(kClientWidth_), float(kClientHeight_), 0.0f, 1.0f);
	commandList_->RSSetViewports(1, &viewport_);

	// シザリング矩形の設定
	scissorRect_ = D3D12_RECT(0, 0, kClientWidth_, kClientHeight_);
	commandList_->RSSetScissorRects(1, &scissorRect_);

	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考える
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

/*////////////////////////////////////////////////////////////////////////////////
*						オフスクリーン描画後の後片付け処理
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::OffscreenPostDraw() {

	// リソースの状態をRENDER_TARGETからPIXEL_SHADER_RESOURCEに変更
	//TransitionBarrier(renderTextureResource_.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	// リソースの状態をD3D12_RESOURCE_STATE_DEPTH_WRITEからPIXEL_SHADER_RESOURCEに変更
	//TransitionBarrier(descriptor_->GetDepthResource(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

/*////////////////////////////////////////////////////////////////////////////////
*								描画前の準備処理
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::PreDraw() {

	// これから書き込むバックバッファのインデックスを取得
	backBufferIndex_ = swapChain_->Get()->GetCurrentBackBufferIndex();

	// バックバッファのリソース状態をPRESENTからRENDER_TARGETに変更
	TransitionBarrier(swapChain_->GetResources(backBufferIndex_), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// 画面のクリア
	ClearWindow();

	// ビューポートの設定
	viewport_ =
		D3D12_VIEWPORT(0.0f, 0.0f, float(kClientWidth_), float(kClientHeight_), 0.0f, 1.0f);
	commandList_->RSSetViewports(1, &viewport_);// Viewportを設定

	// シザリング矩形の設定
	scissorRect_ = D3D12_RECT(0, 0, kClientWidth_, kClientHeight_);
	commandList_->RSSetScissorRects(1, &scissorRect_);// Scirssorを設定

}

/*////////////////////////////////////////////////////////////////////////////////
*							 描画後の後片付け処理
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::PostDraw() {

	// 次のOffscreenに状態が合うように状態を変更
	//TransitionBarrier(descriptor_->GetDepthResource(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	//TransitionBarrier(renderTextureResource_.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// バックバッファの状態をRENDER_TARGETからPRESENTに変更 処理的には上のバリアと同じ
	TransitionBarrier(swapChain_->GetResources(backBufferIndex_), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	// コマンドリストの内容を確定させる。すべてのコマンドを積んでからCloseする
	hr_ = commandList_->Close();
	assert(SUCCEEDED(hr_));

	// GPUにコマンドリストの実行を行わせる
	ID3D12CommandList* commandLists[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandLists);

	// GPUとOSに画面の交換を行うように通知する
	swapChain_->Get()->Present(1, 0);

	// Feneceの値を更新
	fenceValue_++;
	// GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
	commandQueue_->Signal(fence_.Get(), fenceValue_);

	// Fenceの値が指定したSignal値にたどり着いているか確認する
	if (fence_->GetCompletedValue() < fenceValue_) {

		// 指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
		// イベントを待つ
		WaitForSingleObject(fenceEvent_, INFINITE);

		// このあとでGPUの実行を待ってからResetが呼び出される
	}

	// FPS固定
	UpdateFixFPS();

	// 次のフレーム用のコマンドリストを準備
	hr_ = commandAllocator_->Reset();
	assert(SUCCEEDED(hr_));
	hr_ = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(hr_));

	// RTV使用インデックスリセット
	rtvManager_->Reset();
}

/*////////////////////////////////////////////////////////////////////////////////
*							コマンドを実行して完了を待つ
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::WaitForGPU() {

	// コマンドリストの内容を確定させる。すべてのコマンドを積んでからCloseする
	hr_ = commandList_->Close();
	assert(SUCCEEDED(hr_));

	// GPUにコマンドリストの実行を行わせる
	ID3D12CommandList* commandLists[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandLists);

	// Feneceの値を更新
	fenceValue_++;
	// GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
	commandQueue_->Signal(fence_.Get(), fenceValue_);

	// Fenceの値が指定したSignal値にたどり着いているか確認する
	if (fence_->GetCompletedValue() < fenceValue_) {

		// 指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
		// イベントを待つ
		WaitForSingleObject(fenceEvent_, INFINITE);

		// このあとでGPUの実行を待ってからResetが呼び出される
	}

	// 次のフレーム用のコマンドリストを準備
	hr_ = commandAllocator_->Reset();
	assert(SUCCEEDED(hr_));
	hr_ = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(hr_));
}

/*////////////////////////////////////////////////////////////////////////////////
*								固定FPSの初期化
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::InitFixFPS() {

	// 現在時間を記録する
	reference_ = std::chrono::steady_clock::now();
}

/*////////////////////////////////////////////////////////////////////////////////
*								　FPS固定処理
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::UpdateFixFPS() {

	// フレームレートピッタリの時間
	constexpr std::chrono::microseconds kMinTime(static_cast<uint64_t>(1000000.0f / 60.0f));

	// 1/60秒よりわずかに短い時間
	constexpr std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 64.0f));

	// 現在時間を取得する
	auto now = std::chrono::steady_clock::now();
	// 前回記録からの経過時間を取得する
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60秒 (よりわずかに短い時間) 経っていない場合
	if (elapsed < kMinCheckTime) {
		// 1/60秒経過するまで微小なスリープを繰り返す
		auto wait_until = reference_ + kMinTime;
		while (std::chrono::steady_clock::now() < wait_until) {
			std::this_thread::yield();
		}
	}

	// 現在の時間を記録する
	reference_ = std::chrono::steady_clock::now();
}

/*////////////////////////////////////////////////////////////////////////////////
*								DirectXの初期化
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::Init(WinApp* winApp, uint32_t width, uint32_t height) {

	device_ = std::make_unique<DXDevice>();
	swapChain_ = std::make_unique<DXSwapChain>();
	descriptor_ = std::make_unique<DXDescriptor>();
	rtvManager_ = std::make_unique<RtvManager>();

	kClientWidth_ = width;
	kClientHeight_ = height;

	// FPS固定初期化
	InitFixFPS();

	// デバッグの表示、エラー警告
	DebugLayer();

	// デバイスの初期化
	device_->Init();

	// エラー、警告があれば起動できない
	DebugInfo();

	// FenceとEventの生成
	CreateFenceEvent();

	// dxCompilerの初期化
	InitDXCompiler();

	// commandQueue, commandAllocator, commandListの生成
	CreateCommand();

	// swapChainの生成
	swapChain_->Init(winApp, device_->GetDxgiFactory(), commandQueue_.Get(), kClientWidth_, kClientHeight_);

	// RTV初期化
	rtvManager_->Init();
	// SwapChain用のRTVの作成
	for (uint32_t i = 0; i < swapChain_->bufferCount; i++) {

		// SwapChainは必ず最初に作る、RTVの配列0、1
		rtvManager_->Create(i, swapChain_->GetResources(i));
	}

	// DSVの初期化
	descriptor_->Init(kClientWidth_, kClientHeight_);
}

/*////////////////////////////////////////////////////////////////////////////////
*									終了処理
////////////////////////////////////////////////////////////////////////////////*/
void DXCommon::Close(WinApp* winApp) {

	CloseHandle(fenceEvent_);

	device_.reset();
	swapChain_.reset();
	descriptor_.reset();

	CloseWindow(winApp->GetHwnd());
}

/*////////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
ID3D12Device* DXCommon::GetDevice() const { return device_->GetDevice(); }
ID3D12GraphicsCommandList* DXCommon::GetCommandList() const { return commandList_.Get(); }
IDxcUtils* DXCommon::GetDxcUtils() const { return dxcUtils_.Get(); }
IDxcCompiler3* DXCommon::GetDxcCompiler() const { return dxcCompiler_.Get(); }
IDxcIncludeHandler* DXCommon::GetIncludeHandler() const { return includeHandler_.Get(); }
DXGI_SWAP_CHAIN_DESC1& DXCommon::GetSwapChainDesc() { return swapChain_->GetDesc(); }
