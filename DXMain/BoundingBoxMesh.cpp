#include "stdafx.h"
#include "BoundingBoxMesh.h"

bool CBoundingBoxMesh::CreateVertexBuffer() {
	//이놈은 정점이 없으심.
	//vertex buffer nullptr
	//index buffer nullptr

	//test
	m_nVertices = 1;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	return true;

//	return false;
}
bool CBoundingBoxMesh::CreateIndexBuffer() {

	return true;

}

CBoundingBoxMesh::CBoundingBoxMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CBoundingBoxMesh::~CBoundingBoxMesh() {

}