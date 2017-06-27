#include "stdafx.h"
#include "FileBasedMesh.h"

static bool gVerbose = true;

#define _AABB 50
bool CFileBasedMesh::Begin() {
	
	////create default aabbzzz
	//if(nullptr == m_pAABB){
	//	BoundingBox boundingBox;
	//	BoundingBox::CreateFromPoints(boundingBox, XMVectorSet(+_AABB, +2*_AABB, +_AABB, 0.f), XMVectorSet(-_AABB, 0, -_AABB, 0.f));
	//	m_pAABB = new CBoundingBox;
	//	m_pAABB->SetBoundingBoxInfo(boundingBox);
	//}

	if (false == CreateVertexBuffer()) return false;

	if (false == CreateIndexBuffer()) return false;


	
	return CMesh::Begin();
}

bool CFileBasedMesh::End() {
	if(m_pNormals) delete[] m_pNormals;
	if(m_pUVs) delete[] m_pUVs;
	if (m_pTs) delete[] m_pTs;
	if (m_pBs) delete[] m_pBs;
	//animation
	if(m_pxmf4BoneIndex) delete[] m_pxmf4BoneIndex;
	if(m_pxmf3Weight) delete[] m_pxmf3Weight;

	return CMesh::End();
}

shared_ptr<CFileBasedMesh> CFileBasedMesh::CreateMesh(wstring path, ID3D11Device * pd3dDevice, ID3D11DeviceContext * pd3dDeviceContext, UINT index, bool bHasAnimation){
	wstring extention{ PathFindExtension(path.c_str()) };
	if (L".FBX" == extention || L".fbx" == extention) {
		return CFileBasedMesh::CreateMeshFromFBXFile(pd3dDevice, pd3dDeviceContext, index, bHasAnimation);
	}
	else if (L".GJM" == extention || L".gjm" == extention) {
		return CFileBasedMesh::CreateMeshFromGJMFile(pd3dDevice, pd3dDeviceContext, index, bHasAnimation);
	}
}

shared_ptr<CFileBasedMesh> CFileBasedMesh::CreateMeshFromFBXFile(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext, UINT index, bool bHasAnimation){
	shared_ptr<CFileBasedMesh> pFileBasedMesh;
	pFileBasedMesh = make_shared<CFileBasedMesh>(pd3dDevice, pd3dDeviceContext);
	//set mesh name, index
	pFileBasedMesh->SetMeshIndex(index);

	if (index == 0) {
		pFileBasedMesh->SetAABBObject(FBXIMPORTER->GetMeshDatas()[index].GetAABB());
	}
	//set vertex buffer info
	if (FBXIMPORTER->GetHasAnimation()) {
		//1. 전체 정점의 수를 구한다.
		UINT nVertices = FBXIMPORTER->GetMeshDatas()[index].GetVertexCnt();
		pFileBasedMesh->SetnVertices(nVertices);

		//texture
		int texture_index = 0;
		char name[64];
		for (auto data : FBXIMPORTER->GetMeshDatas()[index].GetTexturePaths()) {
			sprintf(name, "obj_texture%d%d", index, texture_index);
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->CreateTexture(name, data.c_str(), RESOURCEMGR->GetSampler("DEFAULT"), texture_index++, BIND_PS));
		}
		//2. 할당
		XMFLOAT3* pVertices = new XMFLOAT3[nVertices];
		XMFLOAT3* pNormals = new XMFLOAT3[nVertices];
		XMFLOAT2* pUVs = new XMFLOAT2[nVertices];
		XMFLOAT3* pTs = new XMFLOAT3[nVertices];
		XMFLOAT3* pBs = new XMFLOAT3[nVertices];
		XMFLOAT3* pxmf3Weight = new XMFLOAT3[nVertices];
		XMFLOAT4* pxmf4BoneIndex = new XMFLOAT4[nVertices];

		//3. 저장
		int nVertex{ 0 };
		//mesh의 모든 정점에 대해서
		for (UINT j = 0; j < FBXIMPORTER->GetMeshDatas()[index].GetVertexCnt(); ++j) {
			//mesh info
			pVertices[nVertex] = FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetPosition();
			pNormals[nVertex] = FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetNormal();
			pUVs[nVertex] = FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetUV();
			pTs[nVertex] = FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetT();
			pBs[nVertex] = FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetB();
			//animation info
			pxmf4BoneIndex[nVertex].x = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[0].GetIndex());
			pxmf4BoneIndex[nVertex].y = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[1].GetIndex());
			pxmf4BoneIndex[nVertex].z = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[2].GetIndex());
			pxmf4BoneIndex[nVertex].w = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[3].GetIndex());

			pxmf3Weight[nVertex].x = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[0].GetWeight());
			pxmf3Weight[nVertex].y = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[1].GetWeight());
			pxmf3Weight[nVertex++].z = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[2].GetWeight());
		}
		//4. set
		pFileBasedMesh->SetpVertices(pVertices);
		pFileBasedMesh->SetpNormals(pNormals);
		pFileBasedMesh->SetpUVs(pUVs);
		pFileBasedMesh->SetpTs(pTs);
		pFileBasedMesh->SetpBs(pBs);
		pFileBasedMesh->SetpWeights(pxmf3Weight);
		pFileBasedMesh->SetpBoneIndex(pxmf4BoneIndex);
	}
	else {
		//1. 전체 정점을 구한다.
		UINT nVertices = FBXIMPORTER->GetMeshDatas()[index].GetVertexCnt();
		pFileBasedMesh->SetnVertices(nVertices);

		//texture
		int texture_index = 0;
		char name[64];
		for (auto data : FBXIMPORTER->GetMeshDatas()[index].GetTexturePaths()) {
			sprintf(name, "obj_texture%d%d", index, texture_index);
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->CreateTexture(name, data.c_str(), RESOURCEMGR->GetSampler("DEFAULT"), texture_index++, BIND_PS));
		}

		//2. 할당
		XMFLOAT3* pVertices = new XMFLOAT3[nVertices];
		XMFLOAT3* pNormals = new XMFLOAT3[nVertices];
		XMFLOAT2* pUVs = new XMFLOAT2[nVertices];
		XMFLOAT3* pTs = new XMFLOAT3[nVertices];
		XMFLOAT3* pBs = new XMFLOAT3[nVertices];

		//3. 저장
		int nVertex{ 0 };
		//mesh의 모든 정점에 대해서
		for (UINT j = 0; j < FBXIMPORTER->GetMeshDatas()[index].GetVertexCnt(); ++j) {
			//정점 정보 얻어온다.
			pVertices[nVertex] = FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetPosition();
			pNormals[nVertex] = FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetNormal();
			pUVs[nVertex] = FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetUV();
			pTs[nVertex] = FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetT();
			pBs[nVertex++] = FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetB();

		}

		//4. set
		pFileBasedMesh->SetpVertices(pVertices);
		pFileBasedMesh->SetpNormals(pNormals);
		pFileBasedMesh->SetpUVs(pUVs);
		pFileBasedMesh->SetpTs(pTs);
		pFileBasedMesh->SetpBs(pBs);

	}


	//set index info
	//index
	//중요중요 offset을 잘 설정 해야한다. 
	//처음 mesh가 인덱스로 되어있다면 모든 mesh가 index로 되어있다고 가정한다
	if (!FBXIMPORTER->GetMeshDatas()[0].GetByControlPoint()) {
		//1. 모든 인덱스 정보 얻어옴
		UINT nIndices = FBXIMPORTER->GetMeshDatas()[index].GetIndexCnt();
		pFileBasedMesh->SetnIndices(nIndices);

		//2. 할당
		UINT* pnIndices = new UINT[nIndices];

		//3. offset을 더해가며 index데이터 완성
		//저장할 인덱스 
		//		int nIndex{ 0 };
		int offset{ 0 };
		for (UINT j = 0; j < nIndices; ++j) {
			//mesh의 index가 0이 아닌 경우 offset을 더해간다.
			//offset을 더한 index를 저장한다. offest은 0번mesh는 0/ 1번 mesh는 0번 mesh의 정점 수만큼 offset을 가진다.
			//지금은 더하지 않는다.
			pnIndices[j] = FBXIMPORTER->GetMeshDatas()[index].GetIndexs()[j] + offset;
		}

		pFileBasedMesh->SetpIndices(pnIndices);

	}
	//index

	//set mesh info
	pFileBasedMesh->Begin();
	//pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULT"));
	//pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTSPEC"));
	//pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTCP"));
	return pFileBasedMesh;

	return nullptr;
}

shared_ptr<CFileBasedMesh> CFileBasedMesh::CreateMeshFromGJMFile(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext, UINT index, bool bHasAnimation){
	/*
	test
	*/
	//WCHAR* p = IMPORTER->ReadWCHAR(0);

	shared_ptr<CFileBasedMesh> pFileBasedMesh;
	pFileBasedMesh = make_shared<CFileBasedMesh>(pd3dDevice, pd3dDeviceContext);
	//set mesh name, index
	pFileBasedMesh->SetMeshIndex(index);
	
	//set vertex buffer info
	if (bHasAnimation) {
		//mesh texture
		int MeshTextureCnt = IMPORTER->ReadInt();

		if (MeshTextureCnt <= 0) {
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULT"));
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTSPEC"));
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTCP"));
		}
		for (int i = 0; i < MeshTextureCnt; ++i) {
			//char name[64];
			//sprintf(name, "Test%d", index);
			string path;
			path = IMPORTER->Readstring();
			wstring wPath{ L"" };
			wPath.assign(path.cbegin(), path.cend());
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->CreateTexture(path.c_str(), wPath.c_str(), RESOURCEMGR->GetSampler("DEFAULT"), i));
			//이걸 file에서 읽어 들인다. 또 저장한다. 
			//pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTSPEC"));
			pFileBasedMesh->SetMeshMaterial(RESOURCEMGR->GetMaterial("DEFAULT"));
		}
		if (MeshTextureCnt == 1) {
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTSPEC"));
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTCP"));
		}
		else if (MeshTextureCnt == 2) {
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTCP"));
		}
		//mesh texture

		//1. 전체 정점의 수를 구한다.
		UINT nVertices = IMPORTER->ReadUINT();
		pFileBasedMesh->SetnVertices(nVertices);

		//2. 할당
		XMFLOAT3* pVertices = new XMFLOAT3[nVertices];
		XMFLOAT3* pNormals = new XMFLOAT3[nVertices];
		XMFLOAT2* pUVs = new XMFLOAT2[nVertices];
		XMFLOAT3* pTs = new XMFLOAT3[nVertices];
		XMFLOAT3* pBs = new XMFLOAT3[nVertices];
		XMFLOAT3* pxmf3Weight = new XMFLOAT3[nVertices];
		XMFLOAT4* pxmf4BoneIndex = new XMFLOAT4[nVertices];

		//3. 저장
		int nVertex{ 0 };
		//mesh의 모든 정점에 대해서
		for (UINT j = 0; j < nVertices; ++j) {
			pVertices[nVertex].x = IMPORTER->ReadFloat();
			pVertices[nVertex].y = IMPORTER->ReadFloat();
			pVertices[nVertex++].z = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pNormals[nVertex].x = IMPORTER->ReadFloat();
			pNormals[nVertex].y = IMPORTER->ReadFloat();
			pNormals[nVertex++].z = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pUVs[nVertex].x = IMPORTER->ReadFloat();
			pUVs[nVertex++].y = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pTs[nVertex].x = IMPORTER->ReadFloat();
			pTs[nVertex].y = IMPORTER->ReadFloat();
			pTs[nVertex++].z = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pBs[nVertex].x = IMPORTER->ReadFloat();
			pBs[nVertex].y = IMPORTER->ReadFloat();
			pBs[nVertex++].z = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pxmf3Weight[nVertex].x = IMPORTER->ReadFloat();
			pxmf3Weight[nVertex].y = IMPORTER->ReadFloat();
			pxmf3Weight[nVertex++].z = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pxmf4BoneIndex[nVertex].x = IMPORTER->ReadFloat();
			pxmf4BoneIndex[nVertex].y = IMPORTER->ReadFloat();
			pxmf4BoneIndex[nVertex].z = IMPORTER->ReadFloat();
			pxmf4BoneIndex[nVertex++].w = IMPORTER->ReadFloat();
		}
		//4. set
		pFileBasedMesh->SetpVertices(pVertices);
		pFileBasedMesh->SetpNormals(pNormals);
		pFileBasedMesh->SetpUVs(pUVs);
		pFileBasedMesh->SetpTs(pTs);
		pFileBasedMesh->SetpBs(pBs);
		pFileBasedMesh->SetpWeights(pxmf3Weight);
		pFileBasedMesh->SetpBoneIndex(pxmf4BoneIndex);
	}
	else {
		//mesh texture
		int MeshTextureCnt = IMPORTER->ReadInt();

		if (MeshTextureCnt <= 0) {
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULT"));
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTSPEC"));
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTCP"));
		}
		for (int i = 0; i < MeshTextureCnt; ++i) {
			//char name[64];
			//sprintf(name, "Test%d", index);
			string path;
			path = IMPORTER->Readstring();
			wstring wPath{ L"" };
			wPath.assign(path.cbegin(), path.cend());
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->CreateTexture(path.c_str(), wPath.c_str(), RESOURCEMGR->GetSampler("DEFAULT"), i));
		}
		if (MeshTextureCnt == 1) {
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTSPEC"));
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTCP"));
		}
		else if (MeshTextureCnt == 2) {
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTCP"));
		}
		//mesh texture
		pFileBasedMesh->SetMeshMaterial(RESOURCEMGR->GetMaterial("DEFAULT"));

		//1. 전체 정점을 구한다.
		UINT nVertices = IMPORTER->ReadUINT();
		pFileBasedMesh->SetnVertices(nVertices);

		//2. 할당
		XMFLOAT3* pVertices = new XMFLOAT3[nVertices];
		XMFLOAT3* pNormals = new XMFLOAT3[nVertices];
		XMFLOAT2* pUVs = new XMFLOAT2[nVertices];
		XMFLOAT3* pTs = new XMFLOAT3[nVertices];
		XMFLOAT3* pBs = new XMFLOAT3[nVertices];

		//3. 저장
		int nVertex{ 0 };
		//mesh의 모든 정점에 대해서
		for (UINT j = 0; j < nVertices; ++j) {
			//정점 정보 얻어온다.
			pVertices[nVertex].x = IMPORTER->ReadFloat();
			pVertices[nVertex].y = IMPORTER->ReadFloat();
			pVertices[nVertex++].z = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pNormals[nVertex].x = IMPORTER->ReadFloat();
			pNormals[nVertex].y = IMPORTER->ReadFloat();
			pNormals[nVertex++].z = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pUVs[nVertex].x = IMPORTER->ReadFloat();
			pUVs[nVertex++].y = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pTs[nVertex].x = IMPORTER->ReadFloat();
			pTs[nVertex].y = IMPORTER->ReadFloat();
			pTs[nVertex++].z = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pBs[nVertex].x = IMPORTER->ReadFloat();
			pBs[nVertex].y = IMPORTER->ReadFloat();
			pBs[nVertex++].z = IMPORTER->ReadFloat();
		}
		//4. set
		pFileBasedMesh->SetpVertices(pVertices);
		pFileBasedMesh->SetpNormals(pNormals);
		pFileBasedMesh->SetpUVs(pUVs);
		pFileBasedMesh->SetpTs(pTs);
		pFileBasedMesh->SetpBs(pBs);
	}
	//set index info
	
	//1. 모든 인덱스 정보 얻어옴
	UINT nIndices = IMPORTER->ReadUINT();
	if (nIndices > 0) {
		pFileBasedMesh->SetnIndices(nIndices);

		//2. 할당
		UINT* pnIndices = new UINT[nIndices];

		//3. 저장
		for (UINT j = 0; j < nIndices; ++j) {
			pnIndices[j] = IMPORTER->ReadUINT();
		}

		pFileBasedMesh->SetpIndices(pnIndices);
		//index
	}

	//다른 tool에 추가되야 할 부분
	if (false == bHasAnimation) {
		if(index == 0){
			XMFLOAT3 xmf3Pos;
			xmf3Pos.x = IMPORTER->ReadFloat();
			xmf3Pos.y = IMPORTER->ReadFloat();
			xmf3Pos.z = IMPORTER->ReadFloat();
			
			XMFLOAT3 xmf3Scale;
			xmf3Scale.x = IMPORTER->ReadFloat();
			xmf3Scale.y = IMPORTER->ReadFloat();
			xmf3Scale.z = IMPORTER->ReadFloat();
			CBoundingBox* aabb = new CBoundingBox;
			aabb->Begin(XMLoadFloat3(&xmf3Pos), XMVectorSet(xmf3Scale.x, xmf3Scale.y, xmf3Scale.z, 1.0f));
			pFileBasedMesh->SetAABBObject(aabb);
			int obbCnt = IMPORTER->ReadInt();
			
			//obb info
			//WriteWCHAR(L"ObbInfos"); WriteSpace();
			for (int i = 0; i < obbCnt; ++i) {
				XMFLOAT3 xmf3Pos;
				xmf3Pos.x = IMPORTER->ReadFloat();
				xmf3Pos.y = IMPORTER->ReadFloat();
				xmf3Pos.z = IMPORTER->ReadFloat();

				XMFLOAT3 xmf3Scale;
				xmf3Scale.x = IMPORTER->ReadFloat();
				xmf3Scale.y = IMPORTER->ReadFloat();
				xmf3Scale.z = IMPORTER->ReadFloat();

				XMFLOAT4 xmf4Quaternion;
				xmf4Quaternion.x = IMPORTER->ReadFloat();
				xmf4Quaternion.y = IMPORTER->ReadFloat();
				xmf4Quaternion.z = IMPORTER->ReadFloat();
				xmf4Quaternion.w = IMPORTER->ReadFloat();

				CBoundingBox* obb = new CBoundingBox;
				obb->Begin(XMLoadFloat3(&xmf3Pos), XMVectorSet(xmf3Scale.x, xmf3Scale.y, xmf3Scale.z, 1.0f), XMLoadFloat4(&xmf4Quaternion));
				obb->SetActive(true);
				pFileBasedMesh->GetvpOBB().push_back(obb);
			}//end obb for 
		}
	}
	pFileBasedMesh->Begin();
	return pFileBasedMesh;

	return nullptr;
}

shared_ptr<CFileBasedMesh> CFileBasedMesh::CreateMeshFromDotXFile(ID3D11Device * pd3dDevice, ID3D11DeviceContext * pd3dDeviceContext, UINT index, bool bHasAnimation){
	shared_ptr<CFileBasedMesh> pFileBasedMesh;
	pFileBasedMesh = make_shared<CFileBasedMesh>(pd3dDevice, pd3dDeviceContext);
	//set mesh name, index
	pFileBasedMesh->SetMeshIndex(index);

	//set vertex buffer info
	if (DOTXIMPORTER->GetHasAnimation()) {
		//1. 전체 정점의 수를 구한다.
		UINT nVertices = DOTXIMPORTER->GetMeshDatas()[index].GetVertexCnt();
		pFileBasedMesh->SetnVertices(nVertices);
		
		//texture
		int texture_index = 0;
		char name[64];
		for (auto data : DOTXIMPORTER->GetMeshDatas()[index].GetTexturePaths()) {
			sprintf(name, "obj_texture%d%d", index, texture_index);
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->CreateTexture(name, data.c_str(), RESOURCEMGR->GetSampler("DEFAULT"), texture_index++, BIND_PS));
		}

		//2. 할당
		XMFLOAT3* pVertices = new XMFLOAT3[nVertices];
		XMFLOAT3* pNormals = new XMFLOAT3[nVertices];
		XMFLOAT2* pUVs = new XMFLOAT2[nVertices];
		XMFLOAT3* pxmf3Weight = new XMFLOAT3[nVertices];
		XMFLOAT4* pxmf4BoneIndex = new XMFLOAT4[nVertices];

		//3. 저장
		int nVertex{ 0 };
		//mesh의 모든 정점에 대해서
		for (UINT j = 0; j < DOTXIMPORTER->GetMeshDatas()[index].GetVertexCnt(); ++j) {
			//mesh info
			pVertices[nVertex] = DOTXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetPosition();
			pNormals[nVertex] = DOTXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetNormal();
			pUVs[nVertex] = DOTXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetUV();
			//animation info
			pxmf4BoneIndex[nVertex].x = static_cast<float>(DOTXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[0].GetIndex());
			pxmf4BoneIndex[nVertex].y = static_cast<float>(DOTXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[1].GetIndex());
			pxmf4BoneIndex[nVertex].z = static_cast<float>(DOTXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[2].GetIndex());
			pxmf4BoneIndex[nVertex].w = static_cast<float>(DOTXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[3].GetIndex());

			pxmf3Weight[nVertex].x = static_cast<float>(DOTXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[0].GetWeight());
			pxmf3Weight[nVertex].y = static_cast<float>(DOTXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[1].GetWeight());
			pxmf3Weight[nVertex++].z = static_cast<float>(DOTXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[2].GetWeight());
		}
		//4. set
		pFileBasedMesh->SetpVertices(pVertices);
		pFileBasedMesh->SetpNormals(pNormals);
		pFileBasedMesh->SetpUVs(pUVs);
		pFileBasedMesh->SetpWeights(pxmf3Weight);
		pFileBasedMesh->SetpBoneIndex(pxmf4BoneIndex);
	}
	else {
		//1. 전체 정점을 구한다.
		UINT nVertices = DOTXIMPORTER->GetMeshDatas()[index].GetVertexCnt();
		pFileBasedMesh->SetnVertices(nVertices);

		//texture
		int texture_index = 0;
		char name[64];
		for (auto data : DOTXIMPORTER->GetMeshDatas()[index].GetTexturePaths()) {
			sprintf(name, "obj_texture%d%d", index, texture_index);
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->CreateTexture(name, data.c_str(), RESOURCEMGR->GetSampler("DEFAULT"), texture_index++, BIND_PS));
		}

		//2. 할당
		XMFLOAT3* pVertices = new XMFLOAT3[nVertices];
		XMFLOAT3* pNormals = new XMFLOAT3[nVertices];
		XMFLOAT2* pUVs = new XMFLOAT2[nVertices];

		//3. 저장
		int nVertex{ 0 };
		//mesh의 모든 정점에 대해서
		for (UINT j = 0; j < DOTXIMPORTER->GetMeshDatas()[index].GetVertexCnt(); ++j) {
			//정점 정보 얻어온다.
			pVertices[nVertex] = DOTXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetPosition();
			pNormals[nVertex] = DOTXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetNormal();
			pUVs[nVertex++] = DOTXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetUV();
		}

		//4. set
		pFileBasedMesh->SetpVertices(pVertices);
		pFileBasedMesh->SetpNormals(pNormals);
		pFileBasedMesh->SetpUVs(pUVs);
	}


	//set index info
	//index
	//중요중요 offset을 잘 설정 해야한다. 
	//처음 mesh가 인덱스로 되어있다면 모든 mesh가 index로 되어있다고 가정한다
	if (!DOTXIMPORTER->GetMeshDatas()[0].GetByControlPoint()) {
		//1. 모든 인덱스 정보 얻어옴
		UINT nIndices = DOTXIMPORTER->GetMeshDatas()[index].GetIndexCnt();
		pFileBasedMesh->SetnIndices(nIndices);

		//2. 할당
		UINT* pnIndices = new UINT[nIndices];

		//3. offset을 더해가며 index데이터 완성
		//저장할 인덱스 
		//		int nIndex{ 0 };
		int offset{ 0 };
		for (UINT j = 0; j < nIndices; ++j) {
			//mesh의 index가 0이 아닌 경우 offset을 더해간다.
			//offset을 더한 index를 저장한다. offest은 0번mesh는 0/ 1번 mesh는 0번 mesh의 정점 수만큼 offset을 가진다.
			//지금은 더하지 않는다.
			pnIndices[j] = DOTXIMPORTER->GetMeshDatas()[index].GetIndexs()[j] + offset;
		}

		pFileBasedMesh->SetpIndices(pnIndices);

	}
	//index

	//set mesh info
	pFileBasedMesh->Begin();
	//pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULT"));
	//pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTSPEC"));
	//pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTCP"));
	return pFileBasedMesh;

	return nullptr;
}

bool CFileBasedMesh::CreateVertexBuffer() {
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT nVertexBuffer{ 0 };
	ID3D11Buffer *pd3dBuffers[1];
	UINT pnBufferStrides[1];
	UINT pnBufferOffsets[1];	

	//position/ aabb
	if (m_pVertices) {
		m_pd3dPositionBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pVertices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	
		pd3dBuffers[0] = m_pd3dPositionBuffer;
		pnBufferStrides[0] = sizeof(XMFLOAT3);
		pnBufferOffsets[0] = 0;
		AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	}
	//normal
	if (m_pNormals) {
		m_pd3dNormalBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		
		pd3dBuffers[0] = m_pd3dNormalBuffer;
		pnBufferStrides[0] = sizeof(XMFLOAT3);
		pnBufferOffsets[0] = 0;
		AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	}
	//uv
	if (m_pUVs) {
		m_pd3dUVBuffer = CreateBuffer(sizeof(XMFLOAT2), m_nVertices, m_pUVs, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		
		pd3dBuffers[0] = m_pd3dUVBuffer;
		pnBufferStrides[0] = sizeof(XMFLOAT2);
		pnBufferOffsets[0] = 0;
		AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	}
	//T
	if (m_pTs) {
		m_pd3dTBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pTs, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

		pd3dBuffers[0] = m_pd3dTBuffer;
		pnBufferStrides[0] = sizeof(XMFLOAT3);
		pnBufferOffsets[0] = 0;
		AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	}
	//B
	if (m_pBs) {
		m_pd3dBBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pBs, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

		pd3dBuffers[0] = m_pd3dBBuffer;
		pnBufferStrides[0] = sizeof(XMFLOAT3);
		pnBufferOffsets[0] = 0;
		AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	}
	//weight
	if (m_pxmf3Weight) {
		m_pd3dWeightBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pxmf3Weight, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

		pd3dBuffers[0] = m_pd3dWeightBuffer;
		pnBufferStrides[0] = sizeof(XMFLOAT3);
		pnBufferOffsets[0] = 0;
		AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	}
	//bone index
	if (m_pxmf4BoneIndex) {
		m_pd3dBoneIndexBuffer = CreateBuffer(sizeof(XMFLOAT4), m_nVertices, m_pxmf4BoneIndex, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

		pd3dBuffers[0] = m_pd3dBoneIndexBuffer;
		pnBufferStrides[0] = sizeof(XMFLOAT4);
		pnBufferOffsets[0] = 0;
		AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	}

	if (m_ppd3dVertexBuffers)	return true;

	return false;
}
bool CFileBasedMesh::CreateIndexBuffer() {
	if (m_pnIndices) {
		m_pd3dIndexBuffer = CreateBuffer(sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	}

	return true;
}

void CFileBasedMesh::RenderExcute(UINT nInstance) {

	if (INPUTMGR->GetDebugMode())
		DEBUGER->AddText(15.0f, 250.0f, static_cast<float>(m_MeshIndex * 20.f + 20.f), YT_Color(255, 255, 255), L"Mesh_%d vertex num:     %d", m_MeshIndex, m_nVertices);

	//tool obb delete logic
	vector<vector<CBoundingBox*>::iterator> vDeleteBoundingBox;
	int DeleteBoxCnt{ 0 };
	vector<CBoundingBox*>::iterator iter;
	for (iter = m_vpOBB.begin(); iter != m_vpOBB.end(); ++iter) {

		if ((*iter)->GetToolActive()) {//ui 삭제 로직 
			if ((*iter)->GetActive()) {
				DEBUGER->RegistOBB((*iter)->GetOBB());
			}
		}
		else {
			vDeleteBoundingBox.push_back(iter);
		}
	}
	if (false == vDeleteBoundingBox.empty()) {
		for (auto data : vDeleteBoundingBox) {
			m_vpOBB.erase(data);
		}
		DeleteMeshOBBProc();
	}
	//tool obb delete logic

	if (m_pd3dIndexBuffer)
		m_pd3dDeviceContext->DrawIndexedInstanced(m_nIndices, nInstance, m_nStartIndexLocation, m_nBaseVertexLocation, m_nStartInstanceLocation);
	else
		m_pd3dDeviceContext->DrawInstanced(m_nVertices, nInstance, m_nStartVertexLocation, m_nStartInstanceLocation);
}

CFileBasedMesh::CFileBasedMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CFileBasedMesh::~CFileBasedMesh(){

}