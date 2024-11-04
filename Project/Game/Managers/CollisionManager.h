#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/3D/Base/Collider.h"

// c++
#include <list>
#include <utility>
#include <unordered_set>
#include <algorithm>

/*////////////////////////////////////////////////////////////////////////////////
*							CollisionManager Class
////////////////////////////////////////////////////////////////////////////////*/
class CollisionManager {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	CollisionManager() = default;
	~CollisionManager() = default;

	void AddCollider(Collider* collider);
	void RemoveCollider(Collider* collider);
	void ClearAllColliders();

	void UpdateAllCollisions();

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//* XOR *//
	struct pair_hash {
		template <class T1, class T2>
		std::size_t operator () (const std::pair<T1, T2>& pair) const {
			return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
		}
	};

	std::list<Collider*> colliders_;
	std::unordered_set<std::pair<Collider*, Collider*>, pair_hash> preCollisions_;

	//===================================================================*/
	//						  collision Methods
	//===================================================================*/

	bool IsColliding(Collider* colliderA, Collider* colliderB);

	bool SphereToSphere(const CollisionShapes::Sphere& sphereA, const CollisionShapes::Sphere& sphereB,
		const Vector3& centerA, const Vector3& centerB);

	bool SphereToOBB(const CollisionShapes::Sphere& sphere, const CollisionShapes::OBB& obb,
		const Vector3& sphereCenter);

	bool OBBToOBB(const CollisionShapes::OBB& obbA, const CollisionShapes::OBB& obbB);

};