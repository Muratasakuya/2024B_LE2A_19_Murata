#include "Easing.h"

// Ease In Sine
float EaseInSine(float t) {
	return 1.0f - std::cos((t * std::numbers::pi_v<float>) / 2.0f);
}

// Ease Out Sine
float EaseOutSine(float t) {
	return std::sin((t * std::numbers::pi_v<float>) / 2.0f);
}

// Ease In Out Sine
float EaseInOutSine(float t) {
	return -(std::cos(std::numbers::pi_v<float> *t) - 1.0f) / 2.0f;
}

// Ease In Quad
float EaseInQuad(float t) {
	return t * t;
}

// Ease Out Quad
float EaseOutQuad(float t) {
	return 1.0f - (1.0f - t) * (1.0f - t);
}

// Ease In Out Quad
float EaseInOutQuad(float t) {
	return t < 0.5f ? 2.0f * t * t : 1.0f - std::powf(-2.0f * t + 2.0f, 2.0f) / 2.0f;
}

// Ease In Cubic
float EaseInCubic(float t) {
	return t * t * t;
}

// Ease Out Cubic
float EaseOutCubic(float t) {
	return 1.0f - std::powf(1.0f - t, 3.0f);
}

// Ease In Out Cubic
float EaseInOutCubic(float t) {
	return t < 0.5f ? 4.0f * t * t * t : 1.0f - std::powf(-2.0f * t + 2.0f, 3.0f) / 2.0f;
}

// Ease In Quart
float EaseInQuart(float t) {
	return t * t * t * t;
}

// Ease Out Quart
float EaseOutQuart(float t) {
	return 1.0f - std::powf(1.0f - t, 4.0f);
}

// Ease In Out Quart
float EaseInOutQuart(float t) {
	return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - std::powf(-2.0f * t + 2.0f, 4.0f) / 2.0f;
}

// Ease In Quint
float EaseInQuint(float t) {
	return t * t * t * t * t;
}

// Ease Out Quint
float EaseOutQuint(float t) {
	return 1.0f - std::powf(1.0f - t, 5.0f);
}

// Ease In Out Quint
float EaseInOutQuint(float t) {
	return t < 0.5f ? 16.0f * t * t * t * t * t : 1.0f - std::powf(-2.0f * t + 2.0f, 5.0f) / 2.0f;
}

// Ease In Expo
float EaseInExpo(float t) {
	return t == 0.0f ? 0.0f : std::powf(2.0f, 10.0f * t - 10.0f);
}

// Ease Out Expo
float EaseOutExpo(float t) {
	return t == 1.0f ? 1.0f : 1.0f - std::powf(2.0f, -10.0f * t);
}

// Ease In Out Expo
float EaseInOutExpo(float t) {
	if (t == 0.0f) return 0.0f;
	if (t == 1.0f) return 1.0f;
	return t < 0.5f ? std::powf(2.0f, 20 * t - 10.0f) / 2.0f : (2.0f - std::powf(2.0f, -20.0f * t + 10.0f)) / 2.0f;
}

// Ease In Circ
float EaseInCirc(float t) {
	return 1.0f - std::sqrtf(1.0f - std::powf(t, 2.0f));
}

// Ease Out Circ
float EaseOutCirc(float t) {
	return std::sqrtf(1.0f - std::powf(t - 1.0f, 2.0f));
}

// Ease In Out Circ
float EaseInOutCirc(float t) {
	return t < 0.5f ? (1.0f - std::sqrtf(1.0f - std::powf(2.0f * t, 2.0f))) / 2.0f
		: (std::sqrtf(1.0f - std::powf(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
}

float EasedValue(EasingType easingType, float t) {

	switch (easingType) {
	case EasingType::EaseInSine: return EaseInSine(t);
	case EasingType::EaseOutSine: return EaseOutSine(t);
	case EasingType::EaseInOutSine: return EaseInOutSine(t);
	case EasingType::EaseInQuad: return EaseInQuad(t);
	case EasingType::EaseOutQuad: return EaseOutQuad(t);
	case EasingType::EaseInOutQuad: return EaseInOutQuad(t);
	case EasingType::EaseInCubic: return EaseInCubic(t);
	case EasingType::EaseOutCubic: return EaseOutCubic(t);
	case EasingType::EaseInOutCubic: return EaseInOutCubic(t);
	case EasingType::EaseInQuart: return EaseInQuart(t);
	case EasingType::EaseOutQuart: return EaseOutQuart(t);
	case EasingType::EaseInOutQuart: return EaseInOutQuart(t);
	case EasingType::EaseInQuint: return EaseInQuint(t);
	case EasingType::EaseOutQuint: return EaseOutQuint(t);
	case EasingType::EaseInOutQuint: return EaseInOutQuint(t);
	case EasingType::EaseInExpo: return EaseInExpo(t);
	case EasingType::EaseOutExpo: return EaseOutExpo(t);
	case EasingType::EaseInOutExpo: return EaseInOutExpo(t);
	case EasingType::EaseInCirc: return EaseInCirc(t);
	case EasingType::EaseOutCirc: return EaseOutCirc(t);
	case EasingType::EaseInOutCirc: return EaseInOutCirc(t);
	default: return t;
	}
}
