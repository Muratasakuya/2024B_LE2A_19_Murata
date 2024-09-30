#include "LightObject.h"

#include "NewMoon.h"

void LightObject::Initialize() {

	// default
	directional.Initialize();
	point.Initialize();
	spot.Initialize();

	// 定数バッファ生成
	DXConstBuffer::Initialize(NewMoon::GetDXDevice());
}

void LightObject::Update() {

	// 平行光源
	light.directional.color = directional.color;
	light.directional.direction = directional.direction;
	light.directional.intensity = directional.intensity;

	// ポイント
	light.point.color = point.color;
	light.point.pos = point.pos;
	light.point.radius = point.radius;
	light.point.intensity = point.intensity;
	light.point.decay = point.decay;

	// スポット
	light.spot.color = spot.color;
	light.spot.pos = spot.pos;
	light.spot.distance = spot.distance;
	light.spot.intensity = spot.intensity;
	light.spot.direction = spot.direction;
	light.spot.decay = spot.decay;
	light.spot.cosAngle = spot.cosAngle;
	light.spot.cosFalloffStart = spot.cosFalloffStart;

	// 定数バッファにデータを転送
	TransferData(light);
}