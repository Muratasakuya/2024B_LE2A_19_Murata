#include "IndexObject.h"

#include "Engine/Base/NewMoon.h"

void IndexObject::Init(UINT indexNum) {

	// 定数バッファ生成
	DXConstBuffer::CreateIndexBuffer(NewMoon::GetDXDevice(), indexNum);
}

void IndexObject::Update() {

	// 定数バッファにデータを転送
	TransferVectorData(data);
}