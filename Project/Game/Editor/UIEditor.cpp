#include "UIEditor.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"
#include "Engine/Managers/TextureManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							Sprite classMethods
////////////////////////////////////////////////////////////////////////////////*/

void UIEditor::Init() {

	NewMoonGame::LoadTexture("uvChecker");
	NewMoonGame::LoadTexture("monsterBall");
	NewMoonGame::LoadTexture("checkerBoard");
	NewMoonGame::LoadTexture("AnimatedCube_BaseColor");

	previewTextureSize_ = Vector2(128.0f, 128.0f);

	currentState_ = UIState::TexturePreviewList; // 初期状態
}

void UIEditor::Update() {

	for (auto& sprite : sprites_) {

		sprite->Update();
	}
}

void UIEditor::Draw() {

	for (auto& sprite : sprites_) {

		sprite->Draw();
	}
}

void UIEditor::ImGui() {
#ifdef _DEBUG
	ImGui::Separator();

	if (currentState_ == UIState::TexturePreviewList) {

		// TexturePreview
		DisplayTexturePreviewList();
	} else if (currentState_ == UIState::CreateTextureScreen) {

		// Create
		DisplayCreateScreen();
	}

#endif // _DEBUG
}

void UIEditor::DisplayTexturePreviewList() {

	// スクロール可能領域
	const Vector2 scrollRegionArea = { 0.0f, 320.0f };

	if (ImGui::CollapsingHeader("TexturePreview")) {
		ImGui::BeginChild("TexturePreviewScrollRegion", ImVec2(scrollRegionArea.x, scrollRegionArea.y), true);

		// テクスチャのリストを取得
		std::vector<std::string> allTextureNames = NewMoonGame::GetTextureManager()->GetAllTextureNames();

		// プレビューサイズ調整
		ImGui::DragFloat2("previewTextureSize", &previewTextureSize_.x);
		ImGui::Separator();

		for (const auto& textureName : allTextureNames) {

			// テクスチャ名を表示
			if (ImGui::Selectable(textureName.c_str())) {
			}

			// state -> CreateTextureScreen
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {

				selectedTextureName = textureName;
				currentState_ = UIState::CreateTextureScreen;
			}

			// TexturePreview
			if (ImGui::IsItemHovered()) {

				D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = NewMoonGame::GetTextureManager()->GetTextureGpuHandle(textureName);
				ImGui::BeginTooltip();
				ImGui::Image(ImTextureID(gpuHandle.ptr), ImVec2(previewTextureSize_.x, previewTextureSize_.y));
				ImGui::EndTooltip();
			}

			ImGui::Separator();
		}

		ImGui::EndChild();
	}
}

void UIEditor::DisplayCreateScreen() {

	// 1番上のCreate座標
	const float topYOffset = 128;

	// state = TexturePreviewList
	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 70, topYOffset));
	if (ImGui::Button("Back")) {

		currentState_ = UIState::TexturePreviewList;
		return;
	}

	// TexturePreview
	ImGui::SetCursorPos(ImVec2(10, topYOffset));
	ImGui::Text("%s", selectedTextureName.c_str());
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = NewMoonGame::GetTextureManager()->GetTextureGpuHandle(selectedTextureName);
	ImGui::Image(ImTextureID(gpuHandle.ptr), ImVec2(previewTextureSize_.x, previewTextureSize_.y));

	// TexturePreviewの下
	const uint32_t textureToCreateDistance = 152;
	ImGui::SetCursorPos(ImVec2(10, previewTextureSize_.y + textureToCreateDistance));
	if (ImGui::Button("Create")) {

		// SpriteCreate
		auto newSprite = std::make_unique<Sprite>();
		newSprite->Init(selectedTextureName);

		sprites_.push_back(std::move(newSprite));

		currentState_ = UIState::TexturePreviewList;
		return;
	}
}
