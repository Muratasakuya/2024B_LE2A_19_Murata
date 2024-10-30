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
		*itA = nullptr;
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
		} else if constexpr (std::is_same_v<ShapeTypeA, CollisionShapes::Sphere> && std::is_same_v<ShapeTypeB, CollisionShapes::AABB>) {

			return SphereToAABB(shapeA, shapeB, centerA, centerB);
		} else if constexpr (std::is_same_v<ShapeTypeA, CollisionShapes::Sphere> && std::is_same_v<ShapeTypeB, CollisionShapes::OBB>) {

			return SphereToOBB(shapeA, shapeB, centerA, centerB);
		} else if constexpr (std::is_same_v<ShapeTypeA, CollisionShapes::AABB> && std::is_same_v<ShapeTypeB, CollisionShapes::AABB>) {

			return AABBToAABB(shapeA, shapeB);
		} else if constexpr (std::is_same_v<ShapeTypeA, CollisionShapes::AABB> && std::is_same_v<ShapeTypeB, CollisionShapes::OBB>) {

			return AABBToOBB(shapeA, shapeB);
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

bool CollisionManager::SphereToAABB(
	const CollisionShapes::Sphere& sphere, const CollisionShapes::AABB& aabb,
	const Vector3& sphereCenter, const Vector3& aabbCenter) {

	// 上手くいっていない
	sphere;
	aabb;
	sphereCenter;
	aabbCenter;

	return false;
}

bool CollisionManager::SphereToOBB(
	const CollisionShapes::Sphere& sphere, const CollisionShapes::OBB& obb,
	const Vector3& sphereCenter, const Vector3& obbCenter) {

	// 上手くいっていない
	sphere;
	obb;
	sphereCenter;
	obbCenter;

	return false;
}

bool CollisionManager::AABBToAABB(const CollisionShapes::AABB& aabbA, const CollisionShapes::AABB& aabbB) {

	if ((aabbA.min.x <= aabbB.max.x && aabbA.max.x >= aabbB.min.x) &&
		(aabbA.min.y <= aabbB.max.y && aabbA.max.y >= aabbB.min.y) &&
		(aabbA.min.z <= aabbB.max.z && aabbA.max.z >= aabbB.min.z)) {

		return true;
	}

	return false;
}

bool CollisionManager::AABBToOBB(const CollisionShapes::AABB& aabb, const CollisionShapes::OBB& obb) {

	// 上手くいっていない
	aabb;
	obb;

	return false;
}

bool CollisionManager::OBBToOBB(const CollisionShapes::OBB& obbA, const CollisionShapes::OBB& obbB) {

	// 上手くいっていない
	obbA;
	obbB;

	return false;
}
