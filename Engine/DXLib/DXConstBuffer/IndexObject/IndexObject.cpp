#include "IndexObject.h"

#include "NewMoon.h"

void IndexObject::Initialize(UINT indexNum) {

	// 定数バッファ生成
	DXConstBuffer::CreateIndexBuffer(NewMoon::GetDXDevice(), indexNum);
}

void IndexObject::Update() {

	// 定数バッファにデータを転送
	TransferVertexData(data);
}