#include "stdafx.h"
#include "CapsuleLightMesh.h"

#define WIDTH 5
#define HEIGHT 5
#define DEPTH 5

bool CCapsuleLightMesh::CreateVertexBuffer() {
	m_nVertices = 2;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;

	float fx = WIDTH*0.5f, fy = HEIGHT*0.5f, fz = DEPTH*0.5f;

	//create space mesh aabb
	BoundingBox boundingBox;
	BoundingBox::CreateFromPoints(boundingBox, XMVectorSet(-fx, -fy, -fz, 0.f), XMVectorSet(fx, fy, fz, 0.f));
	m_pAABB->SetBoundingBoxInfo(boundingBox);


	m_ppd3dVertexBuffers = nullptr;

	return true;
}
bool CCapsuleLightMesh::CreateIndexBuffer() {

	return true;
}

CCapsuleLightMesh::CCapsuleLightMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CCapsuleLightMesh::~CCapsuleLightMesh() {

}