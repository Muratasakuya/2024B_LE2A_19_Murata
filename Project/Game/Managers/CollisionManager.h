#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/3D/Base/Collider.h"

// c++
#include <list>
#include <utility>
#include <unordered_set>

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

private:
	//===================================================================*/
	//						  collision Methods
	//===================================================================*/

	bool IsColliding(Collider* colliderA, Collider* colliderB);

	// やること
	// タイプに応じて関数を分けて衝突判定を取る。

};