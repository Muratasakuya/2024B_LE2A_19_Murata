#pragma once

// c++
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <numbers>

//===================================================================*/
//							Easing Methods
//===================================================================*/

float EaseInSine(float t);
float EaseOutSine(float t);
float EaseInOutSine(float t);
float EaseInQuad(float t);
float EaseOutQuad(float t);
float EaseInOutQuad(float t);
float EaseInCubic(float t);
float EaseOutCubic(float t);
float EaseInOutCubic(float t);
float EaseInQuart(float t);
float EaseOutQuart(float t);
float EaseInOutQuart(float t);
float EaseInQuint(float t);
float EaseOutQuint(float t);
float EaseInOutQuint(float t);
float EaseInExpo(float t);
float EaseOutExpo(float t);
float EaseInOutExpo(float t);
float EaseInCirc(float t);
float EaseOutCirc(float t);
float EaseInOutCirc(float t);

enum class EasingType {

	EaseInSine,
	EaseInQuad,
	EaseInCubic,
	EaseInQuart,
	EaseInQuint,
	EaseInExpo,
	EaseInCirc,

	EaseOutSine,
	EaseOutQuad,
	EaseOutCubic,
	EaseOutQuart,
	EaseOutQuint,
	EaseOutExpo,
	EaseOutCirc,

	EaseInOutSine,
	EaseInOutQuad,
	EaseInOutCubic,
	EaseInOutQuart,
	EaseInOutQuint,
	EaseInOutExpo,
	EaseInOutCirc
};

float EasedValue(EasingType easingType, float t);