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

// 16進数 -> Vector4 色変換
Vector4 Color::Convert(int color) {

	int r = (color >> 16) & 0xFF;
	int g = (color >> 8) & 0xFF;
	int b = color & 0xFF;
	int a = (color >> 24) & 0xFF;

	return Vector4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

// Color
Vector4 Color::White() {

	return Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}
Vector4 Color::Black() {

	return Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}
Vector4 Color::Red() {

	return Vector4(1.0f, 0.0f, 0.0f, 1.0f);
}
Vector4 Color::Green() {

	return Vector4(0.0f, 1.0f, 0.0f, 1.0f); ;
}
Vector4 Color::Blue() {

	return Vector4(0.0f, 0.0f, 1.0f, 1.0f); ;
}