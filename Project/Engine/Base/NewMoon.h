#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/Managers/PipelineManager.h"
#include "Engine/Methods/Input.h"
#include "Game/Components/MaterialObject.h"
#include "Game/Managers/CameraManager.h"
#include "Lib/Structure.h"

// directX
#include <Externals/DirectXTex/DirectXTex.h>

// c++
#include <cstdint>
#include <memory>
#include <array>
#include <iostream>
#include <tuple>
#include <utility>

///===============================================================================
/// クラス前方宣言
#pragma region
class WinApp;
class DXCommon;
class SrvManager;
class ImGuiManager;
class TextureManager;
class ModelManager;
class ParticleManager;
class ParticleEmitter;
class Audio;
#pragma endregion
///===============================================================================

/// GameとDirectXなど、役割ごとにEngineを分ける、これは絶対

/*////////////////////////////////////////////////////////////////////////////////
*									NewMoon Class
////////////////////////////////////////////////////////////////////////////////*/
class NewMoon {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	NewMoon() = default;
	~NewMoon() = default;

	///===============================================================================
	// メインシステム

	// 各システムの初期化
	static void Initialize(uint32_t width, uint32_t height);

	// フレーム開始処理
	static void BeginFrame();

	// フレーム終了処理
	static void EndFrame();

	// 描画設定
	static void OffscreenPreDraw();
	static void OffscreenPostDraw();

	static void PreDraw();
	static void PostDraw();

	// システムの終了
	static void Finalize();

	// メッセージの受け渡し処理
	static bool ProcessMessage();

	///===============================================================================
	// オフスクリーン描画処理

	static void OffscreenDraw();
	static void OffscreenDepthOutlineDraw(OffscreenDepthMaterial& depthMaterial);
	static void OffscreenDissolveDraw(OffscreenDissolveMaterial& dissolveMaterial);

	///===============================================================================
	// Sound

	// サウンドデータ読み込み
	static void LoadWave(const std::string filename);
	// サウンド再生
	static void PlayWave(const std::string& name, bool loop = false);
	// サウンド停止
	static void StopWave(const std::string& name);
	// サウンド一時停止
	static void PauseWave(const std::string& name);
	// サウンド一時停止からの再生
	static void ResumeWave(const std::string& name);
	// サウンド音量の設定
	static void SetVolume(const std::string& name, float volume);
	// サウンド再生中かどうか
	static bool IsPlayWave(const std::string& name);

	///===============================================================================
	// Input

	// キーの押下判定
	static bool PushKey(BYTE keyNumber);
	// キーのトリガー判定
	static bool TriggerKey(BYTE keyNumber);

	// ゲームパッドのボタンの押下判定
	static bool PushGamepadButton(InputGamePadButtons button);
	// ゲームパッドのボタンのトリガー判定
	static bool TriggerGamepadButton(InputGamePadButtons button);
	// Vector2型 左スティックと右スティックの状態を取得
	static Vector2 GetLeftStickVal();
	static Vector2 GetRightStickVal();
	// デッドゾーンの設定
	static void SetDeadZone(float deadZone);

	// マウスの入力判定
	static bool PushMouseLeft();
	static bool PushMouseRight();
	// マウスカーソル座標の取得
	static Vector2 GetMousePos();

	// Input情報表示
	static void InputImGui();

	///===============================================================================
	// Screen

	// フルスクリーン設定
	static void SetFullScreenMode(bool fullScreen);
	static bool IsFullScreenMode();

	///===============================================================================
	// Load

	// テクスチャ読み込み
	static void LoadTexture(const std::string& textureName);

	// モデル読み込み
	static void LoadModel(const std::string& directoryPath, const std::string& modelName);

	// アニメーション読み込み
	static void LoadAnimation(const std::string& directoryPath, const std::string& animationName, const std::string& modelName);

	///===============================================================================
	// 更新処理

	static void SkeletonUpdate(const std::string& animationName);
	static void ApplyAnimation(const std::string& animationName, float animationTime);
	
	static void SkinClusterUpdate(const std::string& animationName);

	///===============================================================================
	// Command

	static void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT rootParamaterIndex, std::string identifier);
	static void SetGraphicsPipeline(ID3D12GraphicsCommandList* commandList, PipelineType pipelineType, BlendMode blendMode);
	static void SetComputePipeline(ID3D12GraphicsCommandList* commandList, ComputePipelineType csPipelineType);
	static void ClearDepthBuffer();
	static void TransitionBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);

	///===============================================================================
	// Getter

	static ID3D12Device* GetDXDevice();
	static ID3D12GraphicsCommandList* GetCommandList();
	static const DirectX::TexMetadata& GetMetaData(const std::string& textureName);
	static ModelData GetModelData(const std::string& modelName);
	static AnimationData GetAnimationData(const std::string& animationName);
	static Skeleton GetSkeletonData(const std::string& animationName);
	static SkinCluster GetSkinClusterData(const std::string& animationName);
	static SrvManager* GetSrvManagerPtr();
	static Matrix4x4 GetViewProjection(const CameraType& cameraType);

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	// ウィンドウサイズ
	// int
	static uint32_t kWindowWidthd;
	static uint32_t kWindowHeightd;
	// float
	static float kWindowWidthf;
	static float kWindowHeightf;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	static std::unique_ptr<WinApp> winApp_;
	static std::unique_ptr<DXCommon> dxCommon_;

	static std::unique_ptr<SrvManager> srvManager_;
	static std::unique_ptr<PipelineManager> pipelineManager_;

	static std::unique_ptr<TextureManager> textureManager_;
	static std::unique_ptr<ModelManager> modelManager_;

	static std::unique_ptr<ImGuiManager> imguiManager_;
	static std::unique_ptr<Input> input_;
	static std::unique_ptr<Audio> audio_;

	static std::unique_ptr<CameraManager> cameraManager_;
};