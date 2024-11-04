#include "CollisionManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							CollisionManager classMethods
////////////////////////////////////////////////////////////////////////////////*/

void CollisionManager::AddCollider(Collider* collider) {

	colliders_.push_back(collider);
}

void CollisionManager::RemoveCollider(Collider* collider) {

	auto itA = std::find(colliders_.begin(), colliders_.end(), collider);
	if (itA != colliders_.end()) {
		colliders_.erase(itA);
	}

	for (auto itB = preCollisions_.begin(); itB != preCollisions_.end();) {
		if (itB->first == collider || itB->second == collider) {

			itB = preCollisions_.erase(itB);
		} else {
			++itB;
		}
	}
}

void CollisionManager::ClearAllColliders() {

	colliders_.clear();
	preCollisions_.clear();
}

void CollisionManager::UpdateAllCollisions() {

	if (colliders_.empty()) {
		return;
	}

	std::unordered_set<std::pair<Collider*, Collider*>, pair_hash> currentCollisions;

	for (auto itA = colliders_.begin(); itA != colliders_.end(); ++itA) {

		auto itB = itA;
		++itB;

		for (; itB != colliders_.end(); ++itB) {

			Collider* colliderA = *itA;
			Collider* colliderB = *itB;

			if (colliderA->GetTargetType() != colliderB->GetType() &&
				colliderB->GetTargetType() != colliderA->GetType()) {
				continue;
			}

			if (IsColliding(colliderA, colliderB)) {
				currentCollisions.insert({ colliderA, colliderB });

				if (preCollisions_.find({ colliderA, colliderB }) == preCollisions_.end()) {

					colliderA->OnCollisionEnter(colliderB);
					colliderB->OnCollisionEnter(colliderA);
				} else {

					colliderA->OnCollisionStay(colliderB);
					colliderB->OnCollisionStay(colliderA);
				}
			}
		}
	}

	for (auto& collision : preCollisions_) {
		if (currentCollisions.find(collision) == currentCollisions.end()) {

			collision.first->OnCollisionExit(collision.second);
			collision.second->OnCollisionExit(collision.first);
		}
	}

	preCollisions_ = currentCollisions;
}

/*////////////////////////////////////////////////////////////////////////////////
*							Collision Methods
////////////////////////////////////////////////////////////////////////////////*/

bool CollisionManager::IsColliding(Collider* colliderA, Collider* colliderB) {

	//* Type &6 Center *//

	const auto& shapeA = colliderA->GetCollisionShape();
	const auto& shapeB = colliderB->GetCollisionShape();

	const Vector3 centerA = colliderA->GetCenterPos();
	const Vector3 centerB = colliderB->GetCenterPos();

	return std::visit([&](const auto& shapeA, const auto& shapeB) {

		using ShapeTypeA = std::decay_t<decltype(shapeA)>;
		using ShapeTypeB = std::decay_t<decltype(shapeB)>;

		if constexpr (std::is_same_v<ShapeTypeA, CollisionShapes::Sphere> && std::is_same_v<ShapeTypeB, CollisionShapes::Sphere>) {

			return SphereToSphere(shapeA, shapeB, centerA, centerB);
		} else if constexpr (std::is_same_v<ShapeTypeA, CollisionShapes::Sphere> && std::is_same_v<ShapeTypeB, CollisionShapes::OBB>) {

			return SphereToOBB(shapeA, shapeB, centerA);
		} else if constexpr (std::is_same_v<ShapeTypeA, CollisionShapes::OBB> && std::is_same_v<ShapeTypeB, CollisionShapes::OBB>) {

			return OBBToOBB(shapeA, shapeB);
		} else {

			return false;
		}
		}, shapeA, shapeB);
}

bool CollisionManager::SphereToSphere(
	const CollisionShapes::Sphere& sphereA, const CollisionShapes::Sphere& sphereB,
	const Vector3& centerA, const Vector3& centerB) {

	float distance = Vector3::Length(centerA - centerB);
	if (distance <= sphereA.radius + sphereB.radius) {
		return true;
	}

	return false;
}

bool CollisionManager::SphereToOBB(
	const CollisionShapes::Sphere& sphere, const CollisionShapes::OBB& obb,
	const Vector3& sphereCenter) {

	Matrix4x4 rotateX = Matrix4x4::MakePitchMatrix(obb.rotate.x);
	Matrix4x4 rotateY = Matrix4x4::MakeYawMatrix(obb.rotate.y);
	Matrix4x4 rotateZ = Matrix4x4::MakeRollMatrix(obb.rotate.z);

	Matrix4x4 rotateMatrix = rotateX * rotateY * rotateZ;

	Vector3 orientations[3];
	orientations[0] = Vector3::Transform({ 1.0f,0.0f,0.0f }, rotateMatrix);
	orientations[1] = Vector3::Transform({ 0.0f,1.0f,0.0f }, rotateMatrix);
	orientations[2] = Vector3::Transform({ 0.0f,0.0f,1.0f }, rotateMatrix);

	Vector3 localSphereCenter = sphereCenter - obb.center;
	Vector3 closestPoint = obb.center;

	for (int i = 0; i < 3; ++i) {

		float distance = Vector3::Dot(localSphereCenter, orientations[i]);
		float halfSize = (i == 0) ? obb.size.x : (i == 1) ? obb.size.y : obb.size.z;

		if (distance > halfSize) {
			distance = halfSize;
		} else if (distance < -halfSize) {
			distance = -halfSize;
		}

		closestPoint += distance * orientations[i];
	}

	Vector3 diff = closestPoint - sphereCenter;
	float distanceSquared = Vector3::Dot(diff, diff);

	return distanceSquared <= (sphere.radius * sphere.radius);
}

bool CollisionManager::OBBToOBB(const CollisionShapes::OBB& obbA, const CollisionShapes::OBB& obbB) {

	// 上手くいっていない
	obbA;
	obbB;

	return false;
}
