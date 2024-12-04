#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/2D/Sprite.h"

// c++
#include <memory>
#include <algorithm>

/*////////////////////////////////////////////////////////////////////////////////
*								Transition Class
////////////////////////////////////////////////////////////////////////////////*/
class Transition {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	//* TransitionState *//
	enum class TransitionState {

		Begin, //* 開始
		Wait,  //* 待ち
		End    //* 終了
	};

public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	Transition() = default;
	~Transition();

	void Init();

	void Update();

	void Draw();

	void ImGui();

	//* Setter *//

	void SetTransition(bool transition) { isTransition_ = transition; }

	//* Getter *//

	bool IsNextScene() const { return nextScene_; }
	bool IsTransition() const { return isTransition_; }

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	TransitionState transitionState_; //* 遷移状態

	bool isTransition_; //* 遷移フラグ
	bool nextScene_;    //* 次のシーンに進める

	//* Mainparameter *//

	float transitionTime_;  //* 遷移にかかる時間
	float transitionTimer_; //* 遷移を管理するタイマー

	float waitTime_;  //* 待ち時間
	float waitTimer_; //* 待ち時間を管理するタイマー

	//* Fade *//

	std::unique_ptr<Sprite> fadeScreen_;

	Vector4 fadeColor_; //* Fadeの色

	//===================================================================*/
	///* functions

	void WaitForTimer();

	//* Fade *//
	void FadeOut();
	void FadeIn();

};