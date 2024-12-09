#pragma once

#include <cmath>

/// <summary>
/// 4次元ベクトル
/// </summary>
class Vector4 final {
public:

	float x, y, z, w;

	/*-------------------------------------------------------------*/
	/// 算術演算子

	// +
	Vector4 operator+(const Vector4& other) const;
	// -
	Vector4 operator-(const Vector4& other) const;
	// *
	Vector4 operator*(const Vector4& other) const;
	// /
	Vector4 operator/(const Vector4& other) const;

	// +=
	Vector4& operator+=(const Vector4& v);
	// -=
	Vector4& operator-=(const Vector4& v);

	// bool
	// 等価演算子 ==
	bool operator==(const Vector4& other) const;

	// 非等価演算子 !=
	bool operator!=(const Vector4& other) const;

	/*-------------------------------------------------------------*/
	/// 関数

	// 0.0f初期化
	void Init();
	// 任意初期化
	void SetInit(float value);

};

namespace Color {

	Vector4 HSVtoRGB(float hue, float saturation, float value);

	// 16進数 -> Vector4 色変換
	Vector4 Convert(int color);

	Vector4 White(float alpha = 1.0f);
	Vector4 Black(float alpha = 1.0f);
	Vector4 Red(float alpha = 1.0f);
	Vector4 Green(float alpha = 1.0f);
	Vector4 Blue(float alpha = 1.0f);
}