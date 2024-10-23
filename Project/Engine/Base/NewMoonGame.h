#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/Managers/TextureManager.h"
#include "Engine/Managers/ModelManager.h"
#include "Engine/Methods/Audio.h"
#include "Engine/Methods/Input.h"
#include "Game/Managers/CameraManager.h"
#include "Game/Managers/LightManager.h"
#include "Game/3D/PrimitiveDrawer.h"
#include "Game/3D/Base/BaseGameObject.h"
#include "Game/Editor/RailEditor.h"
#include "Game/Editor/UIEditor.h"

// c++
#include <memory>
#include <vector>
#include <string>
#include <chrono>

// directX
#include <Externals/DirectXTex/DirectXTex.h>

/*////////////////////////////////////////////////////////////////////////////////
*								NewMoonGame Class
////////////////////////////////////////////////////////////////////////////////*/
class NewMoonGame {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	NewMoonGame() = default;
	~NewMoonGame() = default;

	static void Init();
	static void Update();
	static void Close();

	static void Reset();

	///===================================================================
	// Audio

	static void LoadWave(const std::string& fileName);                   // データ読み込み
	static void PlayWave(const std::string fileName, bool loop = false); // 再生
	static void StopWave(const std::string& fileName);                   // 停止
	static void PauseWave(const std::string& fileName);                  // 一時停止
	static void ResumeWave(const std::string& fileName);                 // 一時停止からの再生
	static void SetVolume(const std::string& fileName, float volume);    // 音量の設定
	static bool IsPlayWave(const std::string& fileName);                 // 再生中かどうか

	///===================================================================
	// Input

	static bool PushKey(BYTE keyNumber);                          // キーの押下判定
	static bool TriggerKey(BYTE keyNumber);                       // キーのトリガー判定
	static bool PushGamepadButton(InputGamePadButtons button);    // ゲームパッドのボタンの押下判定
	static bool TriggerGamepadButton(InputGamePadButtons button); // ゲームパッドのボタンのトリガー判定
	static Vector2 GetLeftStickVal();                             // 左スティックの状態を取得
	static Vector2 GetRightStickVal();                            // 右スティックの状態を取得
	static void SetDeadZone(float deadZone);                      // デッドゾーンの設定
	static bool PushMouseLeft();                                  // 左マウスの入力判定
	static bool PushMouseRight();                                 // 右マウスの入力判定
	static Vector2 GetMousePos();                                 // マウスカーソル座標の取得
	static void InputInformation();                               // Input情報表示

	///===================================================================
	// Load

	static void LoadTexture(const std::string& textureName);
	static void LoadModel(const std::string& directoryPath, const std::string& modelName);
	static void LoadAnimation(const std::string& directoryPath, const std::string& animationName, const std::string& modelName);

	///===================================================================
	// Update

	static void SkeletonUpdate(const std::string& animationName);
	static void ApplyAnimation(const std::string& animationName, float animationTime);
	static void SkinClusterUpdate(const std::string& animationName);

	///===================================================================
	// Draw

	static void DrawLine2D(const Vector2& pointA, const Vector2& pointB, const LineColor& color);
	static void DrawLine3D(const Vector3& pointA, const Vector3& pointB, const LineColor& color);
	static void DrawGrid();

	static void Renderer2D();

	///===================================================================
	// Setter

	static void SetToImGui(BaseGameObject* gameObject);

	static void SetToEditor(RailEditor* railEditor);

	///===================================================================
	// Getter

	static TextureManager* GetTextureManager();
	static ModelManager* GetModelMangager();
	static CameraManager* GameCamera();
	static LightManager* GetGameLight();
	static float GetDeltaTime();

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	static std::chrono::steady_clock::time_point lastFrameTime_;
	static float deltaTime_;

	static std::unique_ptr<TextureManager> textureManager_;
	static std::unique_ptr<ModelManager> modelManager_;

	static std::unique_ptr<Audio> audio_;
	static std::unique_ptr<Input> input_;

	static std::unique_ptr<CameraManager> cameraManager_;
	static std::unique_ptr<LightManager> lightManager_;

	static std::unique_ptr<PrimitiveDrawer> lineDrawer2D_;
	static std::unique_ptr<PrimitiveDrawer> lineDrawer3D_;

	static std::vector<BaseGameObject*> gameObjects_;

	static RailEditor* railEditor_;
	static std::unique_ptr<UIEditor> uiEditor_;

	static void ImGui();

};