#pragma once

#include "Lib/Structure.h"

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
	EaseOutSine,
	EaseInOutSine,
	EaseInQuad,
	EaseOutQuad,
	EaseInOutQuad,
	EaseInCubic,
	EaseOutCubic,
	EaseInOutCubic,
	EaseInQuart,
	EaseOutQuart,
	EaseInOutQuart,
	EaseInQuint,
	EaseOutQuint,
	EaseInOutQuint,
	EaseInExpo,
	EaseOutExpo,
	EaseInOutExpo,
	EaseInCirc,
	EaseOutCirc,
	EaseInOutCirc
};