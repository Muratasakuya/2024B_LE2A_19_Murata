#include "Vector3.h"

//============================================================================*/
//	Vector3 classMethods
//============================================================================*/

Vector3 Vector3::operator+(const Vector3& other) const {
	return Vector3(x + other.x, y + other.y, z + other.z);
}
Vector3 Vector3::operator-(const Vector3& other) const {
	return Vector3(x - other.x, y - other.y, z - other.z);
}
Vector3 Vector3::operator*(const Vector3& other) const {
	return Vector3(x * other.x, y * other.y, z * other.z);
}
Vector3 Vector3::operator/(const Vector3& other) const {
	return Vector3(x / other.x, y / other.y, z / other.z);
}

Vector3& Vector3::operator+=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
Vector3& Vector3::operator-=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3 Vector3::operator*(float scalar) const {

	return Vector3(x * scalar, y * scalar, z * scalar);
}
Vector3 operator*(float scalar, const Vector3& v) {

	return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
}
Vector3 Vector3::operator/(float scalar) const {

	return Vector3(x / scalar, y / scalar, z / scalar);
}
Vector3 operator/(float scalar, const Vector3& v) {

	return Vector3(v.x / scalar, v.y / scalar, v.z / scalar);
}

bool Vector3::operator==(const Vector3& other) const {

	return x == other.x && y == other.y && z == other.z;
}
bool Vector3::operator!=(const Vector3& other) const {

	return !(*this == other);
}
