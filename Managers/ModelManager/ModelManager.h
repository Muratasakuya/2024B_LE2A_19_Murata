#pragma once

// DirectX
#include "CBufferStructure.h"
#include "VertexResource.h"

// assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// 3D
#include "Model.h"

// c++
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream> 
#include <unordered_map>
#include <filesystem>
#include <cassert>

///===============================================================================
/// クラス前方宣言
class SrvManager;
///===============================================================================

/*////////////////////////////////////////////////////////////////////////////////
*							ModelManager Class
////////////////////////////////////////////////////////////////////////////////*/
class ModelManager {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*

	ModelManager() = default;
	~ModelManager() = default;

	void Initialize(SrvManager* srvManager);

	void LoadModel(const std::string& directoryPath, const std::string& modelName);
	void LoadAniamation(const std::string& directoryPath, const std::string& animationName, const std::string& modelName);

	void SkeletonUpdate(const std::string& animationName);
	void ApplyAnimation(const std::string& animationName, float animationTime);

	void SkinClusterUpdate(const std::string& animationName);

	// getter
	ModelData GetModelData(const std::string& modelName);
	AnimationData GetAnimationData(const std::string& animationName);
	Skeleton GetSkeletonData(const std::string& animationName);
	SkinCluster GetSkinClusterData(const std::string& animationName);

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	SrvManager* srvManager_;
	uint32_t srvIndex_;

	std::unordered_map<std::string, ModelData> models_;
	std::unordered_map<std::string, AnimationData> animations_;
	std::unordered_map<std::string, Skeleton> skeletons_;
	std::unordered_map<std::string, SkinCluster> skinClusters_;

private:
	//===================================================================*/
	//							private Functions
	//===================================================================*/

	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	Node ReadNode(aiNode* node);
	Skeleton CreateSkeleton(const Node& rootNode, const std::string& animationName);
	int32_t CreateJoint(const Node& node, const std::optional<int32_t> parent, std::vector<Joint>& joints);
	SkinCluster CreateSkinCluster(const std::string& modelName, const std::string& animationName);

};