#include "Transition.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include <imgui.h>

/*////////////////////////////////////////////////////////////////////////////////
*							Transition classMethods
////////////////////////////////////////////////////////////////////////////////*/

//================================================================================
// Main
//================================================================================

Transition::~Transition() {
	int deleteTest = 1;
	deleteTest = 0;
}

void Transition::Init() {

	transitionState_ = TransitionState::Begin;

	isTransition_ = false;
	nextScene_ = false;

	transitionTime_ = 6.0f;
	waitTime_ = 0.5f;

	transitionTimer_ = 0.0f;
	waitTimer_ = 0.0f;

	//================================================================================
	//* Fade *//
	//================================================================================

	fadeScreen_ = std::make_unique<Sprite>();
	fadeScreen_->Init("screenSizeWhite");
	fadeScreen_->SetCenterAnchor();
	fadeScreen_->SetPos(Vector2(NewMoon::kWindowWidthf / 2.0f, NewMoon::kWindowHeightf / 2.0f));

	fadeColor_ = Color::White(0.0f);

	fadeScreen_->SetColor(fadeColor_);

}

void Transition::Update() {

	if (!isTransition_) {
		return;
	}

	switch (transitionState_) {
	case Transition::TransitionState::Begin: {

		// 遷移入り
		FadeOut();
		break;
	}
	case Transition::TransitionState::Wait: {

		// 待ち時間
		WaitForTimer();
		break;
	}
	case Transition::TransitionState::End: {

		// 遷移終わり
		FadeIn();
		break;
	}
	}

}

void Transition::Draw() {

	fadeScreen_->Draw();

}

void Transition::ImGui() {
#ifdef _DEBUG

	ImGui::Separator();
	// Transition progress
	float transitionProgress = (transitionTime_ > 0.0f)
		? (transitionTimer_ / transitionTime_)
		: 0.0f; // 0除算を防ぐ
	ImGui::Text("Transition Progress:");
	ImGui::ProgressBar(transitionProgress, ImVec2(-1.0f, 0.0f));

	// Wait progress
	float waitProgress = (waitTime_ > 0.0f)
		? (waitTimer_ / waitTime_)
		: 0.0f;
	ImGui::Text("Wait Progress:");
	ImGui::ProgressBar(waitProgress, ImVec2(-1.0f, 0.0f));

	ImGui::ColorEdit4("FadeColor", &fadeColor_.x);
	fadeScreen_->SetColor(fadeColor_);
	fadeScreen_->Update();

#endif // _DEBUG
}

//================================================================================
// Transition
//================================================================================

void Transition::WaitForTimer() {

	nextScene_ = false;

	// ここではすでに次のシーンになっている

	waitTimer_ += NewMoonGame::GetDeltaTime();

	if (waitTimer_ >= waitTime_) {

		// 次のStateに遷移
		waitTimer_ = 0.0f;
		transitionState_ = TransitionState::End;
	}

}

void Transition::FadeOut() {

	transitionTimer_ += NewMoonGame::GetDeltaTime();

	float t = transitionTimer_ / transitionTime_;
	fadeColor_.w = std::clamp(t, 0.0f, 1.0f);

	if (transitionTimer_ >= transitionTime_) {

		// 次のシーンに進める && stateを待ちにする
		nextScene_ = true;
		transitionTimer_ = 0.0;
		transitionState_ = TransitionState::Wait;
	}

}

void Transition::FadeIn() {

	transitionTimer_ += NewMoonGame::GetDeltaTime();

	float t = transitionTimer_ / transitionTime_;
	fadeColor_.w = 1.0f - std::clamp(t, 0.0f, 1.0f);

	if (fadeColor_.w <= 0.0f) {

		// 次に備えてリセット
		isTransition_ = false;
		transitionTimer_ = 0.0f;
		transitionState_ = TransitionState::Begin;
	}
}