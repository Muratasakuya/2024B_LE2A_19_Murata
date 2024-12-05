#include "NewMoon.h"

#include "Engine/Base/WinApp.h"
#include "Engine/Base/DXCommon.h"
#include "Engine/Managers/ImGuiManager.h"
#include "Engine/Managers/SrvManager.h"

// GameEngine
#include "Engine/Base/NewMoonGame.h"

///===============================================================================
/// staticメンバ変数初期化
#pragma region
uint32_t NewMoon::kWindowWidthd = 1280;
uint32_t NewMoon::kWindowHeightd = 720;
float NewMoon::kWindowWidthf = 1280.0f;
float NewMoon::kWindowHeightf = 720.0f;
Vector2 NewMoon::kWindowSizef = { kWindowWidthf,kWindowHeightf };
std::unique_ptr<WinApp> NewMoon::winApp_ = nullptr;
std::unique_ptr<DXCommon> NewMoon::dxCommon_ = nullptr;
std::unique_ptr<SrvManager> NewMoon::srvManager_ = nullptr;
std::unique_ptr<PipelineManager> NewMoon::pipelineManager_ = nullptr;
std::unique_ptr<ImGuiManager> NewMoon::imguiManager_ = nullptr;
#pragma endregion
///===============================================================================

///===============================================================================
/// MainSystem

void NewMoon::BeginFrame() {

#ifdef _DEBUG
	imguiManager_->Begin();
#endif
	srvManager_->PreDraw();
}

void NewMoon::EndFrame() {

#ifdef _DEBUG
	imguiManager_->End();
#endif
	imguiManager_->Draw();

	//* すべての後処理 *//

	dxCommon_->PostDraw();
}

void NewMoon::BeginPreOffscreen() {

	dxCommon_->BeginPreOffscreen();
}
void NewMoon::EndPostOffscreen() {

	dxCommon_->EndPostOffscreen();
}

void NewMoon::OffscreenDraw(const PipelineType& pipelineType) {

	dxCommon_->OffscreenDraw(pipelineType);

}

void NewMoon::PostDraw() {

#ifdef _DEBUG
	imguiManager_->End();
#endif
	imguiManager_->Draw();

	dxCommon_->PostDraw();
}

bool NewMoon::ProcessMessage() {

	if (winApp_->ProcessMessage()) {

		return 1;
	} else {

		return 0;
	}
}

void NewMoon::Close() {

#ifdef _DEBUG
	imguiManager_->Close();
#endif

	dxCommon_->Close(winApp_.get());
	dxCommon_.reset();
	winApp_.reset();
	srvManager_.reset();
	pipelineManager_.reset();
	imguiManager_.reset();

	// ComFinalize
	CoUninitialize();

}

void NewMoon::Init(uint32_t width, uint32_t height) {

	// ComInitialize
#pragma warning(push)
	// C6031の警告無視
#pragma warning(disable:6031)
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#pragma warning(pop)

	/// ウィンドウサイズの設定
	// int
	kWindowWidthd = width;
	kWindowHeightd = height;
	// float
	kWindowWidthf = static_cast<float>(width);
	kWindowHeightf = static_cast<float>(height);
	kWindowSizef = { kWindowWidthf ,kWindowHeightf };

	winApp_ = std::make_unique<WinApp>();
	winApp_->CreateMainWindow(width, height);

	dxCommon_ = std::make_unique<DXCommon>();
	dxCommon_->Init(winApp_.get(), width, height);

	srvManager_ = std::make_unique<SrvManager>();
	srvManager_->Init();

#ifdef _DEBUG
	imguiManager_ = std::make_unique<ImGuiManager>();
	imguiManager_->Init(winApp_.get(), dxCommon_.get(), srvManager_.get());
#endif

	// Offscreen初期化
	dxCommon_->CreateOffscreenRenderTexture(srvManager_.get(), width, height);

	pipelineManager_ = std::make_unique<PipelineManager>();
	pipelineManager_->CreatePipelineStateObject(dxCommon_.get());

}

///===============================================================================
// Screen

void NewMoon::SetFullScreenMode(bool fullScreen) {
	winApp_->SetFullscreen(fullScreen);
}

bool NewMoon::IsFullScreenMode() {
	return winApp_->IsFullscreen();
}

///===============================================================================
// Command

void NewMoon::SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT rootParamaterIndex, std::string identifier) {

	// ここはどうにかした方がいい
	auto textureManager = NewMoonGame::GetTextureManager();
	textureManager->SetGraphicsRootDescriptorTable(commandList, rootParamaterIndex, identifier);
}

void NewMoon::SetGraphicsPipeline(ID3D12GraphicsCommandList* commandList, PipelineType pipelineType, BlendMode blendMode) {
	pipelineManager_->SetGraphicsPipeline(commandList, pipelineType, blendMode);
}

void NewMoon::SetComputePipeline(ID3D12GraphicsCommandList* commandList, ComputePipelineType csPipelineType) {
	pipelineManager_->SetComputePipeline(commandList, csPipelineType);
}

void NewMoon::ClearDepthBuffer() {
	dxCommon_->ClearDepthBuffer();
}

void NewMoon::TransitionBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter) {
	dxCommon_->TransitionBarrier(resource, stateBefore, stateAfter);
}

///===================================================================
// TestOffscreen

void NewMoon::OffscreenDepthOutlineDraw(OffscreenDepthMaterial& depthMaterial) {

	auto commandList = dxCommon_->GetCommandList();

	pipelineManager_->SetGraphicsPipeline(commandList, OffscreenOutline, kBlendModeNormal);
	depthMaterial.SetCommand(commandList, 1);
	commandList->SetGraphicsRootDescriptorTable(0, dxCommon_->GetRendreTextureGpuHandle());
	commandList->DrawInstanced(3, 1, 0, 0);
}

void NewMoon::OffscreenDissolveDraw(DissolveMaterialBuffer& dissolveMaterial) {

	auto commandList = dxCommon_->GetCommandList();

	pipelineManager_->SetGraphicsPipeline(commandList, OffscreenDissolve, kBlendModeNormal);
	dissolveMaterial.SetCommand(commandList, 2);
	NewMoon::SetGraphicsRootDescriptorTable(commandList, 1, dissolveMaterial.textureName_);
	commandList->SetGraphicsRootDescriptorTable(0, dxCommon_->GetRendreTextureGpuHandle());
	commandList->DrawInstanced(3, 1, 0, 0);
}

///===================================================================
// GetterS

SrvManager* NewMoon::GetSrvManagerPtr() { 
	return srvManager_.get();
}

DXCommon* NewMoon::GetDXCommonPtr() {
	return dxCommon_.get();
}

WinApp* NewMoon::GetWinAppPtr() { 
	return winApp_.get();
}

ID3D12Device* NewMoon::GetDXDevice() { 
	return dxCommon_->GetDevice();
}

ID3D12GraphicsCommandList* NewMoon::GetCommandList() { 
	return dxCommon_->GetCommandList();
}