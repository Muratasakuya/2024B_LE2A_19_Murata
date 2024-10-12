#include "NewMoonGame.h"

#include "NewMoon.h"

///===============================================================================
/// staticメンバ変数初期化
#pragma region
std::unique_ptr<TextureManager> NewMoonGame::textureManager_ = nullptr;
std::unique_ptr<ModelManager> NewMoonGame::modelManager_ = nullptr;
std::unique_ptr<Audio> NewMoonGame::audio_ = nullptr;
std::unique_ptr<Input> NewMoonGame::input_ = nullptr;
std::unique_ptr<CameraManager> NewMoonGame::cameraManager_ = nullptr;
std::unique_ptr<LightManager> NewMoonGame::lightManager_ = nullptr;
std::unique_ptr<PrimitiveDrawer> NewMoonGame::primitiveDrawer_ = nullptr;
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

	primitiveDrawer_ = std::make_unique<PrimitiveDrawer>();
	primitiveDrawer_->Init(cameraManager_->GetViewProjectionBuffer());
}

void NewMoonGame::Update() {

	input_->Update();

	cameraManager_->Update();
	lightManager_->Update();
	primitiveDrawer_->Update();
}

void NewMoonGame::Close() {

	textureManager_.reset();
	modelManager_.reset();
	audio_->Close();
	audio_.reset();
	input_.reset();
	cameraManager_.reset();
	lightManager_.reset();
	primitiveDrawer_.reset();
}

void NewMoonGame::Reset() {

	primitiveDrawer_->Reset();
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

Vector2 NewMoonGame::GetMousePos() {
	return input_->GetMousePos();
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
// Draw

void NewMoonGame::DrawLine(const Vector3& pointA, const Vector3& pointB, const Vector4& color) {
	primitiveDrawer_->DrawLine(pointA, pointB, color);
}

void NewMoonGame::DrawGrid() {
	primitiveDrawer_->DrawGrid();
}

///===================================================================
// Getter

TextureManager* NewMoonGame::GetTextureManager() {
	return textureManager_.get();
}

ModelManager* NewMoonGame::GetModelMangager() {
	return modelManager_.get();
}

CameraManager* NewMoonGame::GetGameCamera() {
	return cameraManager_.get();
}

LightManager* NewMoonGame::GetGameLight() {
	return lightManager_.get();
}
