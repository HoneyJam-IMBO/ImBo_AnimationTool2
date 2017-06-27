#include "stdafx.h"
#include "DebugTextureMesh.h"


bool CDebugTextureMesh::CreateVertexBuffer() {
	m_nVertices = 1;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	
	return true;
}
bool CDebugTextureMesh::CreateIndexBuffer() {

	return true;
}

CDebugTextureMesh::CDebugTextureMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CDebugTextureMesh::~CDebugTextureMesh() {

}