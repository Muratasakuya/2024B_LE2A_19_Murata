#include "Collider.h"

#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Collider classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Collider::DrawCollider() {

	std::visit([&](const auto& shape) {
		using ShapeType = std::decay_t<decltype(shape)>;

		//* Sphere *//
		if constexpr (std::is_same_v<ShapeType, CollisionShapes::Sphere>) {

			const uint32_t kSubdivision = 8;                                         //* 分割数
			const float kLatEvery = std::numbers::pi_v<float> / kSubdivision;        //* 緯度
			const float kLonEvery = 2.0f * std::numbers::pi_v<float> / kSubdivision; //* 経度

			auto calculatePoint = [&](float lat, float lon) -> Vector3 {
				return {
					shape.radius * std::cos(lat) * std::cos(lon),
					shape.radius * std::sin(lat),
					shape.radius * std::cos(lat) * std::sin(lon)
				};
				};

			for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
				float lat = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex;
				for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
					float lon = lonIndex * kLonEvery;

					Vector3 pointA = calculatePoint(lat, lon);
					Vector3 pointB = calculatePoint(lat + kLatEvery, lon);
					Vector3 pointC = calculatePoint(lat, lon + kLonEvery);

					NewMoonGame::DrawLine3D(pointA + centerPos_, pointB + centerPos_, LineColor::Red);
					NewMoonGame::DrawLine3D(pointA + centerPos_, pointC + centerPos_, LineColor::Red);
				}
			}

		}
		//* AABB *//
		else if constexpr (std::is_same_v<ShapeType, CollisionShapes::AABB>) {

			Vector3 min = shape.min;
			Vector3 max = shape.max;

			std::vector<Vector3> vertices = {

				{shape.min.x, shape.min.y, shape.min.z},
				{shape.max.x, shape.min.y, shape.min.z},
				{shape.min.x, shape.max.y, shape.min.z},
				{shape.max.x, shape.max.y, shape.min.z},
				{shape.min.x, shape.min.y, shape.max.z},
				{shape.max.x, shape.min.y, shape.max.z},
				{shape.min.x, shape.max.y, shape.max.z},
				{shape.max.x, shape.max.y, shape.max.z}
			};

			std::vector<std::pair<uint32_t, uint32_t>> edges = {

				{0, 1}, {1, 3}, {3, 2}, {2, 0}, // 前面
				{4, 5}, {5, 7}, {7, 6}, {6, 4}, // 背面
				{0, 4}, {1, 5}, {2, 6}, {3, 7}  // 前面と背面を繋ぐ辺
			};

			for (const auto& edge : edges) {

				const Vector3& start = vertices[edge.first];
				const Vector3& end = vertices[edge.second];

				NewMoonGame::DrawLine3D(start, end, LineColor::Red);
			}

		}
		//* OBB *//
		else if constexpr (std::is_same_v<ShapeType, CollisionShapes::OBB>) {

			Vector3 size = shape.size;


		}
		}, shape_.value());
}

void Collider::SetCollisionShapeSphere(const CollisionShapes::Sphere& sphere) {
	shape_ = sphere;
	shapeType_ = ShapeType::Type_Sphere;

	NewMoonGame::AddCollider(this);
}

void Collider::SetCollisionShapeAABB(const CollisionShapes::AABB& aabb) {
	shape_ = aabb;
	shapeType_ = ShapeType::Type_AABB;

	NewMoonGame::AddCollider(this);
}

void Collider::SetCollisionShapeOBB(const CollisionShapes::OBB& obb) {
	shape_ = obb;
	shapeType_ = ShapeType::Type_OBB;

	NewMoonGame::AddCollider(this);
}
