#include "stdafx.h"
#include "CoordinateSysMesh.h"

/*정점의 색상을 무작위로(Random) 설정하기 위해 사용한다. 각 정점의 색상은 난수(Random Number)를 생성하여 지정한다.*/
#define RANDOM_COLOR XMFLOAT4(((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX))


#define WIDTH 20
#define HEIGHT 20
#define DEPTH 20

bool CCoordinateSysMesh::CreateVertexBuffer() {
	m_nVertices = 6;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	float fx = WIDTH, fy = HEIGHT, fz = DEPTH;

	m_pVertices = new XMFLOAT3[m_nVertices];
	XMFLOAT2 pxmf2TexCoords[6];
	int i = 0;

	m_pVertices[i++] = XMFLOAT3(0.f, 0.f, 0.f);
	m_pVertices[i++] = XMFLOAT3(+fx, 0.f, 0.f);

	m_pVertices[i++] = XMFLOAT3(0.f, 0.f, 0.f);
	m_pVertices[i++] = XMFLOAT3(0.f, +fy, 0.f);

	m_pVertices[i++] = XMFLOAT3(0.f, 0.f, 0.f);
	m_pVertices[i++] = XMFLOAT3(0.f, 0.f, +fz);

	m_pd3dPositionBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pVertices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	//정점은 위치 벡터, 법선 벡터, 텍스쳐 좌표를 갖는다.
	ID3D11Buffer *pd3dBuffers[1] = { m_pd3dPositionBuffer };
	UINT pnBufferStrides[1] = { sizeof(XMFLOAT3) };
	UINT pnBufferOffsets[1] = { 0 };
	AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	//create space mesh aabb
	BoundingBox boundingBox;
	BoundingBox::CreateFromPoints(boundingBox, XMVectorSet(0.0f, 0.0f, 0.0f, 0.f), XMVectorSet(fx, fy, fz, 0.f));
	m_pAABB->SetBoundingBoxInfo(boundingBox);
	

	if (m_ppd3dVertexBuffers)	return true;

	return false;
}
bool CCoordinateSysMesh::CreateIndexBuffer() {

	return true;


}
CCoordinateSysMesh::CCoordinateSysMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CCoordinateSysMesh::~CCoordinateSysMesh() {

}