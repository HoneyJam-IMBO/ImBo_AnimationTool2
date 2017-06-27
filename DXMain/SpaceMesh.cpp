#include "stdafx.h"
#include "SpaceMesh.h"

bool CSpaceMesh::CreateVertexBuffer() {
	m_nVertices = 36;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = static_cast<float>(ONESPACE_SIZE), fy = static_cast<float>(SPACE_SIZE), fz = static_cast<float>(ONESPACE_SIZE);

	m_pVertices = new XMFLOAT3[m_nVertices];
	XMFLOAT2 pxmf2TexCoords[36];
	int i = 0;

	//직육면체의 각 면(삼각형 2개)에 하나의 텍스쳐 이미지 전체가 맵핑되도록 텍스쳐 좌표를 설정한다.
	m_pVertices[i] = XMFLOAT3(0, +fy, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fy, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, 0, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(0, +fy, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, 0, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(0, 0, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(0, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fy, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(0, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fy, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(0, +fy, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(0, 0, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, 0, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(0, 0, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(0, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(0, 0, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, 0, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, 0, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(0, 0, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, 0, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(0, 0, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(0, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(0, +fy, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(0, 0, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(0, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(0, 0, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(0, 0, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(+fx, +fy, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, 0, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(+fx, +fy, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, 0, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(+fx, 0, 0);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pd3dPositionBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pVertices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dUVBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, pxmf2TexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	//법선 벡터를 생성하기 위한 다음 코드를 추가한다.
	XMVECTOR pxmvNormals[36];
	CalculateVertexNormal(pxmvNormals);
	XMFLOAT3 pxmf3Normals[36];
	for (int j = 0; j < 36; ++j) {
		XMStoreFloat3(&pxmf3Normals[j], pxmvNormals[j]);
	}
	m_pd3dNormalBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, pxmf3Normals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	//정점은 위치 벡터, 법선 벡터, 텍스쳐 좌표를 갖는다.
	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dUVBuffer };
	UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[3] = { 0, 0, 0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	//create space mesh aabb
	BoundingBox boundingBox;
	BoundingBox::CreateFromPoints(boundingBox, XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(fx, fy, fz, 0.f));
	m_pAABB->SetBoundingBoxInfo(boundingBox);

	if (m_ppd3dVertexBuffers)	return true;

	return false;
}
bool CSpaceMesh::CreateIndexBuffer() {

	return true;
}
CSpaceMesh::CSpaceMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CSpaceMesh::~CSpaceMesh() {

}