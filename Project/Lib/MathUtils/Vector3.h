#pragma once

//============================================================================*/
//	include
//============================================================================*/

//============================================================================*/
//	Vector3 class
//============================================================================*/
class Vector3 {

	float x, y, z;

	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	//========================================================================*/
	//	operators
	//========================================================================*/

	Vector3 operator+(const Vector3& other) const;
	Vector3 operator-(const Vector3& other) const;
	Vector3 operator*(const Vector3& other) const;
	Vector3 operator/(const Vector3& other) const;

	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);

	Vector3 operator*(float scalar) const;
	friend Vector3 operator*(float scalar, const Vector3& v);

	Vector3 operator/(float scalar) const;
	friend Vector3 operator/(float scalar, const Vector3& v);

	bool operator==(const Vector3& other) const;
	bool operator!=(const Vector3& other) const;

	//========================================================================*/
	//	functions
	//========================================================================*/

	void Init();

};