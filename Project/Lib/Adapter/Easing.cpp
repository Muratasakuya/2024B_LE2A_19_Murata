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