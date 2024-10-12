#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Lib/Math/Vector4.h"

// c++
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

// 前方宣言
template <typename tValue>
struct Keyframe;
class Matrix4x4;

/// <summary>
/// 3次元ベクトル
/// </summary>
class Vector3 final {
public:

	float x, y, z;

	/*-------------------------------------------------------------*/
	/// 算術演算子

	// +
	Vector3 operator+(const Vector3& other) const;
	// -
	Vector3 operator-(const Vector3& other) const;
	// *
	Vector3 operator*(const Vector3& other) const;
	// /
	Vector3 operator/(const Vector3& other) const;

	// +=
	Vector3& operator+=(const Vector3& v);
	// -=
	Vector3& operator-=(const Vector3& v);

	// float*
	Vector3 operator*(float scalar) const;
	friend Vector3 operator*(float scalar, const Vector3& v);
	// float/
	Vector3 operator/(float scalar) const;
	friend Vector3 operator/(float scalar, const Vector3& v);

	// bool
	// 等価演算子 ==
	bool operator==(const Vector3& other) const;

	// 非等価演算子 !=
	bool operator!=(const Vector3& other) const;

	/*-------------------------------------------------------------*/
	/// 関数

	// 0.0f初期化
	void Init();
	// 任意初期化
	void SetInit(float value);

	// 三角形の頂点から法線の取得
	static Vector3 CalculateTriangleNormal(const Vector4& v0, const Vector4& v1, const Vector4& v2);

	// ノルム
	static float Length(const Vector3& v);
	// 正規化
	static Vector3 Normalize(const Vector3& v);

	// クロス
	static Vector3 Cross(const Vector3& v0, const Vector3& v1);

	// 任意の時刻の値を取得
	static Vector3  CalculateValue(const std::vector<Keyframe<Vector3>>& keyframes, float time);

	// 線形補完
	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

	// 4x4行列の座標変換
	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

};