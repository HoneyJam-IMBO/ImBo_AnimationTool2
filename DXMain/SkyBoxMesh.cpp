#include "stdafx.h"
#include "SkyBoxMesh.h"

#define WIDTH 2048
#define HEIGHT 2048
#define DEPTH 2048


void CSkyBoxMesh::RenderExcute(UINT nInstance) {
	//m_pd3dDeviceContext->DrawIndexedInstanced(m_nIndices, nInstance, m_nStartIndexLocation, 0, m_nStartInstanceLocation);
	for (int i = 0; i < 6; i++)
	{

		m_pd3dDeviceContext->Draw(4, i * 4);
		//m_pd3dDeviceContext->DrawIndexedInstanced(m_nIndices, nInstance, m_nStartIndexLocation, i * 4, m_nStartInstanceLocation);
		//m_pd3dDeviceContext->DrawIndexed(4, 0, i * 4);
	}
}

bool CSkyBoxMesh::CreateVertexBuffer() {
	m_nVertices = 24;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	m_pVertices = new XMFLOAT3[m_nVertices];
	XMFLOAT2 *pxmf2TexCoords = new XMFLOAT2[m_nVertices];

	int i = 0;
	float fx = WIDTH;
	//// Front Quad (remember all quads point inward)
	//m_pVertices[i] = XMFLOAT3(-fx, +fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	//m_pVertices[i] = XMFLOAT3(+fx, +fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	//m_pVertices[i] = XMFLOAT3(+fx, -fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	//m_pVertices[i] = XMFLOAT3(-fx, -fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	//// Back Quad																
	//m_pVertices[i] = XMFLOAT3(+fx, +fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	//m_pVertices[i] = XMFLOAT3(-fx, +fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	//m_pVertices[i] = XMFLOAT3(-fx, -fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	//m_pVertices[i] = XMFLOAT3(+fx, -fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	//// Left Quad																
	//m_pVertices[i] = XMFLOAT3(-fx, +fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	//m_pVertices[i] = XMFLOAT3(-fx, +fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	//m_pVertices[i] = XMFLOAT3(-fx, -fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	//m_pVertices[i] = XMFLOAT3(-fx, -fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	//// Right Quad																
	//m_pVertices[i] = XMFLOAT3(+fx, +fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	//m_pVertices[i] = XMFLOAT3(+fx, +fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	//m_pVertices[i] = XMFLOAT3(+fx, -fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	//m_pVertices[i] = XMFLOAT3(+fx, -fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	//// Top Quad																	
	//m_pVertices[i] = XMFLOAT3(-fx, +fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	//m_pVertices[i] = XMFLOAT3(+fx, +fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	//m_pVertices[i] = XMFLOAT3(+fx, +fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	//m_pVertices[i] = XMFLOAT3(-fx, +fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	//// Bottom Quad																
	//m_pVertices[i] = XMFLOAT3(-fx, -fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	//m_pVertices[i] = XMFLOAT3(+fx, -fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	//m_pVertices[i] = XMFLOAT3(+fx, -fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	//m_pVertices[i] = XMFLOAT3(-fx, -fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	
	// Front Quad (remember all quads point inward)
	m_pVertices[i] = XMFLOAT3(-fx, +fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(-fx, -fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(+fx, -fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	// Back Quad																
	m_pVertices[i] = XMFLOAT3(+fx, +fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(-fx, +fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, -fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(-fx, -fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	// Left Quad																
	m_pVertices[i] = XMFLOAT3(-fx, +fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(-fx, +fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(-fx, -fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(-fx, -fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	// Right Quad																
	m_pVertices[i] = XMFLOAT3(+fx, +fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, -fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(+fx, -fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	// Top Quad																	
	m_pVertices[i] = XMFLOAT3(-fx, +fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(-fx, +fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	// Bottom Quad																
	m_pVertices[i] = XMFLOAT3(-fx, -fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, -fx, +fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(-fx, -fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(+fx, -fx, -fx); pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	m_pd3dPositionBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pVertices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dUVBuffer = CreateBuffer(sizeof(XMFLOAT2), m_nVertices, pxmf2TexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	delete[] pxmf2TexCoords;

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dUVBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	//create space mesh aabb
	BoundingBox boundingBox;
	BoundingBox::CreateFromPoints(boundingBox, XMVectorSet(-fx, -fx, -fx, 0.f), XMVectorSet(fx, fx, fx, 0.f));
	m_pAABB->SetBoundingBoxInfo(boundingBox);
	

	if (m_ppd3dVertexBuffers)	return true;

	return false;
}
bool CSkyBoxMesh::CreateIndexBuffer() {
	return true;


	//m_nIndices = 4;
	//m_pnIndices = new UINT[m_nIndices];
	//
	//m_pnIndices[0] = 0;
	//m_pnIndices[1] = 1;
	//m_pnIndices[2] = 3;
	//m_pnIndices[3] = 2;
	//
	//m_pd3dIndexBuffer = CreateBuffer(sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	//
	//if (m_pd3dIndexBuffer) return true;
	//
	//return false;
}
CSkyBoxMesh::CSkyBoxMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CSkyBoxMesh::~CSkyBoxMesh() {

}