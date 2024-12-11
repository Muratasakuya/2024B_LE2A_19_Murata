#pragma once

//* ここからコメントアウトは左詰め *//

//===================================================================*/
//	include
//===================================================================*/
#include "Game/Components/BaseTransform.h"
#include "Engine/Methods/Input.h"

/*==============================================================================*/
/// Physics EnumClass
/*==============================================================================*/
enum class KeyInformation : uint8_t {
	None = 0,
	MoveLeft = 1 << 1,    //* 左移動
	MoveRight = 1 << 2,   //* 右移動
	MoveForward = 1 << 3, //* 前移動
	MoveBackward = 1 << 4 //* 後移動
};
// bitCalOperator //
inline KeyInformation operator|(KeyInformation lhs, KeyInformation rhs) {
	return static_cast<KeyInformation>(
		static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}
inline KeyInformation operator&(KeyInformation lhs, KeyInformation rhs) {
	return static_cast<KeyInformation>(
		static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}
inline bool operator!=(KeyInformation lhs, KeyInformation rhs) {
	return static_cast<uint8_t>(lhs) != static_cast<uint8_t>(rhs);
}

/*////////////////////////////////////////////////////////////////////////////////
*	MoveUtility Class
////////////////////////////////////////////////////////////////////////////////*/
class MoveUtility {
public:
	//===================================================================*/
	//	public Methods
	//===================================================================*/

	//===================================================================*/
	///* functions

	MoveUtility() = default;
	~MoveUtility() = default;

	//* init *//

	void AllKey();

	//* methods *//

	void KeyMove(BaseTransform& transform);
	void GamePadMove(BaseTransform& transform);

	void Jump(BaseTransform& transform, BYTE keyNumber);
	void GamePadJamp(BaseTransform& transform, InputGamePadButtons padButton);

	void RotateToDirection(BaseTransform& transform, float lerpRate = 0.25f);

	//===================================================================*/
	///* variables

	Vector3 velocity;

	KeyInformation keyInfo;

	//* jumpParameter *//

	float jumpStrength;   //* ジャンプ力
	float groundY = 0.0f; //* 地面の高さ

private:
	//===================================================================*/
	//	private Methods
	//===================================================================*/

	Vector3 move_;                //* 移動量

	const float gravity_ = -9.8f; //* 重力

	bool isGrounded_ = true;      //* 地面に着いているかどうか

};