#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/2D/Sprite.h"

// c++
#include <memory>
#include <algorithm>
#include <functional>
#include <chrono>

//* 遷移の種類 *//
enum class TransitionType {

	Fade, // フェード //* FadeOut -> FadeIn
};

//* 遷移の状態 *//
enum class TransitionState {

	None,    // 遷移なし
	Begin,   // 遷移開始入
	Change,  // シーンの遷移待ち
	End,     // 遷移終了入
	Complete // 遷移完了
};

/*////////////////////////////////////////////////////////////////////////////////
*							TransitionScene Class
////////////////////////////////////////////////////////////////////////////////*/
class TransitionScene {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	TransitionScene() = default;
	~TransitionScene() = default;

	void Init();

	void Start(const TransitionType& type, std::function<void()> onSceneChange);

	void Update();

	void Render();

	// Setter
	void SetDuration(float duration);

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	TransitionType type_;
	TransitionState state_ = TransitionState::None;

	// 遷移にかかる時間
	float duration_ = 1.0f;
	std::chrono::time_point<std::chrono::steady_clock> startTime_;

	std::function<void()> onSceneChange_;

	std::unique_ptr<Sprite> sprite_;

	void BeginUpdate(float elapsedTime);
	void ChangeScene();
	void EndUpdate(float elapsedTime);

	//===================================================================*/
	// FadeMethods

	float AlphaUpdate() const;

};