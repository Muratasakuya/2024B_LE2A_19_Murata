#include "Math.h"

/*-------------------------------------------------------------*/
/// 関数

// ノルム
float Math::Length(float a, float b) {

	return std::sqrtf(a * a + b * b);
}

// 正規化
float Math::Normalize(float a, float b) {

	if (b != 0) {
		return a / b;
	} else {
		return a;
	}
}

// 最短角度補完
float Math::LerpShortAngle(float a, float b, float t) {

	// 角度差分を求める
	float diff = b - a;

	// 角度を[-2PI, +2PI]に補正する
	diff = fmod(diff, std::numbers::pi_v<float> *2.0f);

	// 角度を[-PI, +PI]に補正する
	if (diff < -std::numbers::pi_v<float>) {

		diff += std::numbers::pi_v<float> *2.0f;
	} else if (diff > std::numbers::pi_v<float>) {

		diff -= std::numbers::pi_v<float> *2.0f;
	}

	// 最短角度で線形補完する
	return a + t * diff;
}