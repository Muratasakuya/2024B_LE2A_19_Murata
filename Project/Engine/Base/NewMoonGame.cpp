#include "NewMoonGame.h"

#include "NewMoon.h"
#include "Engine/Managers/SrvManager.h"
#include "Engine/Managers/ImGuiManager.h"
#include "DXCommon.h"

///===============================================================================
/// staticメンバ変数初期化
#pragma region
std::chrono::steady_clock::time_point NewMoonGame::lastFrameTime_ = std::chrono::steady_clock::now();
float NewMoonGame::deltaTime_ = 0.0f;
std::unique_ptr<TextureManager> NewMoonGame::textureManager_ = nullptr;
std::unique_ptr<ModelManager> NewMoonGame::modelManager_ = nullptr;
std::unique_ptr<Audio> NewMoonGame::audio_ = nullptr;
std::unique_ptr<Input> NewMoonGame::input_ = nullptr;
std::unique_ptr<CameraManager> NewMoonGame::cameraManager_ = nullptr;
std::unique_ptr<LightManager> NewMoonGame::lightManager_ = nullptr;
std::unique_ptr<PrimitiveDrawer> NewMoonGame::lineDrawer2D_ = nullptr;
std::unique_ptr<PrimitiveDrawer> NewMoonGame::lineDrawer3D_ = nullptr;
std::vector<BaseGameObject*> NewMoonGame::gameObjects_ = {};
BaseGameObject* NewMoonGame::selectedGameObject_ = nullptr;
int NewMoonGame::currentObjectIndex_ = 0;
std::vector<IBaseParticle*> NewMoonGame::particles_ = {};
std::unique_ptr<CollisionManager> NewMoonGame::collisionManager_ = nullptr;
Vector2 NewMoonGame::mainWindowPos_ = Vector2(210.0f, 64.0f);
#pragma endregion
///===============================================================================

/*////////////////////////////////////////////////////////////////////////////////
*						NewMoonGame classMethods
////////////////////////////////////////////////////////////////////////////////*/

void NewMoonGame::Init() {

	textureManager_ = std::make_unique<TextureManager>();
	textureManager_->Init();

	modelManager_ = std::make_unique<ModelManager>();
	modelManager_->Init();

	audio_ = std::make_unique<Audio>();
	audio_->Init();

	input_ = std::make_unique<Input>();
	input_->Init();

	cameraManager_ = std::make_unique<CameraManager>();
	cameraManager_->Init();

	lightManager_ = std::make_unique<LightManager>();
	lightManager_->Init();

	lineDrawer2D_ = std::make_unique<PrimitiveDrawer>();
	lineDrawer2D_->Init(cameraManager_->GetCamera2D()->GetViewProBuffer());

	lineDrawer3D_ = std::make_unique<PrimitiveDrawer>();
	lineDrawer3D_->Init(cameraManager_->GetCamera3D()->GetViewProBuffer());

	collisionManager_ = std::make_unique<CollisionManager>();

}

void NewMoonGame::ImGui() {
#ifdef _DEBUG

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(NewMoon::kWindowWidthf, NewMoon::kWindowHeightf));
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
	ImGui::Begin("Engine", nullptr, windowFlags);

	//* MainWindowSetting *//
	const ImVec2 imageSize(768.0f, 432.0f);              //* サイズ
	ImVec2 imagePos(mainWindowPos_.x, mainWindowPos_.y); //* 座標
	ImGui::SetCursorPos(imagePos);
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = NewMoon::GetDXCommonPtr()->GetRendreTextureGpuHandle();
	ImGui::Image(ImTextureID(gpuHandle.ptr), imageSize);

	//* GameObjects List (Left Side) *//
	ImGui::SetCursorPos(ImVec2(8.0f, imagePos.y - 32.0f));
	ImGui::Text("Game Objects");
	ImGui::BeginChild("GameObjectsChild", ImVec2(196.0f, imageSize.y + 12.0f), true, ImGuiWindowFlags_AlwaysUseWindowPadding);

	for (const auto& gameObject : gameObjects_) {
		if (gameObject) {
			if (ImGui::Selectable(gameObject->GetName().c_str(), selectedGameObject_ == gameObject)) {

				selectedGameObject_ = gameObject;
			}
		}
	}

	ImGui::EndChild();

	//* EngineLog *//
	ImGui::BeginChild("EngineChild",
		ImVec2(((NewMoon::kWindowWidthf / 2.0f) + (imageSize.x / 2.0f) - 254.0f), 212.0f),
		true, ImGuiWindowFlags_AlwaysUseWindowPadding);

	ImGui::Text("Engine");
	ImGui::Separator();
	ImGui::Text("Frame Rate: %.1f fps", ImGui::GetIO().Framerate); // フレームレート情報
	ImGui::Text("Delta Time: %.3f s", deltaTime_);                 // ΔTime
	ImGui::Separator();
	ImGui::Text("Descriptor");
	NewMoon::GetSrvManagerPtr()->ImGui();

	ImGui::EndChild();

	//* Selected GameObject Details (Right Side) *//
	if (selectedGameObject_) {
		ImGui::SetCursorPos(ImVec2(imagePos.x + imageSize.x + 6.0f, imagePos.y - 32.0f));
		ImGui::BeginChild("SelectedGameObjectChild", ImVec2(288.0f, imageSize.y + 32.0f), true, ImGuiWindowFlags_AlwaysUseWindowPadding);

		ImGui::Text(selectedGameObject_->GetName().c_str());
		ImGui::Separator();

		selectedGameObject_->ImGui();

		ImGui::EndChild();
	} else {

		ImGui::SetCursorPos(ImVec2(imagePos.x + imageSize.x + 6.0f, imagePos.y - 32.0f));
		ImGui::BeginChild("NoSelectedGameObjectChild", ImVec2(288.0f, imageSize.y + 32.0f), true, ImGuiWindowFlags_AlwaysUseWindowPadding);

		ImGui::Text("No Selected");
		ImGui::Separator();

		ImGui::EndChild();
	}

	//* Collision Logs (Right Bottom Side) *//
	ImGui::SetCursorPos(ImVec2(imagePos.x + imageSize.x - 192.0f, imagePos.y + imageSize.y + 4.0f));
	ImGui::BeginChild("CollisionLogsChild", ImVec2(486.0f, 212.0f), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	collisionManager_->DisplayCollisionLogs();

	ImGui::EndChild();

	ImGui::End();

#endif // _DEBUG
}

void NewMoonGame::Update() {

	ImGui();

	auto currentFrameTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsedTime = currentFrameTime - lastFrameTime_;
	deltaTime_ = elapsedTime.count();

	lastFrameTime_ = currentFrameTime;

	input_->Update();

	cameraManager_->Update();
	lightManager_->Update();
	lineDrawer2D_->Update();
	lineDrawer3D_->Update();

	collisionManager_->UpdateAllCollisions();

}

void NewMoonGame::Close() {

	textureManager_.reset();
	modelManager_.reset();
	audio_->Close();
	audio_.reset();
	input_.reset();
	cameraManager_.reset();
	lightManager_.reset();
	lineDrawer2D_.reset();
	lineDrawer3D_.reset();
}

void NewMoonGame::Reset() {

	lineDrawer2D_->Reset();
	lineDrawer3D_->Reset();
}

///===================================================================
// Audio

void NewMoonGame::LoadWave(const std::string& fileName) {
	audio_->LoadWave(fileName);
}

void NewMoonGame::PlayWave(const std::string fileName, bool loop) {
	audio_->PlayWave(fileName, loop);
}

void NewMoonGame::StopWave(const std::string& fileName) {
	audio_->StopWave(fileName);
}

void NewMoonGame::PauseWave(const std::string& fileName) {
	audio_->PauseWave(fileName);
}

void NewMoonGame::ResumeWave(const std::string& fileName) {
	audio_->ResumeWave(fileName);
}

void NewMoonGame::SetVolume(const std::string& fileName, float volume) {
	audio_->SetVolume(fileName, volume);
}

bool NewMoonGame::IsPlayWave(const std::string& fileName) {
	return audio_->IsPlayWave(fileName);
}

///===================================================================
// Input

bool NewMoonGame::PushKey(BYTE keyNumber) {
	return input_->PushKey(keyNumber);
}

bool NewMoonGame::TriggerKey(BYTE keyNumber) {
	return input_->TriggerKey(keyNumber);
}

bool NewMoonGame::PushGamepadButton(InputGamePadButtons button) {
	return input_->PushGamepadButton(button);
}

bool NewMoonGame::TriggerGamepadButton(InputGamePadButtons button) {
	return input_->TriggerGamepadButton(button);
}

Vector2 NewMoonGame::GetLeftStickVal() {
	return input_->GetLeftStickVal();
}

Vector2 NewMoonGame::GetRightStickVal() {
	return input_->GetRightStickVal();
}

void NewMoonGame::SetDeadZone(float deadZone) {
	input_->SetDeadZone(deadZone);
}

bool NewMoonGame::PushMouseLeft() {
	return input_->PushMouseLeft();
}

bool NewMoonGame::PushMouseRight() {
	return input_->PushMouseRight();
}

bool NewMoonGame::PushMouseCenter() {
	return input_->PushMouseCenter();
}

Vector2 NewMoonGame::GetMousePos() {
	return input_->GetMousePos();
}

Vector2 NewMoonGame::GetMousePrePos() {
	return input_->GetMousePrePos();
}

float NewMoonGame::GetMouseWheel() {
	return input_->GetMouseWheel();
}

Vector2 NewMoonGame::GetMouseMoveValue() {
	return input_->GetMouseMoveValue();
}

void NewMoonGame::InputInformation() {
	input_->ImGui();
}

///===================================================================
// Load

void NewMoonGame::LoadTexture(const std::string& textureName) {
	textureManager_->LoadTexture(textureName);
}

void NewMoonGame::LoadModel(const std::string& directoryPath, const std::string& modelName) {
	modelManager_->LoadModel(directoryPath, modelName);
}

void NewMoonGame::LoadAnimation(const std::string& directoryPath, const std::string& animationName, const std::string& modelName) {
	modelManager_->LoadAniamation(directoryPath, animationName, modelName);
}

///===================================================================
// Update

void NewMoonGame::SkeletonUpdate(const std::string& animationName) {
	modelManager_->SkeletonUpdate(animationName);
}

void NewMoonGame::ApplyAnimation(const std::string& animationName, float animationTime) {
	modelManager_->ApplyAnimation(animationName, animationTime);
}

void NewMoonGame::SkinClusterUpdate(const std::string& animationName) {
	modelManager_->SkinClusterUpdate(animationName);
}

///===================================================================
// Collision

void NewMoonGame::AddCollider(Collider* collider) {
	collisionManager_->AddCollider(collider);
}

void NewMoonGame::RemoveCollider(Collider* collider) {
	collisionManager_->RemoveCollider(collider);
}

void NewMoonGame::ClearAllColliders() {
	collisionManager_->ClearAllColliders();
}

///===================================================================
// Draw

void NewMoonGame::DrawLine2D(const Vector2& pointA, const Vector2& pointB, const LineColor& color) {
	lineDrawer2D_->DrawLine2D(pointA, pointB, color);
}

void NewMoonGame::DrawLine3D(const Vector3& pointA, const Vector3& pointB, const LineColor& color) {
	lineDrawer3D_->DrawLine3D(pointA, pointB, color);
}

void NewMoonGame::DrawGrid() {
	lineDrawer3D_->DrawGrid();
}

void NewMoonGame::DrawSphere(float DrawSphere, const Vector3& centerPos, const LineColor& color) {
	lineDrawer3D_->DrawSphere(DrawSphere, centerPos, color);
}

void NewMoonGame::DrawOBB(
	const Vector3& size, const Quaternion& rotate, const Vector3& centerPos, const LineColor& color) {
	lineDrawer3D_->DrawOBB(size, rotate, centerPos, color);
}

///===================================================================
// Setter

void NewMoonGame::SetToImGui(BaseGameObject* gameObject) {
	gameObjects_.push_back(gameObject);
}

void NewMoonGame::EraseToImGui(BaseGameObject* gameObject) {
	gameObjects_.erase(std::remove(gameObjects_.begin(), gameObjects_.end(), gameObject), gameObjects_.end());
}

void NewMoonGame::SetToImGui(IBaseParticle* particle) {
	particles_.push_back(particle);
}

//* command
void NewMoonGame::SetEnvironmentCommand(ID3D12GraphicsCommandList* commandList, PipelineType pipeline) {

	// light
	lightManager_->GetLightBuffer().SetCommand(
		commandList, lightManager_->GetLightBuffer().GetRootParameterIndex(pipeline));
	// camera
	cameraManager_->GetCamera3D()->SetCommand(commandList, pipeline);
}

///===================================================================
// Getter

TextureManager* NewMoonGame::GetTextureManager() {
	return textureManager_.get();
}

ModelManager* NewMoonGame::GetModelManager() {
	return modelManager_.get();
}

CameraManager* NewMoonGame::GameCamera() {
	return cameraManager_.get();
}

LightManager* NewMoonGame::GetGameLight() {
	return lightManager_.get();
}

float NewMoonGame::GetDeltaTime() {
	return deltaTime_;
}