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
		//* OBB *//
		else if constexpr (std::is_same_v<ShapeType, CollisionShapes::OBB>) {

			const uint32_t vertexNum = 8;

			Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(rotate_);

			Vector3 vertices[vertexNum];
			Vector3 halfSizeX = Vector3::Transform({ 1.0f,0.0f,0.0f }, rotateMatrix) * size_.x;
			Vector3 halfSizeY = Vector3::Transform({ 0.0f,1.0f,0.0f }, rotateMatrix) * size_.y;
			Vector3 halfSizeZ = Vector3::Transform({ 0.0f,0.0f,1.0f }, rotateMatrix) * size_.z;

			Vector3 offsets[vertexNum] = {
				{-1, -1, -1}, {-1,  1, -1}, {1, -1, -1}, {1,  1, -1},
				{-1, -1,  1}, {-1,  1,  1}, {1, -1,  1}, {1,  1,  1}
			};

			for (int i = 0; i < vertexNum; ++i) {

				Vector3 localVertex = offsets[i].x * halfSizeX +
					offsets[i].y * halfSizeY +
					offsets[i].z * halfSizeZ;
				vertices[i] = centerPos_ + localVertex;
			}

			int edges[12][2] = {
				{0, 1}, {1, 3}, {3, 2}, {2, 0},
				{4, 5}, {5, 7}, {7, 6}, {6, 4},
				{0, 4}, {1, 5}, {2, 6}, {3, 7}
			};

			for (int i = 0; i < 12; ++i) {

				int start = edges[i][0];
				int end = edges[i][1];

				NewMoonGame::DrawLine3D(vertices[start], vertices[end], LineColor::Red);
			}
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
