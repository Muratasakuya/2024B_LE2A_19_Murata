#pragma once

#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

#include <numbers>

/// <summary>
/// 数学関数クラス
/// </summary>
struct Math {

	/*-------------------------------------------------------------*/
	/// 関数

	// ノルム
	static float Length(float a, float b);

	// 正規化
	static float Normalize(float a, float b);

	// 最短角度補完
	static float LerpShortAngle(float a, float b, float t);

};