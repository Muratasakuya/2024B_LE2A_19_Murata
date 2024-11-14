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

	Type_None,
	Type_Test,
};

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