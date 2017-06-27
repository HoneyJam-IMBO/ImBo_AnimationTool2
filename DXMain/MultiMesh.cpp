#include "stdafx.h"
#include "MultiMesh.h"


bool CMultiMesh::Begin(string path){
	//mesh의 정점 인덱스, 노말 데이터 추출
	FBXIMPORTER->Begin(path);
	
	m_nMeshCnt = FBXIMPORTER->GetMeshCnt();
	UINT nVertices{ 0 };
	CMesh* pMesh = new CMesh(m_pd3dDevice, m_pd3dDeviceContext);

	XMFLOAT3* pVertices = new XMFLOAT3[nVertices];
	XMFLOAT3* pNormals = new XMFLOAT3[nVertices];
	XMFLOAT2* pUVs = new XMFLOAT2[nVertices];

	//1. 전체 정점을 구한다.
	for (int i = START_MESH_INDEX; i < m_nMeshCnt; ++i) {
		nVertices += FBXIMPORTER->GetMeshDatas()[i].GetVertexCnt();
	}
	//저장할 정점 인덱스
	int nVertex{ 0 };
	//모든 mesh에 대해서
	for (int i = START_MESH_INDEX; i < m_nMeshCnt; ++i) {
		//mesh의 모든 정점에 대해서
		for (UINT j = 0; j < FBXIMPORTER->GetMeshDatas()[i].GetVertexCnt(); ++j) {
			//정점 정보 얻어온다.
			pVertices[nVertex] = FBXIMPORTER->GetMeshDatas()[i].GetVertexDatas()[j].GetPosition();
			pNormals[nVertex] = FBXIMPORTER->GetMeshDatas()[i].GetVertexDatas()[j].GetNormal();
			pUVs[nVertex++] = FBXIMPORTER->GetMeshDatas()[i].GetVertexDatas()[j].GetUV();
		}
	}
	//set Mesh Data
	pMesh->SetnVertices(nVertices);
	pMesh->SetpVertices(pVertices);

	//create space mesh aabb
	BoundingBox boundingBox;
	BoundingBox::CreateFromPoints(boundingBox, (size_t)nVertices, pVertices, (size_t)sizeof(XMFLOAT3));
	m_AABB.SetBoundingBoxInfo(boundingBox);



	m_pd3dPositionBuffer = CreateBuffer(sizeof(XMFLOAT3), nVertices, m_pVertices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(sizeof(XMFLOAT3), nVertices, pNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dUVBuffer = CreateBuffer(sizeof(XMFLOAT2), nVertices, pUVs, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	//-----------------------------------버퍼 조립-----------------------------------
	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dUVBuffer };
	UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[3] = { 0, 0, 0 };
	pMesh->AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	//-----------------------------------버퍼 조립-----------------------------------

	//index buffer
	//중요중요 offset을 잘 설정 해야한다. 
	//처음 mesh가 인덱스로 되어있다면 모든 mesh가 index로 되어있다고 가정한다.
	UINT nIndices{ 0 };
	UINT* pnIndices{ nullptr };

	if (!FBXIMPORTER->GetMeshDatas()[0].GetByControlPoint()) {
		//1. 모든 인덱스 정보 얻어옴
		for (int i = START_MESH_INDEX; i < m_nMeshCnt; ++i) {
			nIndices += FBXIMPORTER->GetMeshDatas()[i].GetIndexCnt();
		}
		//2. 할당
		pnIndices = new UINT[nIndices];

		//3. offset을 더해가며 index데이터 완성
		//저장할 인덱스 
		int nIndex{ 0 };
		int offset{ 0 };
		for (int i = START_MESH_INDEX; i < m_nMeshCnt; ++i) {
			if (START_MESH_INDEX != i) offset += FBXIMPORTER->GetMeshDatas()[i - 1].GetVertexCnt();
			for (UINT j = 0; j < FBXIMPORTER->GetMeshDatas()[i].GetIndexCnt(); ++j) {
				//mesh의 index가 0이 아닌 경우 offset을 더해간다.
				//offset을 더한 index를 저장한다. offest은 0번mesh는 0/ 1번 mesh는 0번 mesh의 정점 수만큼 offset을 가진다.
				pnIndices[nIndex++] = FBXIMPORTER->GetMeshDatas()[i].GetIndexs()[j] + offset;
			}
		}

		m_pd3dIndexBuffer = CreateBuffer(sizeof(UINT), nIndices, pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		pMesh->Setpd3dIndexBuffer(m_pd3dIndexBuffer);
		pMesh->SetnIndices(nIndices);
		pMesh->SetpnIndices(pnIndices);
	}//index buffer

	//create bone index buffer data
	XMFLOAT4* pxmf4BoneIndex = new XMFLOAT4[nVertices];
	XMFLOAT3* pxmf3Weight = new XMFLOAT3[nVertices];

	//모든 mesh에 대해서
	int nVertexIndex{ 0 };
	for (int i = START_MESH_INDEX; i < m_nMeshCnt; ++i) {
		//mesh의 모든 정점에 대해서
		for (UINT j = 0; j < FBXIMPORTER->GetMeshDatas()[i].GetVertexCnt(); ++j) {

			pxmf4BoneIndex[nVertexIndex].x = static_cast<float>(FBXIMPORTER->GetMeshDatas()[i].GetVertexDatas()[j].GetBlendWeightPairs()[0].GetIndex());
			pxmf4BoneIndex[nVertexIndex].y = static_cast<float>(FBXIMPORTER->GetMeshDatas()[i].GetVertexDatas()[j].GetBlendWeightPairs()[1].GetIndex());
			pxmf4BoneIndex[nVertexIndex].z = static_cast<float>(FBXIMPORTER->GetMeshDatas()[i].GetVertexDatas()[j].GetBlendWeightPairs()[2].GetIndex());
			pxmf4BoneIndex[nVertexIndex].w = static_cast<float>(FBXIMPORTER->GetMeshDatas()[i].GetVertexDatas()[j].GetBlendWeightPairs()[3].GetIndex());

			pxmf3Weight[nVertexIndex].x = static_cast<float>(FBXIMPORTER->GetMeshDatas()[i].GetVertexDatas()[j].GetBlendWeightPairs()[0].GetWeight());
			pxmf3Weight[nVertexIndex].y = static_cast<float>(FBXIMPORTER->GetMeshDatas()[i].GetVertexDatas()[j].GetBlendWeightPairs()[1].GetWeight());
			pxmf3Weight[nVertexIndex++].z = static_cast<float>(FBXIMPORTER->GetMeshDatas()[i].GetVertexDatas()[j].GetBlendWeightPairs()[2].GetWeight());
		}
	}
	//create weight buffer data

	//create buffer
	m_pd3dBoneIndexBuffer = CreateBuffer(sizeof(XMFLOAT4), nVertices, pxmf4BoneIndex, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dWeightBuffer = CreateBuffer(sizeof(XMFLOAT3), nVertices, pxmf3Weight, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	//create buffer

	//-----------------------------------버퍼 조립-----------------------------------
	pd3dBuffers[0] = m_pd3dBoneIndexBuffer;
	pd3dBuffers[1] = m_pd3dWeightBuffer;
	pnBufferStrides[0] = sizeof(XMFLOAT4);
	pnBufferStrides[1] = sizeof(XMFLOAT3);
	pnBufferOffsets[0] = 0;
	pnBufferOffsets[1] = 0;
	pMesh->AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	//-----------------------------------버퍼 조립-----------------------------------

	//bone data set
	int jointIndex = 0;

	//int size = FBXIMPORTER->GetTotalJointCnt();
	XMMATRIX* pAnimationData = new XMMATRIX[size];
	for (UINT i = 0; i < FBXIMPORTER->GetMeshCnt(); ++i) {
		for (UINT j = 0; j < FBXIMPORTER->GetAnimationDatas()[i].GetJointCnt(); ++j) {
			XMMATRIX BoneTransform;
			if (FBXIMPORTER->GetAnimationDatas()[i].GetJointDatas()[j].GetKeyFrames().empty())
				BoneTransform = XMMatrixIdentity();
			else BoneTransform = FBXIMPORTER->GetAnimationDatas()[i].GetJointDatas()[j].GetKeyFrames()[0].GetKeyFrameTransformMtx();

			BoneTransform *= FBXIMPORTER->GetAnimationDatas()[i].GetJointDatas()[j].GetOffsetMtx();

			pAnimationData[jointIndex++] = XMMatrixTranspose(BoneTransform);//그럼 최종 행렬 = A*B*C*D
		}
	}
	m_pAnimBuffer = new CStaticBuffer(m_pd3dDevice, m_pd3dDeviceContext);
	m_pAnimBuffer->Begin(1, size * sizeof(XMMATRIX), pAnimationData, 10, BIND_VS);

	delete[] pAnimationData;
	//bone data set

	return true;
}
bool CMultiMesh::End() {
	for (auto pMesh : m_vpMesh) {
		pMesh->End();
	}
	return true;
}
bool CMultiMesh::CreateVertexBuffer() {
	
	if (m_ppd3dVertexBuffers)	return true;

	return false;
}

bool CMultiMesh::CreateIndexBuffer() {
	
	return true;

}


void CMultiMesh::SetShaderState(){
	//update animation data
	void* pData = m_pAnimBuffer->Map();
	XMMATRIX* pAnimationData = static_cast<XMMATRIX*>(pData);

	if (m_nTime++ > 30) {
		m_nTime = 0;
		m_nFrame++;
		m_nFrame = (m_nFrame) % FBXIMPORTER->GetAnimStackData().GetAnimationLength();
	}
	int jointIndex = 0;
	for (UINT i = 0; i < FBXIMPORTER->GetMeshCnt(); ++i) {
		for (UINT j = 0; j < FBXIMPORTER->GetAnimationDatas()[i].GetJointCnt(); ++j) {
			XMMATRIX BoneTransform;
			if (FBXIMPORTER->GetAnimationDatas()[i].GetJointDatas()[j].GetKeyFrames().empty()) BoneTransform = XMMatrixIdentity();
			else BoneTransform = FBXIMPORTER->GetAnimationDatas()[i].GetJointDatas()[j].GetKeyFrames()[m_nFrame].GetKeyFrameTransformMtx();

			BoneTransform *= FBXIMPORTER->GetAnimationDatas()[i].GetJointDatas()[j].GetOffsetMtx();

			pAnimationData[jointIndex++] = XMMatrixTranspose(BoneTransform);
		}
	}

	m_pAnimBuffer->SetShaderState();

}

void CMultiMesh::CleanShaderState(){
	m_pAnimBuffer->CleanShaderState();
}

void CMultiMesh::RenderExcute(UINT nInstance){
//	UINT nMeshIndex{ 0 };
	for (auto pMesh : m_vpMesh) {
		//하나의 메쉬를 그린다.
		pMesh->SetShaderState();
		pMesh->RenderExcute(nInstance);
		pMesh->CleanShaderState();
	}
}


CMultiMesh::CMultiMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext){

}

CMultiMesh::~CMultiMesh(){

}
