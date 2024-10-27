#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Lib/Structure.h"

// c++
#include <optional>

//===================================================================*/
//							ColliderTypeId
//===================================================================*/
enum class ColliderType {

	Type_None,
	Type_PlayerBullet,
	Type_Enemy,
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

	virtual void OnCollisionEnter(Collider* other) {};

	virtual void OnCollisionStay(Collider* other) {};

	virtual void OnCollisionExit(Collider* other) {};

	// Setter
	void SetCollisionShapeSphere(const CollisionShapes::Sphere& sphere = CollisionShapes::Sphere::Default());

	void SetCollisionShapeAABB(const CollisionShapes::AABB& aabb = CollisionShapes::AABB::Default());

	void SetCollisionShapeOBB(const CollisionShapes::OBB& obb = CollisionShapes::OBB::Default());

	// Getter
	ColliderType GetType() const { return type_; }

	CollisionShapes::Shapes GetCollisionShape() const { return shape_.value(); }

	ShapeType GetShapeType() const { return shapeType_.value(); }

protected:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	ColliderType type_;      //* 自身のタイプ
	ColliderType targetType; //* 衝突相手のタイプ

	//* 何も設定されていない状態でCollisionManagerにセットされていた場合、
	//* -> Sphere(Default)で衝突判定を取る
	std::optional<CollisionShapes::Shapes> shape_ = std::nullopt; //* 衝突判定を行う形状
	std::optional<ShapeType> shapeType_ = std::nullopt;           //* 衝突判定を行う形状のタイプ

};