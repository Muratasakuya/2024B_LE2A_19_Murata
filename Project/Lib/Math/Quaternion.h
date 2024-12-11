#pragma once

//===================================================================*/
//								include
//===================================================================*/

// c++
#include <vector>
#include <algorithm>

// 前方宣言
class Vector3;
class Matrix4x4;
template <typename tValue>
struct Keyframe;

/// <summary>
/// クォータニオン
/// </summary>
struct Quaternion {

	float x, y, z, w;

	/*-------------------------------------------------------------*/
	/// 算術演算子

	// +
	Quaternion operator+(const Quaternion& other) const;
	// *
	Quaternion operator*(const Quaternion& other) const;
	// -
	Quaternion operator-() const;

	// float*
	Quaternion operator*(float scalar) const;
	friend Quaternion operator*(float scalar, const Quaternion& q);

	// Vector3*
	Vector3 operator*(const Vector3& v) const;

	/*-------------------------------------------------------------*/
	/// 関数

	// 0.0f初期化
	void Init();

	// Euler -> Quaternion
	static Quaternion EulerToQuaternion(const Vector3& euler);
	// 積
	static Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);
	// 単位
	static Quaternion IdentityQuaternion();
	// 共役
	static Quaternion Conjugate(const Quaternion& quaternion);
	// ノルム
	static float Norm(const Quaternion& quaternion);
	// 正規化
	static Quaternion Normalize(const Quaternion& quaternion);
	// 逆
	static Quaternion Inverse(const Quaternion& quaternion);
	// 任意軸回転
	static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);
	// ベクトルをQuaternionで回転させた結果のベクトルを求める
	static Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);
	// 回転行列
	static Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);
	// 球面線形補完
	static Quaternion Slerp(Quaternion q0, const Quaternion& q1, float t);
	// 内積
	static float Dot(const Quaternion& q0, const Quaternion& q1);
	// 任意の時刻の値を取得
	static Quaternion CalculateValue(const std::vector<Keyframe<Quaternion>>& keyframes, float time);
	// 進行方向に向きを合わせる
	static Quaternion LookRotation(const Vector3& forward, const Vector3& up);

};