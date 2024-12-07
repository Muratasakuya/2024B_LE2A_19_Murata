#include "Vector4.h"

/*-------------------------------------------------------------*/
/// 算術演算子

// +
Vector4 Vector4::operator+(const Vector4& other) const {
	return { x + other.x, y + other.y, z + other.z,w + other.w };
}
// -
Vector4 Vector4::operator-(const Vector4& other) const {
	return { x - other.x, y - other.y, z - other.z,w - other.w };
}
// *
Vector4 Vector4::operator*(const Vector4& other) const {
	return { x * other.x, y * other.y, z * other.z,w * other.w };
}
// /
Vector4 Vector4::operator/(const Vector4& other) const {
	return { x / other.x, y / other.y, z / other.z ,w / other.w };
}

// +=
Vector4& Vector4::operator+=(const Vector4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

// -=
Vector4& Vector4::operator-=(const Vector4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

// bool
// 等価演算子 ==
bool Vector4::operator==(const Vector4& other) const {

	return x == other.x && y == other.y && z == other.z && w == other.w;
}

// 非等価演算子 !=
bool Vector4::operator!=(const Vector4& other) const {

	return !(*this == other);
}

/*-------------------------------------------------------------*/
/// 関数

// 0.0f初期化
void Vector4::Init() {

	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->w = 0.0f;
}

// 任意初期化
void Vector4::SetInit(float value) {

	this->x = value;
	this->y = value;
	this->z = value;
	this->w = value;
}

Vector4 Color::HSVtoRGB(float hue, float saturation, float value) {

	float c = value * saturation;
	float x = c * (1.0f - std::fabs(std::fmod(hue / 60.0f, 2.0f) - 1.0f));
	float m = value - c;

	float r, g, b;
	if (hue < 60.0f) {
		r = c; g = x; b = 0.0f;
	} else if (hue < 120.0f) {
		r = x; g = c; b = 0.0f;
	} else if (hue < 180.0f) {
		r = 0.0f; g = c; b = x;
	} else if (hue < 240.0f) {
		r = 0.0f; g = x; b = c;
	} else if (hue < 300.0f) {
		r = x; g = 0.0f; b = c;
	} else {
		r = c; g = 0.0f; b = x;
	}

	return Vector4(r + m, g + m, b + m, 1.0f);
}

// 16進数 -> Vector4 色変換
Vector4 Color::Convert(int color) {

	int r = (color >> 16) & 0xFF;
	int g = (color >> 8) & 0xFF;
	int b = color & 0xFF;
	int a = (color >> 24) & 0xFF;

	return Vector4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

// Color
Vector4 Color::White(float alpha) {

	return Vector4(1.0f, 1.0f, 1.0f, alpha);
}
Vector4 Color::Black(float alpha) {

	return Vector4(0.0f, 0.0f, 0.0f, alpha);
}
Vector4 Color::Red(float alpha) {

	return Vector4(1.0f, 0.0f, 0.0f, alpha);
}
Vector4 Color::Green(float alpha) {

	return Vector4(0.0f, 1.0f, 0.0f, alpha);
}
Vector4 Color::Blue(float alpha) {

	return Vector4(0.0f, 0.0f, 1.0f, alpha);
}