#include "stdafx.h"
#include "PointLightMesh.h"

#define WIDTH 5
#define HEIGHT 5
#define DEPTH 5

bool CPointLightMesh::CreateVertexBuffer() {
	m_nVertices = 2;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;

	m_ppd3dVertexBuffers = nullptr;
	//derectional light mesh는 점이 0개임 hlsl코드에서 알아서 제작한다.
	
	float fx = WIDTH*0.5f, fy = HEIGHT*0.5f, fz = DEPTH*0.5f;

	//create space mesh aabb
	BoundingBox boundingBox;
	BoundingBox::CreateFromPoints(boundingBox, XMVectorSet(-fx, -fy, -fz, 0.f), XMVectorSet(fx, fy, fz, 0.f));
	m_pAABB->SetBoundingBoxInfo(boundingBox);

	return true;
}
bool CPointLightMesh::CreateIndexBuffer() {

	return true;
}
CPointLightMesh::CPointLightMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CPointLightMesh::~CPointLightMesh() {

}