#include "NewMoonGame.h"

#include "NewMoon.h"
#include "Engine/Managers/ImGuiManager.h"

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
RailEditor* NewMoonGame::railEditor_ = nullptr;
std::unique_ptr<UIEditor> NewMoonGame::uiEditor_ = nullptr;
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

	uiEditor_ = std::make_unique<UIEditor>();
	uiEditor_->Init();

}

void NewMoonGame::ImGui() {
#ifdef _DEBUG
	ImGui::Begin("Game Debug");

	// Performance情報は常に表示する
	ImGui::Text("Frame Rate: %.1f fps", ImGui::GetIO().Framerate); // フレームレート情報
	ImGui::Text("Delta Time: %.3f s", deltaTime_);                 // ΔTime

	if (ImGui::BeginTabBar("Tabs")) {

		if (ImGui::BeginTabItem("Camera")) {
			cameraManager_->ImGui();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Input")) {
			input_->ImGui();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("GameObject")) {
			for (auto& gameObject : gameObjects_) {
				if (ImGui::CollapsingHeader(gameObject->GetName().c_str())) {

					gameObject->ImGui();
				}
			}
			ImGui::EndTabItem();
		}
		// Editors
		if (ImGui::BeginTabItem("Editor")) {

			// Rail Editor
			if (railEditor_) {
				if (ImGui::CollapsingHeader("Rail Editor")) {
					railEditor_->ImGui();
				}
			}

			// UI Editor
			if (ImGui::CollapsingHeader("UI Editor")) {
				uiEditor_->ImGui();
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

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

	uiEditor_->Update();
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
	uiEditor_.reset();
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

float NewMoonGame::GetMouseWheel() {
	return input_->GetMouseWheel();
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

void NewMoonGame::DrawLine2D(const Vector2& pointA, const Vector2& pointB, const LineColor& color) {
	lineDrawer2D_->DrawLine2D(pointA, pointB, color);
}

void NewMoonGame::DrawLine3D(const Vector3& pointA, const Vector3& pointB, const LineColor& color) {
	lineDrawer3D_->DrawLine3D(pointA, pointB, color);
}

void NewMoonGame::DrawGrid() {
	lineDrawer3D_->DrawGrid();
}

void NewMoonGame::Renderer2D() {
	uiEditor_->Draw();
}

///===================================================================
// Setter

void NewMoonGame::SetToImGui(BaseGameObject* gameObject) {
	gameObjects_.push_back(gameObject);
}

void NewMoonGame::SetToEditor(RailEditor* railEditor) {
	railEditor_ = railEditor;
}

///===================================================================
// Getter

TextureManager* NewMoonGame::GetTextureManager() {
	return textureManager_.get();
}

ModelManager* NewMoonGame::GetModelMangager() {
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