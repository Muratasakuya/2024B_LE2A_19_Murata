#include "NewMoon.h"

#include "WinApp.h"
#include "DXCommon.h"
#include "SrvManager.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "Audio.h"

///===============================================================================
/// staticメンバ変数初期化
#pragma region
uint32_t NewMoon::kWindowWidthd = 1280;
uint32_t NewMoon::kWindowHeightd = 720;
float NewMoon::kWindowWidthf = 1280.0f;
float NewMoon::kWindowHeightf = 720.0f;
std::unique_ptr<WinApp> NewMoon::winApp_ = nullptr;
std::unique_ptr<DXCommon> NewMoon::dxCommon_ = nullptr;
std::unique_ptr<SrvManager> NewMoon::srvManager_ = nullptr;
std::unique_ptr<PipelineManager> NewMoon::pipelineManager_ = nullptr;
std::unique_ptr<ImGuiManager> NewMoon::imguiManager_ = nullptr;
std::unique_ptr<TextureManager> NewMoon::textureManager_ = nullptr;
std::unique_ptr<ModelManager> NewMoon::modelManager_ = nullptr;
std::unique_ptr<Input> NewMoon::input_ = nullptr;
std::unique_ptr<Audio> NewMoon::audio_ = nullptr;
#pragma endregion
///===============================================================================

///===============================================================================
/// メインシステム

/*////////////////////////////////////////////////////////////////////////////////
*								 フレーム開始処理
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::BeginFrame() {

	input_->Update();
#ifdef _DEBUG
	imguiManager_->Begin();
#endif
	srvManager_->PreDraw();
}

/*////////////////////////////////////////////////////////////////////////////////
*								 フレーム終了処理
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::EndFrame() {}

/*////////////////////////////////////////////////////////////////////////////////
*								オフスクリーン描画処理
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::OffscreenPreDraw() {

	dxCommon_->OffscreenPreDraw();
}
void NewMoon::OffscreenPostDraw() {

	dxCommon_->OffscreenPostDraw();
}

/*////////////////////////////////////////////////////////////////////////////////
*									通常描画処理
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::PreDraw() {

	dxCommon_->PreDraw();
}

void NewMoon::PostDraw() {

#ifdef _DEBUG
	imguiManager_->End();
#endif
	imguiManager_->Draw();

	dxCommon_->PostDraw();
}

/*////////////////////////////////////////////////////////////////////////////////
*							  メッセージの受け渡し処理
////////////////////////////////////////////////////////////////////////////////*/
bool NewMoon::ProcessMessage() {

	if (winApp_->ProcessMessage()) {

		return 1;
	} else {

		return 0;
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*									終了処理
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::Finalize() {

#ifdef _DEBUG
	imguiManager_->Finalize();
#endif

	// 手動で解放
	dxCommon_->Finalize(winApp_.get());
	dxCommon_.reset();
	winApp_.reset();

	srvManager_.reset();
	pipelineManager_.reset();

	textureManager_.reset();
	modelManager_.reset();

	imguiManager_.reset();
	input_.reset();
	audio_->Finalize();
	audio_.reset();

	// ComFinalize
	CoUninitialize();

}

/*////////////////////////////////////////////////////////////////////////////////
*								メインシステムの初期化
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::Initialize(uint32_t width, uint32_t height) {

	// ComInitialize
#pragma warning(push)
	// C6031の警告無視
#pragma warning(disable:6031)
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#pragma warning(pop)

	/*-----------------------------------------------------------------------*/
	/// ウィンドウサイズの設定
	// int
	kWindowWidthd = width;
	kWindowHeightd = height;
	// float
	kWindowWidthf = static_cast<float>(width);
	kWindowHeightf = static_cast<float>(height);

	/*-----------------------------------------------------------------------*/
	/// WinApp

	// ウィンドウ生成
	winApp_ = std::make_unique<WinApp>();
	winApp_->CreateMainWindow(width, height);
	/*-----------------------------------------------------------------------*/
	/// DXCommon SrvManager

	// DirectXの初期化
	dxCommon_ = std::make_unique<DXCommon>();
	dxCommon_->Initialize(winApp_.get(), width, height);
	// srvの初期化
	srvManager_ = std::make_unique<SrvManager>();
	srvManager_->Initialize();

	// Offscreen初期化
	dxCommon_->CreateOffscreenRenderTexture(srvManager_.get(), width, height);

	/*-----------------------------------------------------------------------*/
	/// TextureManager

	// インスタンスのセット
	textureManager_ = std::make_unique<TextureManager>();
	textureManager_->Initialize(dxCommon_.get(), srvManager_.get());
	/*-----------------------------------------------------------------------*/
	/// ImGuiManager

#ifdef _DEBUG
	// ImGuiの初期化
	imguiManager_ = std::make_unique<ImGuiManager>();
	imguiManager_->Initialize(winApp_.get(), dxCommon_.get(), srvManager_.get());
#endif
	/*-----------------------------------------------------------------------*/
	/// PipelineManager

	// パイプラインの各種初期化
	pipelineManager_ = std::make_unique<PipelineManager>();
	pipelineManager_->CreatePipelineStateObject(dxCommon_.get());
	/*-----------------------------------------------------------------------*/
	/// Input

	// インプットの初期化
	input_ = std::make_unique<Input>();
	input_->Initialize(winApp_.get());
	/*-----------------------------------------------------------------------*/
	/// Input

	// オーディオ初期化
	audio_ = std::make_unique<Audio>();
	audio_->Initialize();
	/*-----------------------------------------------------------------------*/
	/// ModelManager

	// モデル初期化
	modelManager_ = std::make_unique<ModelManager>();
	modelManager_->Initialize(srvManager_.get());
}

///===============================================================================
/// ライブラリ関数

/*////////////////////////////////////////////////////////////////////////////////
*									 テクスチャ読み込み
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::LoadTexture(const std::string& textureName) {

	textureManager_->LoadTexture(textureName);
}

/*////////////////////////////////////////////////////////////////////////////////
*									モデル読み込み
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::LoadModel(const std::string& directoryPath, const std::string& modelName) {

	modelManager_->LoadModel(directoryPath, modelName);
}

/*////////////////////////////////////////////////////////////////////////////////
*									アニメーション読み込み
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::LoadAnimation(const std::string& directoryPath, const std::string& animationName, const std::string& modelName) {

	modelManager_->LoadAniamation(directoryPath, animationName, modelName);
}

/*////////////////////////////////////////////////////////////////////////////////
*								スケルトンの更新、適応
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::SkeletonUpdate(const std::string& animationName) {

	modelManager_->SkeletonUpdate(animationName);
}
void NewMoon::ApplyAnimation(const std::string& animationName, float animationTime) {

	modelManager_->ApplyAnimation(animationName, animationTime);
}

/*////////////////////////////////////////////////////////////////////////////////
*								スキンクラスターの更新
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::SkinClusterUpdate(const std::string& animationName) {

	modelManager_->SkinClusterUpdate(animationName);
}

/*////////////////////////////////////////////////////////////////////////////////
*								  サウンドデータ読み込み
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::LoadWave(const std::string filename) {

	audio_->LoadWave(filename);
}

/*////////////////////////////////////////////////////////////////////////////////
*									サウンド再生
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::PlayWave(const std::string& name, bool loop) {

	audio_->PlayWave(name, loop);
}

/*////////////////////////////////////////////////////////////////////////////////
*									サウンド停止
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::StopWave(const std::string& name) {

	audio_->StopWave(name);
}

/*////////////////////////////////////////////////////////////////////////////////
*									サウンド一時停止
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::PauseWave(const std::string& name) {

	audio_->PauseWave(name);
}

/*////////////////////////////////////////////////////////////////////////////////
*								サウンド一時停止からの再生
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::ResumeWave(const std::string& name) {

	audio_->ResumeWave(name);
}

/*////////////////////////////////////////////////////////////////////////////////
*									サウンド音量の設定
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::SetVolume(const std::string& name, float volume) {

	audio_->SetVolume(name, volume);
}

/*////////////////////////////////////////////////////////////////////////////////
*									サウンド再生中かどうか
////////////////////////////////////////////////////////////////////////////////*/
bool NewMoon::IsPlayWave(const std::string& name) {

	return audio_->IsPlayWave(name);
}

/*////////////////////////////////////////////////////////////////////////////////
*									キーの押下判定
////////////////////////////////////////////////////////////////////////////////*/
bool NewMoon::PushKey(BYTE keyNumber) {

	return input_->PushKey(keyNumber);
}

/*////////////////////////////////////////////////////////////////////////////////
*								   キーのトリガー判定
////////////////////////////////////////////////////////////////////////////////*/
bool NewMoon::TriggerKey(BYTE keyNumber) {

	return input_->TriggerKey(keyNumber);
}

/*////////////////////////////////////////////////////////////////////////////////
*							ゲームパッドのボタンの押下判定
////////////////////////////////////////////////////////////////////////////////*/
bool NewMoon::PushGamepadButton(InputGamePadButtons button) {

	return input_->PushGamepadButton(button);
}

/*////////////////////////////////////////////////////////////////////////////////
*							ゲームパッドのボタンのトリガー判定
////////////////////////////////////////////////////////////////////////////////*/
bool NewMoon::TriggerGamepadButton(InputGamePadButtons button) {

	return input_->TriggerGamepadButton(button);
}

/*////////////////////////////////////////////////////////////////////////////////
*								左スティックの値の取得
////////////////////////////////////////////////////////////////////////////////*/
Vector2 NewMoon::GetLeftStickVal() {

	return input_->GetLeftStickVal();
}

/*////////////////////////////////////////////////////////////////////////////////
*								右スティックの値の取得
////////////////////////////////////////////////////////////////////////////////*/
Vector2 NewMoon::GetRightStickVal() {

	return input_->GetRightStickVal();
}

/*////////////////////////////////////////////////////////////////////////////////
*								   デッドゾーンの設定
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::SetDeadZone(float deadZone) {

	input_->SetDeadZone(deadZone);
}

/*////////////////////////////////////////////////////////////////////////////////
*								マウスの入力判定 左
////////////////////////////////////////////////////////////////////////////////*/
bool NewMoon::PushMouseLeft() {

	return input_->PushMouseLeft();
}

/*////////////////////////////////////////////////////////////////////////////////
*								マウスの入力判定 右
////////////////////////////////////////////////////////////////////////////////*/
bool NewMoon::PushMouseRight() {

	return input_->PushMouseRight();
}

/*////////////////////////////////////////////////////////////////////////////////
*								マウスカーソル座標の取得
////////////////////////////////////////////////////////////////////////////////*/
Vector2 NewMoon::GetMousePos() {

	return input_->GetMousePos();
}

/*////////////////////////////////////////////////////////////////////////////////
*								  InputImGui情報表示
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::InputImGui() {

	input_->ImGui();
}

/*////////////////////////////////////////////////////////////////////////////////
*								   フルスクリーン設定
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::SetFullScreenMode(bool fullScreen) {

	winApp_->SetFullscreen(fullScreen);
}
/*////////////////////////////////////////////////////////////////////////////////
*								  フルスクリーンかどうか
////////////////////////////////////////////////////////////////////////////////*/
bool NewMoon::IsFullScreenMode() { return winApp_->IsFullscreen(); }

/*////////////////////////////////////////////////////////////////////////////////
*									Command処理
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT rootParamaterIndex, std::string identifier) {

	textureManager_->SetGraphicsRootDescriptorTable(commandList, rootParamaterIndex, identifier);
}
void NewMoon::SetGraphicsPipeline(ID3D12GraphicsCommandList* commandList, PipelineType pipelineType, BlendMode blendMode) {

	pipelineManager_->SetGraphicsPipeline(commandList, pipelineType, blendMode);
}
void NewMoon::SetComputePipeline(ID3D12GraphicsCommandList* commandList, ComputePipelineType csPipelineType) {

	pipelineManager_->SetComputePipeline(commandList, csPipelineType);
}
void NewMoon::ClearDepthBuffer() { dxCommon_->ClearDepthBuffer(); }

void NewMoon::TransitionBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter) {

	dxCommon_->TransitionBarrier(resource, stateBefore, stateAfter);
}

/*////////////////////////////////////////////////////////////////////////////////
*								オフスクリーン描画処理
////////////////////////////////////////////////////////////////////////////////*/
void NewMoon::OffscreenDraw() {

	auto commandList = dxCommon_->GetCommandList();

	pipelineManager_->SetGraphicsPipeline(commandList, OffscreenCopy, kBlendModeNormal);
	commandList->SetGraphicsRootDescriptorTable(0, dxCommon_->GetRendreTextureGpuHandle());
	commandList->DrawInstanced(3, 1, 0, 0);
}
void NewMoon::OffscreenDepthOutlineDraw(OffscreenDepthMaterial& depthMaterial) {

	auto commandList = dxCommon_->GetCommandList();

	pipelineManager_->SetGraphicsPipeline(commandList, OffscreenOutline, kBlendModeNormal);
	depthMaterial.SetCommand(commandList, 1);
	commandList->SetGraphicsRootDescriptorTable(0, dxCommon_->GetRendreTextureGpuHandle());
	commandList->DrawInstanced(3, 1, 0, 0);
}
void NewMoon::OffscreenDissolveDraw(OffscreenDissolveMaterial& dissolveMaterial) {

	auto commandList = dxCommon_->GetCommandList();

	pipelineManager_->SetGraphicsPipeline(commandList, OffscreenDissolve, kBlendModeNormal);
	dissolveMaterial.SetCommand(commandList, 2);
	NewMoon::SetGraphicsRootDescriptorTable(commandList, 1, dissolveMaterial.textureName_);
	commandList->SetGraphicsRootDescriptorTable(0, dxCommon_->GetRendreTextureGpuHandle());
	commandList->DrawInstanced(3, 1, 0, 0);
}

/*///////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
ID3D12Device* NewMoon::GetDXDevice() { return dxCommon_->GetDevice(); }
ID3D12GraphicsCommandList* NewMoon::GetCommandList() { return dxCommon_->GetCommandList(); }
const DirectX::TexMetadata& NewMoon::GetMetaData(const std::string& textureName) { return textureManager_->GetMetaData(textureName); }
ModelData NewMoon::GetModelData(const std::string& modelName) { return modelManager_->GetModelData(modelName); }
AnimationData NewMoon::GetAnimationData(const std::string& animationName) { return modelManager_->GetAnimationData(animationName); }
Skeleton NewMoon::GetSkeletonData(const std::string& animationName) { return modelManager_->GetSkeletonData(animationName); }
SkinCluster NewMoon::GetSkinClusterData(const std::string& animationName) { return modelManager_->GetSkinClusterData(animationName); }
SrvManager* NewMoon::GetSrvManagerPtr() { return srvManager_.get(); }