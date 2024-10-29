#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/ComPtr.h"
#include "Lib/Math/Vector2.h"
#include "Lib/Math/Vector3.h"
#include "Lib/Math/Vector4.h"
#include "Lib/Math/Matrix3x3.h"
#include "Lib/Math/Matrix4x4.h"
#include "Lib/Math/Quaternion.h"

// directX
#include <d3d12.h>

// c++
#include <vector>
#include <memory>
#include <list>
#include <array>
#include <map>
#include <numbers>
#include <string>
#include <optional>
#include <utility>
#include <span>
#include <variant>
#include <cassert>

/*==========================================================*/
/// Collision

namespace CollisionShapes {

	struct Sphere {

		float radius;

		static Sphere Default() {
			Sphere sphere = {
				.radius = 1.0f
			};
			return sphere;
		};
	};

	struct AABB {

		Vector3 min;
		Vector3 max;

		static AABB Default() {
			AABB aabb = {
				.min = {-1.0f,-1.0f,-1.0f},
				.max = {1.0f,1.0f,1.0f}
			};
			return aabb;
		};
	};

	struct OBB {

		Vector3 size;

		static OBB Default() {
			OBB obb = {
				.size = {1.0f,1.0f,1.0f}
			};
			return obb;
		};
	};

	using Shapes = std::variant<Sphere, AABB, OBB>;

};

enum class ShapeType {

	Type_Sphere,
	Type_AABB,
	Type_OBB
};

/*==========================================================*/
/// Transform
struct Transform3D {

	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};
struct QuaternionTransform3D {

	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};
struct Transform2D {

	Vector2 pos;            // 座標
	float rotate;           // 回転
	Vector2 size;           // 大きさ
	Vector2 anchorPoint;    // アンカーポイント /* {0.5f,0.5f}で真ん中 */
	Vector2 textureLeftTop; // テクスチャ左上座標
	Vector2 textureSize;    // テクスチャ切り出しサイズ
	bool isFlipX;           // 左右反転
	bool isFlipY;           // 上下反転

	// DefaultInitialize
	void Init() {

		pos = { 0.0f,0.0f };
		rotate = 0.0f;
		size = { 0.0f,0.0f };
		anchorPoint = { 0.5f,0.5f };
		textureLeftTop = { 0.0f,0.0f };
		textureSize = { 0.0f,0.0f };
		isFlipX = false;
		isFlipY = false;
	}
};
/*==========================================================*/
/// 頂点データ
struct VertexData3D {

	Vector4 pos;
	Vector2 texcoord;
	Vector3 normal;
};
struct VertexData2D {

	Vector2 pos;
	Vector2 texcoord;
};
/*==========================================================*/
/// マテリアル
struct Material3D {

	Vector4 color;
	int32_t enableLighting;
	int32_t enableHalfLambert;
	int32_t enablePhongReflection;
	int32_t enableBlinnPhongReflection;
	float phongRefShininess;
	Vector3 specularColor;
	Matrix4x4 uvTransform;

	// DefaultInitialize
	void Init() {

		color.SetInit(1.0f);
		enableLighting = false;
		enableHalfLambert = false;
		enablePhongReflection = false;
		enableBlinnPhongReflection = false;
		phongRefShininess = 1.0f;
		specularColor.SetInit(1.0f);
		uvTransform = Matrix4x4::MakeIdentity4x4();
	}
};
struct Material2D {

	Vector4 color;
	Matrix4x4 uvTransform;

	// DefaultInitialize
	void Init() {

		color.SetInit(1.0f);
		uvTransform = Matrix4x4::MakeIdentity4x4();
	}
};
struct DepthMaterial {

	Matrix4x4 projectionInverse;
};
struct DissolveMaterial {

	float threshold;
	float edgeSize;
	Vector3 edgeColor;
	float padding[2];
};
/*==========================================================*/
/// PunctualLight
struct DirectionalLight {

	Vector4 color;
	Vector3 direction;
	float intensity;

	// DefaultInitialize
	void Init() {

		color = { 1.0f,1.0f,1.0f,1.0f };
		direction = { 0.0f,-1.0f,0.0f };
		intensity = 1.0f;
	};
};
struct PointLight {

	Vector4 color;
	Vector3 pos;
	float intensity;
	float radius;
	float decay;
	float padding[2];

	// DefaultInitialize
	void Init() {

		color = { 1.0f,1.0f,1.0f,1.0f };
		pos = { 0.0f,0.0f,0.0f };
		radius = 5.0f;
		intensity = 1.0f;
		decay = 1.0f;
	};
};
struct SpotLight {

	Vector4 color;
	Vector3 pos;
	float intensity;
	Vector3 direction;
	float distance;
	float decay;
	float cosAngle;
	float cosFalloffStart;
	float padding[2];

	// DefaultInitialize
	void Init() {

		color = { 1.0f,1.0f,1.0f,1.0f };
		pos = { 0.0f,0.0f,0.0f };
		distance = 10.0f;
		intensity = 1.0f;
		direction = { -1.0f,-1.0f,0.0f };
		decay = 1.0f;
		cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
		cosFalloffStart = 1.0f;
	};
};
struct PunctualLight {

	DirectionalLight directional;
	PointLight point;
	SpotLight spot;

	// DefaultInitialize
	void Init() {

		directional.Init();
		point.Init();
		spot.Init();
	};
};
/*==========================================================*/
/// 座標変換データ
struct TransformationMatrix {

	Matrix4x4 World;
	Matrix4x4 WVP;
	Matrix4x4 WorldInverseTranspose;
};
struct SpriteTransformationMatrix {

	Matrix4x4 WVP;
};
/*==========================================================*/
/// パーティクル
struct ParticleForGPU {

	Matrix4x4 World;
	Matrix4x4 WVP;
	Vector4 color;
};
struct ParticleData {

	Transform3D transform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	std::optional<float> easedT_;
	float currentTime;
};
struct Emitter {

	Transform3D transform; // SRT
	Vector3 prePos_;       // 前座標
	Vector4 color;         // 色
	uint32_t count;        // 個数
	float frequency;       // ~秒置き、発生頻度
	float frequencyTime;   // 発生頻度用の時刻
};
struct AccelerationField {

	Vector3 acceleration;
	CollisionShapes::AABB area;
};
/*==========================================================*/
/// カメラ座標
struct CameraForGPU {

	Vector3 worldPosition;
};
/*==========================================================*/
/// 波の情報
struct WaveForGPU {

	float time;       // 時間変数
	float amplitude;  // 波の振幅
	float frequency;  // 波の周波数
	float speed;      // 波の速度
	float wavelength; // 波長
};
/*==========================================================*/
/// モデル
struct MaterialData {

	std::optional<std::string> textureName;
};
struct Node {

	QuaternionTransform3D transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};
struct MeshModelData {

	std::vector<VertexData3D> vertices;
	std::vector<uint32_t> indices;
	MaterialData material;
};
struct VertexWeightData {

	float weight;
	uint32_t vertexIndex;
};
struct JointWeightData {

	Matrix4x4 inverseBindPoseMatrix;
	std::vector<VertexWeightData> vertexWeights;
};
struct ModelData {

	std::vector<MeshModelData> meshes;
	std::map<std::string, JointWeightData> skinClusterData;
	Node rootNode;
	CollisionShapes::AABB  aabb;
};
/*==========================================================*/
/// アニメーション
template <typename tValue>
struct Keyframe {

	float time;
	tValue value;
};
using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuaternion = Keyframe<Quaternion>;

template <typename tValue>
struct AnimationCurve {

	std::vector<Keyframe<tValue>> keyframes;
};
struct NodeAnimation {

	AnimationCurve<Vector3> scale;
	AnimationCurve<Quaternion> rotate;
	AnimationCurve<Vector3> translate;
};

struct AnimationData {

	float duration;                                      // アニメーション全体の尺
	std::map<std::string, NodeAnimation> nodeAnimations; // NodeAnimationの集合、Node名で引けるようにしておく
};
struct Joint {

	QuaternionTransform3D transform;
	Matrix4x4 localMatrix;
	Matrix4x4 skeletonSpaceMatrix;   // skeletonSpaceでの変換行列
	std::string name;
	std::vector<int32_t> children;   // 子JointのIndexのリスト。いなければ空
	int32_t index;                   // 自身のIndex
	std::optional<int32_t> parent;   // 親JointのIndex。いなければnull
};
struct Skeleton {

	int32_t root;                            // RootJointのIndex
	std::map<std::string, int32_t> jointMap; // joint名とIndexの辞書
	std::vector<Joint> joints;               // 所属しているジョイント
	std::string name;
};

// サイダイ4Jointの影響を受ける
const uint32_t kNumMaxInfluence = 4;
struct VertexInfluence {

	std::array<float, kNumMaxInfluence> weights;
	std::array<int32_t, kNumMaxInfluence> jointIndices;
};
struct WellForGPU {

	Matrix4x4 skeletonSpaceMatrix;                 // 位置用
	Matrix4x4 skeletonSpaceInverseTransposeMatrix; // 法線用
};
struct SkinningInformation {

	uint32_t numVertices;
};
struct SkinCluster {

	std::vector<Matrix4x4> inverseBindPoseMatrices;

	ComPtr<ID3D12Resource> influenceResource;
	D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
	std::span<VertexInfluence> mappedInfluence;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> influenceSrvHandle;

	ComPtr<ID3D12Resource> paletteResource;
	std::span<WellForGPU> mappedPalette;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> paletteSrvHandle;
};