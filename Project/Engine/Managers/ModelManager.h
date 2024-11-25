#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/VertexResource.h"
#include "Game/3D/Model.h"
#include "Lib/Structure.h"

// assimp
#include <Externals/assimp/include/assimp/Importer.hpp>
#include <Externals/assimp/include/assimp/scene.h>
#include <Externals/assimp/include/assimp/postprocess.h>

// c++
#include <vector>
#include <memory>
#include <array>
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
	//===================================================================*/

	ModelManager() = default;
	~ModelManager() = default;

	void Init();

	void LoadModel(const std::string& directoryPath, const std::string& modelName);
	void LoadAniamation(const std::string& directoryPath, const std::string& animationName, const std::string& modelName);

	void MakeRailModel(const std::string& modelName, uint32_t id,
		const std::vector<VertexData3D>& vertexData);
	void ExportToOBJ(const std::string& modelName, const std::string& filePath);

	void SkeletonUpdate(const std::string& animationName);
	void ApplyAnimation(const std::string& animationName, float animationTime);

	void SkinClusterUpdate(const std::string& animationName);

	// Getter
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