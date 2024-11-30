#include "Collider.h"

#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Collider classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Collider::OBBUpdate() {

	if (shape_ && std::holds_alternative <CollisionShapes::OBB>(*shape_)) {
		CollisionShapes::OBB& obb = std::get<CollisionShapes::OBB>(*shape_);

		obb.center = centerPos_;
		obb.rotate = rotate_;
		obb.size = size_;
	} else {

		assert(false && "collisionShape is not OBB");
	}
}

void Collider::DrawCollider(const LineColor& lineColor) {

	std::visit([&](const auto& shape) {
		using ShapeType = std::decay_t<decltype(shape)>;

		//* Sphere *//
		if constexpr (std::is_same_v<ShapeType, CollisionShapes::Sphere>) {

			NewMoonGame::DrawSphere(shape.radius, centerPos_, lineColor);
		}
		//* OBB *//
		else if constexpr (std::is_same_v<ShapeType, CollisionShapes::OBB>) {

			NewMoonGame::DrawOBB(size_, rotate_, centerPos_, lineColor);
		}
		}, shape_.value());
}

void Collider::SetCollisionShapeSphere(const CollisionShapes::Sphere& sphere) {
	shape_ = sphere;
	shapeType_ = ShapeType::Type_Sphere;

	NewMoonGame::AddCollider(this);
}

void Collider::SetCollisionShapeOBB(const CollisionShapes::OBB& obb) {
	shape_ = obb;
	shapeType_ = ShapeType::Type_OBB;

	NewMoonGame::AddCollider(this);
}
