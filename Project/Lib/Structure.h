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
#include "Lib/Adapter/Easing.h"

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

	struct OBB {

		Vector3 size;
		Vector3 center;
		Quaternion rotate;

		static OBB Default() {
			OBB obb = {
				.size = {1.0f,1.0f,1.0f},
				.center = {0.0f,0.0f,0.0f},
				.rotate = {0.0f,0.0f,0.0f}
			};
			return obb;
		};
	};

	using Shapes = std::variant<Sphere, OBB>;

};

enum class ShapeType {

	Type_Sphere,
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

	void SetUVTransform(
		const std::optional<Vector3>& scale = std::nullopt,
		const std::optional<Vector3>& rotate = std::nullopt,
		const std::optional<Vector3>& translate = std::nullopt) {

		uvTransform =
			Matrix4x4::MakeAffineMatrix(
				scale.value_or(Vector3(1.0f, 1.0f, 1.0f)),
				rotate.value_or(Vector3::Zero()),
				translate.value_or(Vector3::Zero()));
	};
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
enum class RandomParticleColor {

	RED,    //* 赤の色域
	GREEN,  //* 緑の色域
	BLUE,   //* 青の色域
	PURPLE, //* 紫の色域
	GRAY,   //* 灰色の色域

	//* Add
	DARKBLUE, //* 濃い青
};
struct ParticlePhysics {

	std::optional<Vector3> gravityDirection; // 重力のかかる方向
	std::optional<float> gravityStrength;    // 重力の強さ
};
struct ParticleData {

	Transform3D transform;
	Vector3 prePos;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
	std::optional<float> easedLifeRatio;
	std::optional<EasingType> easingType;

	ParticlePhysics physics;

	Matrix4x4 worldMatrix;
	Matrix4x4 wvpMatrix;
};
struct AccelerationField {

	Vector3 acceleration;
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