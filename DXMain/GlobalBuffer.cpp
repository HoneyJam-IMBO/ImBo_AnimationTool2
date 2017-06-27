#include "stdafx.h"
#include "GlobalBuffer.h"

bool CGlobalBuffer::Begin(UINT BufferStride, UINT Slot, UINT BindFlag, UINT Offset) {
	CConstantBuffer::Begin(1, BufferStride, Slot, BindFlag, Offset);


	return true;
}

bool CGlobalBuffer::End() {

	return true;
}


CGlobalBuffer::CGlobalBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CConstantBuffer(pd3dDevice, pd3dDeviceContext) {

}

CGlobalBuffer::~CGlobalBuffer()
{
}
