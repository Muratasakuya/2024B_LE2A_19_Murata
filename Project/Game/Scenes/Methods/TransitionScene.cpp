#define NOMINMAX
#include "TransitionScene.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"
#include "Game/Managers/SceneManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*						TransitionScene classMethods
////////////////////////////////////////////////////////////////////////////////*/

void TransitionScene::SetDuration(float duration) {

	duration_ = duration;
}

void TransitionScene::Init() {

	const std::string textureName = "windowSizeWhite";
	NewMoonGame::LoadTexture(textureName);

	sprite_ = std::make_unique<Sprite>();
	sprite_->Init(textureName);
	sprite_->SetPos({ NewMoon::kWindowWidthf / 2.0f,NewMoon::kWindowHeightf / 2.0f });
	sprite_->SetCenterAnchor();
}

void TransitionScene::Start(const TransitionType& type, std::function<void()> onSceneChange) {

	state_ = TransitionState::Begin;

	type_ = type;
	onSceneChange_ = onSceneChange;
	startTime_ = std::chrono::steady_clock::now();
}

void TransitionScene::Update() {

	// 遷移していない状態
	if (state_ == TransitionState::None || state_ == TransitionState::Complete) {
		return;
	}

	auto currentTime = std::chrono::steady_clock::now();
	float elapsedTime = std::chrono::duration<float>(currentTime - startTime_).count();

	switch (state_) {
	case TransitionState::Begin:

		// 遷移開始
		BeginUpdate(elapsedTime);

		break;
	case TransitionState::Change:

		// 遷移中
		ChangeScene();

		break;
	case TransitionState::End:

		// 遷移終了
		EndUpdate(elapsedTime);

		break;
	}
}

void TransitionScene::Render() {

	// 遷移していない状態
	if (state_ == TransitionState::None || state_ == TransitionState::Complete) {
		return;
	}

	float alpha = AlphaUpdate();

	sprite_->SetAlpha(alpha);
	sprite_->Update();
	sprite_->Draw();

}

void TransitionScene::BeginUpdate(float elapsedTime) {

	if (elapsedTime >= duration_) {

		state_ = TransitionState::Change;
		onSceneChange_();
		startTime_ = std::chrono::steady_clock::now();
	}
}

void TransitionScene::ChangeScene() {

	state_ = TransitionState::End;
	startTime_ = std::chrono::steady_clock::now();
}

void TransitionScene::EndUpdate(float elapsedTime) {

	if (elapsedTime >= duration_) {

		state_ = TransitionState::Complete;
		SceneManager::GetInstance()->SetTransitioning(false);
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								FadeMethods
////////////////////////////////////////////////////////////////////////////////*/

float TransitionScene::AlphaUpdate() const {

	auto currentTime = std::chrono::steady_clock::now();
	float elapsedTime = std::chrono::duration<float>(currentTime - startTime_).count();
	float alpha = 0.0f;

	if (state_ == TransitionState::Change) {
		return alpha = 1.0f;
	}

	if (state_ == TransitionState::Begin) {

		alpha = elapsedTime / duration_;
	} else if (state_ == TransitionState::End) {

		alpha = 1.0f - elapsedTime / duration_;
	}

	return std::clamp(alpha, 0.0f, 1.0f);
}
