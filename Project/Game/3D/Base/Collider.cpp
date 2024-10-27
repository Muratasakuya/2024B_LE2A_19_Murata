#include "Collider.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Collider classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Collider::SetCollisionShapeSphere(const CollisionShapes::Sphere& sphere) {
	shape_ = sphere;
	shapeType_ = ShapeType::Type_Sphere;
}

void Collider::SetCollisionShapeAABB(const CollisionShapes::AABB& aabb) {
	shape_ = aabb;
	shapeType_ = ShapeType::Type_AABB;
}

void Collider::SetCollisionShapeOBB(const CollisionShapes::OBB& obb) {
	shape_ = obb;
	shapeType_ = ShapeType::Type_OBB;
}
