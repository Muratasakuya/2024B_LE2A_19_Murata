#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Lib/Structure.h"

// c++
#include <optional>

//===================================================================*/
//							  ColliderType
//===================================================================*/
enum class ColliderType {
	Type_None = 0,        // ビットが立っていない状態
	Type_Test = 1 << 0,
};

// ビット演算のオーバーロード
inline ColliderType operator|(ColliderType lhs, ColliderType rhs) {
	using T = std::underlying_type_t<ColliderType>;
	return static_cast<ColliderType>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

inline ColliderType& operator|=(ColliderType& lhs, ColliderType rhs) {
	lhs = lhs | rhs;
	return lhs;
}

// ビットAND演算のオーバーロード
inline ColliderType operator&(ColliderType lhs, ColliderType rhs) {
	using T = std::underlying_type_t<ColliderType>;
	return static_cast<ColliderType>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

// ビットAND代入演算のオーバーロード
inline ColliderType& operator&=(ColliderType& lhs, ColliderType rhs) {
	lhs = lhs & rhs;
	return lhs;
}

/*////////////////////////////////////////////////////////////////////////////////
*								Collider Class
////////////////////////////////////////////////////////////////////////////////*/
class Collider {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	Collider() = default;
	virtual ~Collider() = default;

	virtual void OnCollisionEnter([[maybe_unused]] Collider* other) {};

	virtual void OnCollisionStay([[maybe_unused]] Collider* other) {};

	virtual void OnCollisionExit([[maybe_unused]] Collider* other) {};

	virtual void OBBUpdate();

	virtual void DrawCollider();

	// Setter
	void SetCollisionShapeSphere(const CollisionShapes::Sphere& sphere = CollisionShapes::Sphere::Default());

	void SetCollisionShapeOBB(const CollisionShapes::OBB& obb = CollisionShapes::OBB::Default());

	// Getter
	Vector3 GetCenterPos() const { return centerPos_; }
	Quaternion GetRotate() const { return rotate_; }
	Vector3 GetSize() const { return size_; }

	ColliderType GetType() const { return type_; }
	ColliderType GetTargetType() const { return targetType_; }

	CollisionShapes::Shapes GetCollisionShape() const { return shape_.value(); }

	ShapeType GetShapeType() const { return shapeType_.value(); }

protected:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	Vector3 centerPos_; //* 衝突判定用中心座標

	Quaternion rotate_; //* OBB衝突用の回転
	Vector3 size_;      //* OBB衝突用の大きさ

	ColliderType type_;       //* 自身のタイプ
	ColliderType targetType_; //* 衝突相手のタイプ

	std::optional<CollisionShapes::Shapes> shape_ = std::nullopt; //* 衝突判定を行う形状
	std::optional<ShapeType> shapeType_ = std::nullopt;           //* 衝突判定を行う形状のタイプ

};