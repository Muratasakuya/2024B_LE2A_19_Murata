#pragma once

// c++
#include <cstdint>
#include <array>

// パイプラインの種類
enum PipelineType {

	Primitive,                 // 単色 テクスチャを使用しない
	Object2D,                  // 2Dテクスチャ
	pObject3D,                 // テクスチャ使用 Lightingの有無あり
	Object3DUnTex,             // マテリアルのみ Lightingの有無あり
	SkinningObject3D,          // スキニングした3Dモデル
	pParticle,                 // パーティクル用
	PrimitiveLine,             // 線分描画用
	OffscreenCopy,             // オフスクリーン、コピー
	OffscreenGrayscale,        // オフスクリーン、グレースケール
	OffscreenSepiaTone,        // オフスクリーン、セピア調
	OffscreenVignette,         // オフスクリーン、ヴィネット
	OffscreenSmoothing,        // オフスクリーン、平滑化
	OffscreenGaussianFilter,   // オフスクリーン、ガウシアンフィルタ
	OffscreenLuminanceOutline, // オフスクリーン、アウトライン、これはたぶん使わない
	OffscreenOutline,          // オフスクリーン、アウトライン、使うならこっち
	OffscreenRadialBlur,       // オフスクリーン、ラジアルブラー
	OffscreenDissolve,         // オフスクリーン、ディゾルブ
	WaveBort,                  // ウェーブボード
};
// ComputeShader
enum ComputePipelineType {

	SkinningCS // スキニング頂点計算
};

// ブレンドモード
enum BlendMode {

	kBlendModeNone,     // ブレンド無し
	kBlendModeNormal,   // 通常αブレンド
	kBlendModeAdd,      // 加算
	kBlendModeSubtract, // 減算
	kBlendModeMultiply, // 乗算
	kBlendModeScreen,   // スクリーン
};

// パイプラインタイプの数
static const uint32_t pipelineTypeNum = 18;
// CSパイプラインタイプの数
static const uint32_t computePipelineTypeNum = 1;

// ブレンドモードの数
static const uint32_t blendModeNum = 6;

static const std::array<PipelineType, pipelineTypeNum> pipelineTypes = {

	Primitive, Object2D, pObject3D, Object3DUnTex,SkinningObject3D, pParticle,PrimitiveLine,
	OffscreenCopy, OffscreenGrayscale, OffscreenSepiaTone, OffscreenVignette,
	OffscreenSmoothing, OffscreenGaussianFilter, OffscreenLuminanceOutline,OffscreenOutline,
	OffscreenRadialBlur, OffscreenDissolve,WaveBort
};
static const std::array<ComputePipelineType, computePipelineTypeNum > computePipelineTypes = {

	SkinningCS
};
static const std::array<BlendMode, blendModeNum> blendModeTypes = {
	kBlendModeNone ,kBlendModeNormal ,kBlendModeAdd ,kBlendModeSubtract ,kBlendModeMultiply ,kBlendModeScreen
};