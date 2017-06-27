#include "stdafx.h"
#include "PlaneMesh.h"

/*정점의 색상을 무작위로(Random) 설정하기 위해 사용한다. 각 정점의 색상은 난수(Random Number)를 생성하여 지정한다.*/
#define RANDOM_COLOR XMFLOAT4(((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX))


#define WIDTH 1000
#define HEIGHT 1
#define DEPTH 1000

bool CPlaneMesh::CreateVertexBuffer() {

	m_nVertices = 4;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	float fx = WIDTH*0.5f, fy = HEIGHT*0.5f, fz = DEPTH*0.5f;

	m_pVertices = new XMFLOAT3[4];
	//직육면체 메쉬는 2개의 정점 버퍼(위치 벡터 버퍼와 색상 버퍼)로 구성된다.
	//직육면체 메쉬의 정점 버퍼(위치 벡터 버퍼)를 생성한다.
	m_pVertices[0] = XMFLOAT3(-fx, 0, -fz);
	m_pVertices[1] = XMFLOAT3(-fx, 0, +fz);
	m_pVertices[2] = XMFLOAT3(+fx, 0, -fz);
	m_pVertices[3] = XMFLOAT3(+fx, 0, +fz);

	int i = 0;
	XMFLOAT2 pxmf2TexCoords[4];
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);

	//pxmf3Positions[4] = XMFLOAT3(-fx, -fy, -fz);
	//pxmf3Positions[5] = XMFLOAT3(+fx, -fy, -fz);
	//pxmf3Positions[6] = XMFLOAT3(+fx, -fy, +fz);
	//pxmf3Positions[7] = XMFLOAT3(-fx, -fy, +fz);

	m_pd3dPositionBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pVertices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dUVBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, pxmf2TexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);


	//직육면체 메쉬의 정점 버퍼(색상 버퍼)를 생성한다.
	XMVECTOR pxmvNormals[4];
	CalculateVertexNormal(pxmvNormals);
	XMFLOAT3 pxmf3Normals[4];
	for (int j = 0; j < 4; ++j) {
		pxmf3Normals[j] = XMFLOAT3(0, 1, 0);
	}
	m_pd3dNormalBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, pxmf3Normals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);


	//-----------------------------------버퍼 조립-----------------------------------
	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dUVBuffer };
	UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[3] = { 0, 0, 0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	//-----------------------------------버퍼 조립-----------------------------------

	//create space mesh aabb

	BoundingBox boundingBox;
	BoundingBox::CreateFromPoints(boundingBox, XMVectorSet(-fx, -fy, -fz, 0.f), XMVectorSet(fx, fy, fz, 0.f));
	m_pAABB->SetBoundingBoxInfo(boundingBox);


	if (m_ppd3dVertexBuffers)	return true;

	return false;
}
bool CPlaneMesh::CreateIndexBuffer() {

	return true;

}
CPlaneMesh::CPlaneMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CPlaneMesh::~CPlaneMesh() {

}