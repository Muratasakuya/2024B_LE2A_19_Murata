#include "Vector3.h"

#include "Engine/Methods/ScreenDimensions.h"
#include "Lib/Structure.h"

/*-------------------------------------------------------------*/
/// 算術演算子

// +
Vector3 Vector3::operator+(const Vector3& other) const {
	return { x + other.x, y + other.y, z + other.z };
}
// -
Vector3 Vector3::operator-(const Vector3& other) const {
	return { x - other.x, y - other.y, z - other.z };
}
// *
Vector3 Vector3::operator*(const Vector3& other) const {
	return { x * other.x, y * other.y, z * other.z };
}
// /
Vector3 Vector3::operator/(const Vector3& other) const {
	return { x / other.x, y / other.y, z / other.z };
}

// +=
Vector3& Vector3::operator+=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

// -=
Vector3& Vector3::operator-=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

// float*
Vector3 Vector3::operator*(float scalar) const {

	return Vector3(x * scalar, y * scalar, z * scalar);
}
Vector3 operator*(float scalar, const Vector3& v) {

	return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
}
// float/
Vector3 Vector3::operator/(float scalar) const {

	return Vector3(x / scalar, y / scalar, z / scalar);
}
Vector3 operator/(float scalar, const Vector3& v) {

	return Vector3(v.x / scalar, v.y / scalar, v.z / scalar);
}

// bool
// 等価演算子 ==
bool Vector3::operator==(const Vector3& other) const {

	return x == other.x && y == other.y && z == other.z;
}

// 非等価演算子 !=
bool Vector3::operator!=(const Vector3& other) const {

	return !(*this == other);
}

/*-------------------------------------------------------------*/
/// 関数

// 0.0f初期化
void Vector3::Init() {

	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
}

// 任意初期化
void Vector3::SetInit(float value) {

	this->x = value;
	this->y = value;
	this->z = value;
}

// 三角形の頂点から法線の取得
Vector3 Vector3::CalculateTriangleNormal(const Vector4& v0, const Vector4& v1, const Vector4& v2) {

	// 頂点間のベクトルを計算
	Vector3 vec1 = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
	Vector3 vec2 = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };

	// 外積を計算して法線ベクトルを得る
	Vector3 normal;
	normal.x = vec1.y * vec2.z - vec1.z * vec2.y;
	normal.y = vec1.z * vec2.x - vec1.x * vec2.z;
	normal.z = vec1.x * vec2.y - vec1.y * vec2.x;

	// 法線ベクトルを正規化する
	float length = std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
	normal.x /= length;
	normal.y /= length;
	normal.z /= length;

	return normal;
}

// ノルム
float Vector3::Length(const Vector3& v) {

	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

// 正規化
Vector3 Vector3::Normalize(const Vector3& v) {

	float length = Length(v);
	if (length != 0) {
		return Vector3(v.x / length, v.y / length, v.z / length);
	} else {

		// 値が入ってなければnullで返す
		return Vector3(0.0f, 0.0f, 0.0f);
	}
}

// クロス
Vector3 Vector3::Cross(const Vector3& v0, const Vector3& v1) {
	return {
	  v0.y * v1.z - v0.z * v1.y,
	  v0.z * v1.x - v0.x * v1.z,
	  v0.x * v1.y - v0.y * v1.x
	};
}

// 任意の時刻の値を取得
Vector3 Vector3::CalculateValue(const std::vector<Keyframe<Vector3>>& keyframes, float time) {

	// キーがないものは返す値が分からないのでアウト
	assert(!keyframes.empty());

	// キーが1つか、時刻がキーフレーム前なら最初の値とする
	if (keyframes.size() == 1 || time <= keyframes[0].time) {

		return keyframes[0].value;
	}

	for (size_t index = 0; index < keyframes.size(); index++) {

		size_t nextIndex = index + 1;

		// indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {

			// 範囲内を補完する
			float t =
				(time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);

			return Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	// ここまで来た場合は1番後の時刻よりも後ろなので最後の値を返す
	return (*keyframes.rbegin()).value;
}

// 線形補完
Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t) {

	return Vector3(
		v1.x + t * (v2.x - v1.x),
		v1.y + t * (v2.y - v1.y),
		v1.z + t * (v2.z - v1.z)
	);
}