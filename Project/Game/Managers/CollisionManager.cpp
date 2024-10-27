#include "CollisionManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							CollisionManager classMethods
////////////////////////////////////////////////////////////////////////////////*/

void CollisionManager::AddCollider(Collider* collider) {

	colliders_.push_back(collider);
}

void CollisionManager::UpdateAllCollisions() {

	std::unordered_set<std::pair<Collider*, Collider*>, pair_hash> currentCollisions;

	for (auto itA = colliders_.begin(); itA != colliders_.end(); ++itA) {

		auto itB = itA;
		++itB;

		for (; itB != colliders_.end(); ++itB) {

			Collider* colliderA = *itA;
			Collider* colliderB = *itB;

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

bool CollisionManager::IsColliding(Collider* colliderA, Collider* colliderB) {


	return false;
}