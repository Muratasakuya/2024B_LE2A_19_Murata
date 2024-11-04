#include "Quaternion.h"

#include "Lib/Math/Vector3.h"
#include "Lib/Math/Matrix4x4.h"
#include "Lib/Structure.h"

/*-------------------------------------------------------------*/
/// 算術演算子

// +
Quaternion Quaternion::operator+(const Quaternion& other) const {
	return { x + other.x, y + other.y, z + other.z, w + other.w };
}
// *
Quaternion Quaternion::operator*(const Quaternion& other) const {
	return { w * other.x + x * other.w + y * other.z - z * other.y,
			w * other.y - x * other.z + y * other.w + z * other.x,
			w * other.z + x * other.y - y * other.x + z * other.w,
			w * other.w - x * other.x - y * other.y - z * other.z };
}
// -
Quaternion Quaternion::operator-() const {
	return { -x, -y, -z, -w };
}

// float*
Quaternion operator*(float scalar, const Quaternion& q) {
	return { scalar * q.x,scalar * q.y ,scalar * q.z ,scalar * q.w };
}
Quaternion Quaternion::operator*(float scalar) const {
	return { x * scalar, y * scalar, z * scalar, w * scalar };
}

/*-------------------------------------------------------------*/
/// 関数

void Quaternion::Init() {

	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->w = 1.0f;
}

Quaternion Quaternion::EulerToQuaternion(const Vector3& euler) {

	// オイラー角（ピッチ、ヨー、ロール）をラジアンに変換
	float pitch = euler.x * 0.5f;
	float yaw = euler.y * 0.5f;
	float roll = euler.z * 0.5f;

	// 各角度に対するサインとコサインを計算
	float sinPitch = std::sin(pitch);
	float cosPitch = std::cos(pitch);
	float sinYaw = std::sin(yaw);
	float cosYaw = std::cos(yaw);
	float sinRoll = std::sin(roll);
	float cosRoll = std::cos(roll);

	// クォータニオンの各成分を計算
	Quaternion q;
	q.w = cosYaw * cosPitch * cosRoll + sinYaw * sinPitch * sinRoll;
	q.x = cosYaw * sinPitch * cosRoll + sinYaw * cosPitch * sinRoll;
	q.y = sinYaw * cosPitch * cosRoll - cosYaw * sinPitch * sinRoll;
	q.z = cosYaw * cosPitch * sinRoll - sinYaw * sinPitch * cosRoll;

	return q;
}

// 積
Quaternion Quaternion::Multiply(const Quaternion& lhs, const Quaternion& rhs) {

	Quaternion result;

	result.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
	result.x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
	result.y = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x;
	result.z = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w;

	return result;
}
// 単位
Quaternion Quaternion::IdentityQuaternion() {

	return { 0.0f, 0.0f, 0.0f, 1.0f };
}
// 共役
Quaternion Quaternion::Conjugate(const Quaternion& quaternion) {

	return { -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
}
// ノルム
float Quaternion::Norm(const Quaternion& quaternion) {

	return std::sqrt(quaternion.x * quaternion.x + quaternion.y * quaternion.y +
		quaternion.z * quaternion.z + quaternion.w * quaternion.w);
}
// 正規化
Quaternion Quaternion::Normalize(const Quaternion& quaternion) {

	float norm = Norm(quaternion);
	return { quaternion.x / norm, quaternion.y / norm, quaternion.z / norm, quaternion.w / norm };
}
// 逆
Quaternion Quaternion::Inverse(const Quaternion& quaternion) {

	Quaternion conjugate = Conjugate(quaternion);
	float normSq = Norm(quaternion) * Norm(quaternion);

	return { conjugate.x / normSq, conjugate.y / normSq, conjugate.z / normSq, conjugate.w / normSq };
}
// 任意軸回転
Quaternion Quaternion::MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {

	Quaternion result{};

	float halfAngle = angle * 0.5f;
	float sinHalfAngle = std::sin(halfAngle);
	result.x = axis.x * sinHalfAngle;
	result.y = axis.y * sinHalfAngle;
	result.z = axis.z * sinHalfAngle;
	result.w = std::cos(halfAngle);

	return result;
}
// ベクトルをQuaternionで回転させた結果のベクトルを求める
Vector3 Quaternion::RotateVector(const Vector3& vector, const Quaternion& quaternion) {

	Quaternion qVector{ vector.x, vector.y, vector.z, 0.0f };
	Quaternion qConjugate = Conjugate(quaternion);
	Quaternion qResult = Multiply(Multiply(quaternion, qVector), qConjugate);

	return { qResult.x, qResult.y, qResult.z };
}
// 回転行列
Matrix4x4 Quaternion::MakeRotateMatrix(const Quaternion& quaternion) {

	Matrix4x4 result;
	float xx = quaternion.x * quaternion.x;
	float yy = quaternion.y * quaternion.y;
	float zz = quaternion.z * quaternion.z;
	float ww = quaternion.w * quaternion.w;
	float xy = quaternion.x * quaternion.y;
	float xz = quaternion.x * quaternion.z;
	float yz = quaternion.y * quaternion.z;
	float wx = quaternion.w * quaternion.x;
	float wy = quaternion.w * quaternion.y;
	float wz = quaternion.w * quaternion.z;

	result.m[0][0] = ww + xx - yy - zz;
	result.m[0][1] = 2.0f * (xy + wz);
	result.m[0][2] = 2.0f * (xz - wy);
	result.m[0][3] = 0.0f;

	result.m[1][0] = 2.0f * (xy - wz);
	result.m[1][1] = ww - xx + yy - zz;
	result.m[1][2] = 2.0f * (yz + wx);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 2.0f * (xz + wy);
	result.m[2][1] = 2.0f * (yz - wx);
	result.m[2][2] = ww - xx - yy + zz;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}
// 球面線形補完
Quaternion Quaternion::Slerp(Quaternion q0, const Quaternion& q1, float t) {

	/// q0とq1の内積
	float dot = Dot(q0, q1);

	// 内積が負の場合、もう片方の回転を利用する
	if (dot < 0.0f) {

		q0 = -q0;
		dot = -dot;
	}

	if (dot >= 1.0f - FLT_EPSILON) {

		return (1.0f - t) * q0 + t * q1;
	}

	// なす角を求める
	float theta = std::acos(dot);
	float sinTheta = std::sin(theta);

	// 補完係数を計算
	float scale0 = std::sin((1.0f - t) * theta) / sinTheta;
	float scale1 = std::sin(t * theta) / sinTheta;

	// 補完後のクォータニオンを求める
	return q0 * scale0 + q1 * scale1;
}
// 内積
float Quaternion::Dot(const Quaternion& q0, const Quaternion& q1) {

	return q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
}

// 任意の時刻の値を取得
Quaternion Quaternion::CalculateValue(const std::vector<Keyframe<Quaternion>>& keyframes, float time) {

	// キーがないものは返す値が分からないのでアウト
	assert(!keyframes.empty());

	// キーが1つか、時刻がキーフレーム前なら最初の値とする
	if (keyframes.size() == 1 || time <= keyframes[0].time) {

		return keyframes[0].value;
	}

	for (size_t index = 0; index < keyframes.size(); ++index) {

		size_t nextIndex = index + 1;

		// indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {

			// 範囲内を補完する
			float t =
				(time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);

			return Slerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	// ここまで来た場合は1番後の時刻よりも後ろなので最後の値を返す
	return (*keyframes.rbegin()).value;
}
