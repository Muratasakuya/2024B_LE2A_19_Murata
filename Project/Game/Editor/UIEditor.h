#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/2D/Sprite.h"

// c++
#include <memory>
#include <vector>
#include <string>

/*////////////////////////////////////////////////////////////////////////////////
*								UIEditor Class
////////////////////////////////////////////////////////////////////////////////*/
class UIEditor {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	UIEditor() = default;
	~UIEditor() = default;

	void Init();

	void Update();

	void Draw();

	void ImGui();

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//* EditorSelectState *//
	enum class UIState {

		TexturePreviewList, // 全てのテクスチャプレビュー表示画面
		CreateTextureScreen // Create画面
	};

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	Vector2 previewTextureSize_;

	std::vector<std::unique_ptr<Sprite>> sprites_;

	UIState currentState_;
	std::string selectedTextureName;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	void DisplayTexturePreviewList();
	void DisplayCreateScreen();

};