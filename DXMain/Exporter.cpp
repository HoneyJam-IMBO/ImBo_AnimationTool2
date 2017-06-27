#include "stdafx.h"
#include "Exporter.h"

bool CExporter::Begin(wstring outputPath){
	m_outputPath = outputPath;
	m_out = wofstream(m_outputPath, ios::out);

	m_pFilrExporter = new CTxtExporter();

	return true;
}

bool CExporter::End(){
	m_out.close();

	delete m_pFilrExporter;
	m_pFilrExporter = nullptr;

	return true;
}

void CExporter::ExportFbxObject(CTestObject * pFBXObject){
	//pFBXObject->SetPosition(0.f, 0.f, 0.f);
	//start
	WriteFBXMeshData(pFBXObject);
	
	WriteFBXAnimaterData(pFBXObject);
	
	WriteAllFBXAnimationInfo(pFBXObject);
}

void CExporter::WriteFBXMeshData(CTestObject* pFBXObject){
	//mesh cnt
	//WriteWCHAR(L"MeshCnt", 0); WriteEnter();
	bool bHasAnimation = pFBXObject->GetAnimater().get() != nullptr;
	WriteBool(bHasAnimation);
	WriteEnter();

	int size = pFBXObject->GetRenderContainer()->GetvMesh().size();
	WriteInt(size);
	WriteEnter();

	//WriteWCHAR(L"Meshinfo", 0);
	//WriteEnter();
	//mesh info
	tag t = pFBXObject->GetTag();
	WriteUINT(t);
	WriteEnter();

	for (auto pMesh : pFBXObject->GetRenderContainer()->GetvMesh()) {
		CFileBasedMesh* pFBXMesh = dynamic_cast<CFileBasedMesh*>(pMesh.get());
		//mesh texture
		int MeshTextureCnt = pFBXMesh->GetvMeshTexture().size();
		WriteInt(MeshTextureCnt); WriteEnter();
		for (auto pTexture : pFBXMesh->GetvMeshTexture()) {
			Writestring(pTexture->GetsPath()); WriteEnter();
		}
		//mesh texture

		//vertex num
		UINT vertexCnt = pFBXMesh->GetVertexCnt();
		WriteUINT(vertexCnt);
		WriteEnter();

		//position 
		//normal
		if(bHasAnimation){
			XMFLOAT3* pVertices = pFBXMesh->GetVertices();
			for (int i = 0; i < vertexCnt; ++i) {
				WriteFloat(pVertices[i].x); WriteSpace();
				WriteFloat(pVertices[i].y); WriteSpace();
				WriteFloat(pVertices[i].z); WriteSpace();
			}
			WriteEnter();
			//WriteWCHAR(L"Normal", 0); WriteEnter();
			XMFLOAT3* pNormals = pFBXMesh->GetNormals();
			for (int i = 0; i < vertexCnt; ++i) {
				WriteFloat(pNormals[i].x); WriteSpace();
				WriteFloat(pNormals[i].y); WriteSpace();
				WriteFloat(pNormals[i].z); WriteSpace();
				WriteEnter();
			}
			WriteEnter();
		}
		else {
			//animation이 없다면 vertex에 offsetMtx를 곱해서 저장한다.
			XMFLOAT3* pVertices = pFBXMesh->GetVertices();
			for (int i = 0; i < vertexCnt; ++i) {
				XMStoreFloat3(&pVertices[i], XMVector3TransformCoord(XMLoadFloat3(&pVertices[i]), pFBXObject->GetWorldMtx()));
				WriteFloat(pVertices[i].x); WriteSpace();
				WriteFloat(pVertices[i].y); WriteSpace();
				WriteFloat(pVertices[i].z); WriteSpace();
				WriteEnter();
			}
			WriteEnter();
		
			XMFLOAT3* pNormals = pFBXMesh->GetNormals();
			for (int i = 0; i < vertexCnt; ++i) {
				XMStoreFloat3(&pNormals[i], XMVector3TransformCoord(XMLoadFloat3(&pNormals[i]), pFBXObject->GetWorldMtx()));
				WriteFloat(pNormals[i].x); WriteSpace();
				WriteFloat(pNormals[i].y); WriteSpace();
				WriteFloat(pNormals[i].z); WriteSpace();
				WriteEnter();
			}
			WriteEnter();
		}

		//uv
		//WriteWCHAR(L"UV", 0); WriteEnter();
		XMFLOAT2* pUVs = pFBXMesh->GetUVs();
		for (int i = 0; i < vertexCnt; ++i) {
			WriteFloat(pUVs[i].x); WriteSpace();
			WriteFloat(pUVs[i].y); WriteSpace();
			WriteEnter();
		}
		WriteEnter();
		
		//t b
		if (bHasAnimation) {
			XMFLOAT3* pTs = pFBXMesh->GetTs();
			for (int i = 0; i < vertexCnt; ++i) {
				WriteFloat(pTs[i].x); WriteSpace();
				WriteFloat(pTs[i].y); WriteSpace();
				WriteFloat(pTs[i].z); WriteSpace();
			}
			WriteEnter();
			
			XMFLOAT3* pBs = pFBXMesh->GetBs();
			for (int i = 0; i < vertexCnt; ++i) {
				WriteFloat(pBs[i].x); WriteSpace();
				WriteFloat(pBs[i].y); WriteSpace();
				WriteFloat(pBs[i].z); WriteSpace();
				WriteEnter();
			}
			WriteEnter();
		}
		else {
			//animation이 없다면 vertex에 offsetMtx를 곱해서 저장한다.
			XMFLOAT3* pTs = pFBXMesh->GetTs();
			for (int i = 0; i < vertexCnt; ++i) {
				XMStoreFloat3(&pTs[i], XMVector3TransformCoord(XMLoadFloat3(&pTs[i]), pFBXObject->GetWorldMtx()));
				WriteFloat(pTs[i].x); WriteSpace();
				WriteFloat(pTs[i].y); WriteSpace();
				WriteFloat(pTs[i].z); WriteSpace();
				WriteEnter();
			}
			WriteEnter();
		
			XMFLOAT3* pBs = pFBXMesh->GetBs();
			for (int i = 0; i < vertexCnt; ++i) {
				XMStoreFloat3(&pBs[i], XMVector3TransformCoord(XMLoadFloat3(&pBs[i]), pFBXObject->GetWorldMtx()));
				WriteFloat(pBs[i].x); WriteSpace();
				WriteFloat(pBs[i].y); WriteSpace();
				WriteFloat(pBs[i].z); WriteSpace();
				WriteEnter();
			}
			WriteEnter();
		}
		
		//joint index/ weight
		if (bHasAnimation) {
			//weight
			//WriteWCHAR(L"Weight", 0); WriteEnter();
			XMFLOAT3* pWeights = pFBXMesh->GetWeights();
			for (int i = 0; i < vertexCnt; ++i) {
				WriteFloat(pWeights[i].x); WriteSpace();
				WriteFloat(pWeights[i].y); WriteSpace();
				WriteFloat(pWeights[i].z); WriteSpace();
			}
			//WriteEnter();

			//joint index
			//WriteWCHAR(L"JointIndex", 0); WriteEnter();
			XMFLOAT4* pJointIndices = pFBXMesh->GetJointIndices();
			for (int i = 0; i < vertexCnt; ++i) {
				WriteFloat(pJointIndices[i].x); WriteSpace();
				WriteFloat(pJointIndices[i].y); WriteSpace();
				WriteFloat(pJointIndices[i].z); WriteSpace();
				WriteFloat(pJointIndices[i].w); WriteSpace();
			}
			WriteEnter();
		}
		//index num
		//WriteWCHAR(L"IndexCnt", 0); WriteEnter();
		UINT indexCnt = pFBXMesh->GetIndexCnt();
		WriteUINT(indexCnt);
		WriteEnter();

		//index
		if (indexCnt > 0) {
			//WriteWCHAR(L"Index", 0); WriteEnter();
			UINT* pIndices = pFBXMesh->GetInices();
			for (int i = 0; i < indexCnt; ++i) {
				WriteUINT(pIndices[i]); WriteSpace();
			}
			WriteEnter();
		}

		//다른 tool에 추가되야 할 부분
		if (false == bHasAnimation) {
			if(pFBXMesh->GetMeshIndex() == 0){
				CBoundingBox* aabb = pFBXObject->GetRenderContainer()->GetMesh()->GetAABBObject();
				BoundingBox outaabb = aabb->GetAABB();
				outaabb.Transform(outaabb, pFBXObject->GetWorldMtx());
				
				XMFLOAT3 xmf3Pos = outaabb.Center;
				//XMStoreFloat3(&xmf3Pos, aabb->GetPosition());
				WriteFloat(xmf3Pos.x); WriteSpace();
				WriteFloat(xmf3Pos.y); WriteSpace();
				WriteFloat(xmf3Pos.z); WriteSpace();
				
				XMFLOAT3 xmf3Scale = outaabb.Extents;
				//XMStoreFloat3(&xmf3Scale, aabb->GetScale());
				WriteFloat(xmf3Scale.x); WriteSpace();
				WriteFloat(xmf3Scale.y); WriteSpace();
				WriteFloat(xmf3Scale.z); WriteSpace();
				WriteEnter();


				int obbCnt = pFBXObject->GetRenderContainer()->GetMesh()->GetOBBCnt();
				WriteInt(obbCnt);
				WriteEnter();

				//obb info
				//WriteWCHAR(L"ObbInfos"); WriteSpace();
				for (int i = 0; i < obbCnt; ++i) {
					CBoundingBox* obb = pFBXObject->GetRenderContainer()->GetMesh()->GetOBBObject(i);
					BoundingOrientedBox outobb = obb->GetOBB();
					//outobb.Transform(outobb, pFBXObject->GetWorldMtx());

					//obb->Rotate(pFBXObject->GetWorldMtx());


					XMFLOAT3 xmf3Pos;
					XMStoreFloat3(&xmf3Pos, obb->GetPosition());
					WriteFloat(xmf3Pos.x); WriteSpace();
					WriteFloat(xmf3Pos.y); WriteSpace();
					WriteFloat(xmf3Pos.z); WriteSpace();

					XMFLOAT3 xmf3Scale;
					XMStoreFloat3(&xmf3Scale, obb->GetScale());
					WriteFloat(xmf3Scale.x); WriteSpace();
					WriteFloat(xmf3Scale.y); WriteSpace();
					WriteFloat(xmf3Scale.z); WriteSpace();

					XMFLOAT4 xmf4Quaternion;
					XMStoreFloat4(&xmf4Quaternion, obb->GetQuaternion());
					WriteFloat(xmf4Quaternion.x); WriteSpace();
					WriteFloat(xmf4Quaternion.y); WriteSpace();
					WriteFloat(xmf4Quaternion.z); WriteSpace();
					WriteFloat(xmf4Quaternion.w); WriteSpace();
				}//end obb for 
				WriteEnter();
			}

		}
	}//for mesh end

}

void CExporter::WriteFBXAnimaterData(CTestObject * pFBXObject){
	//WriteWCHAR(L"Animater"); WriteEnter();
	bool bHasAnimation = pFBXObject->GetAnimater().get() != nullptr;
	if (false == bHasAnimation) return;

	//WriteWCHAR(L"AnimaterOffsetMtx"); WriteEnter();
	if (GLOBALVALUEMGR->GetIsGJM()) {
		WriteFloat4x4(pFBXObject->GetAnimater().get()->GetMeshOffsetMtx());
	}
	else {
		WriteXMMatrix(pFBXObject->GetWorldMtx());
	}
	WriteEnter();

	//mtx다음에 head, rhand, lhand정보가 들어간다.
	WriteInt(pFBXObject->GetAnimater().get()->GetHead()); WriteSpace();
	WriteInt(pFBXObject->GetAnimater().get()->GetRHand()); WriteSpace();
	WriteInt(pFBXObject->GetAnimater().get()->GetLHand()); WriteSpace();

	//main aabb info
	//WriteWCHAR(L"MainAABBInfo");WriteEnter();
	/*
	m_xmf3Position
	m_xmf4Scale
	*/
	XMFLOAT3 xmf3Position;
	XMStoreFloat3(&xmf3Position, pFBXObject->GetAnimater()->GetMainAABB()->GetPosition());
	WriteFloat(xmf3Position.x); WriteSpace();
	WriteFloat(xmf3Position.y); WriteSpace();
	WriteFloat(xmf3Position.z); WriteSpace();

	XMFLOAT3 xmf3Scale;
	XMStoreFloat3(&xmf3Scale, pFBXObject->GetAnimater()->GetMainAABB()->GetScale());
	WriteFloat(xmf3Scale.x); WriteSpace();
	WriteFloat(xmf3Scale.y); WriteSpace();
	WriteFloat(xmf3Scale.z); WriteSpace();
	WriteEnter();

	//joint cnt
	//WriteWCHAR(L"JointInfo"); WriteEnter();
	UINT JointCnt = pFBXObject->GetAnimater()->GetSkeletonData()->GetJointCnt();
	WriteUINT(JointCnt); WriteEnter();
	//joint info
	//joint cnt
	/*
	- string m_sName;
	- UINT m_MyIndex{ 0 };
	- UINT m_ParentIndex{ 0 };
	- XMMATRIX m_xmmtxOffset;
	*/
	for (int i = 0; i < JointCnt; ++i) {
		string name = pFBXObject->GetAnimater()->GetSkeletonData()->GetJointDatas()[i].GetJointName();
		Writestring(name); WriteSpace();
		UINT myIndex = pFBXObject->GetAnimater()->GetSkeletonData()->GetJointDatas()[i].GetMyIndex();
		WriteUINT(myIndex); WriteSpace();
		UINT paIndex = pFBXObject->GetAnimater()->GetSkeletonData()->GetJointDatas()[i].GetMyIndex();
		WriteUINT(paIndex); WriteSpace();
		XMMATRIX xmMtx = pFBXObject->GetAnimater()->GetSkeletonData()->GetJointDatas()[i].GetOffsetMtx();
		WriteXMMatrix(xmMtx);
		WriteEnter();
	}

}

void CExporter::WriteAllFBXAnimationInfo(CTestObject * pFBXObject) {
	bool bHasAnimation = pFBXObject->GetAnimater().get() != nullptr;
	if (false == bHasAnimation) return;

	//animation info cnt
	//WriteWCHAR(L"Animation Info Cnt"); WriteEnter();
	int animationCnt = pFBXObject->GetAnimater()->GetAnimationCnt();
	WriteInt(animationCnt);
	WriteEnter();

	for (int i = 0; i < animationCnt; ++i) {
		//obb cnt
		int obbCnt = pFBXObject->GetAnimater()->GetAnimationInfo(i)->GetActiveOBB().size();
		//WriteWCHAR(L"ObbCnt"); WriteSpace();
		WriteInt(obbCnt);
		WriteEnter();

		//obb info
		//WriteWCHAR(L"ObbInfos"); WriteSpace();
		for (auto obb : pFBXObject->GetAnimater()->GetAnimationInfo(i)->GetActiveOBB()) {
			float min = obb->GetMin();
			WriteFloat(min); WriteSpace();
			float max = obb->GetMax();
			WriteFloat(max); WriteSpace();

			int myJointIndex = obb->GetMyJointIndex();
			WriteInt(myJointIndex); WriteSpace();

			XMFLOAT3 xmf3Pos;
			XMStoreFloat3(&xmf3Pos, obb->GetPosition());

			WriteFloat(xmf3Pos.x); WriteSpace();
			WriteFloat(xmf3Pos.y); WriteSpace();
			WriteFloat(xmf3Pos.z); WriteSpace();

			XMFLOAT3 xmf3Scale;
			XMStoreFloat3(&xmf3Scale, obb->GetScale());
			WriteFloat(xmf3Scale.x); WriteSpace();
			WriteFloat(xmf3Scale.y); WriteSpace();
			WriteFloat(xmf3Scale.z); WriteSpace();
			WriteEnter();

			XMFLOAT4 xmf4Quaternion;
			XMStoreFloat4(&xmf4Quaternion, obb->GetQuaternion());
			WriteFloat(xmf4Quaternion.x); WriteSpace();
			WriteFloat(xmf4Quaternion.y); WriteSpace();
			WriteFloat(xmf4Quaternion.z); WriteSpace();
			WriteFloat(xmf4Quaternion.w); WriteSpace();
			WriteEnter();
		}//end obb for 

		 //animation spd
		float animationSpd = pFBXObject->GetAnimater()->GetAnimationInfo(i)->GetAnimationSpd();
		//WriteWCHAR(L"AnimationSpd"); WriteSpace();
		WriteFloat(animationSpd);
		WriteEnter();

		//vector<vector<CKeyFrame>> m_vvKeyFrame;
		int jointCnt = pFBXObject->GetAnimater()->GetAnimationInfo(i)->GetAnimationData()->GetAllKeyFrame().size();
		//WriteInt(jointCnt); WriteSpace();
		int frameCnt = pFBXObject->GetAnimater()->GetAnimationInfo(i)->GetFrameCnt();
		WriteInt(frameCnt); WriteSpace();

		for (int j = 0; j < jointCnt; ++j) {
			int curJointFrameCnt = pFBXObject->GetAnimater()->GetAnimationInfo(i)->GetAnimationData()->GetKeyFrames(j).size();
			WriteInt(curJointFrameCnt); WriteSpace();
			if (curJointFrameCnt <= 0) continue;
			for (int k = 0; k < frameCnt; ++k) {
				XMMATRIX xmMtxFrame = pFBXObject->GetAnimater()->GetAnimationInfo(i)->GetAnimationData()->GetKeyFrames(j)[k].GetKeyFrameTransformMtx();
				WriteXMMatrix(xmMtxFrame);
			}
		}//end frame mtx for
	}//end animation info for
}

void CExporter::WriteWCHAR(const WCHAR * data) {
	wstring ws{data};
	m_pFilrExporter->WriteWstring(m_out, ws);
}

void CExporter::WriteCHAR(const char * data){
	string s{ data };
	wstring ws;
	ws.assign(s.begin(), s.end());
	m_pFilrExporter->WriteWstring(m_out, ws);
}
void CExporter::WriteWstring(const wstring data) {
	m_pFilrExporter->WriteWstring(m_out, data);
}

void CExporter::Writestring(string data) {
	wstring ws;
	ws.assign(data.begin(), data.end());
	m_pFilrExporter->WriteWstring(m_out, ws);
}
void CExporter::WriteUINT(const UINT data) {
	m_pFilrExporter->WriteUINT(m_out, data);
}

void CExporter::WriteInt(const int data) {
	m_pFilrExporter->WriteInt(m_out, data);
}

void CExporter::WriteFloat(const float data) {
	m_pFilrExporter->WriteFloat(m_out, data);
}

void CExporter::WriteFloat4x4(const XMFLOAT4X4 xmf4x4){
	WriteFloat(xmf4x4._11); WriteSpace(); WriteFloat(xmf4x4._12); WriteSpace(); WriteFloat(xmf4x4._13); WriteSpace(); WriteFloat(xmf4x4._14); WriteSpace();
	WriteFloat(xmf4x4._21); WriteSpace(); WriteFloat(xmf4x4._22); WriteSpace(); WriteFloat(xmf4x4._23); WriteSpace(); WriteFloat(xmf4x4._24); WriteSpace();
	WriteFloat(xmf4x4._31); WriteSpace(); WriteFloat(xmf4x4._32); WriteSpace(); WriteFloat(xmf4x4._33); WriteSpace(); WriteFloat(xmf4x4._34); WriteSpace();
	WriteFloat(xmf4x4._41); WriteSpace(); WriteFloat(xmf4x4._42); WriteSpace(); WriteFloat(xmf4x4._43); WriteSpace(); WriteFloat(xmf4x4._44); WriteSpace();
}

void CExporter::WriteXMMatrix(const XMMATRIX xmMtx){
	XMFLOAT4X4 xmf4x4;
	XMStoreFloat4x4(&xmf4x4, xmMtx);
	WriteFloat4x4(xmf4x4);
}

void CExporter::WriteBool(const bool b){
	m_pFilrExporter->WriteBool(m_out, b);
}

void CExporter::WriteSpace() {
	m_pFilrExporter->WriteSpace(m_out);
}

void CExporter::WriteEnter() {
	m_pFilrExporter->WriteEnter(m_out);
}
