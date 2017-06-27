#include "stdafx.h"
#include "FbxImporter.h"

bool CFbxImporter::Begin(string path) {
	m_FileName = path;
	//scene importer는 재사용 불가 
	m_pScene = FbxScene::Create(m_pManager, "tempName");
	FbxIOSettings* ios = FbxIOSettings::Create(m_pManager, IOSROOT);//2.

	m_pImporter = FbxImporter::Create(m_pManager, "");
	if (!m_pImporter->Initialize(path.c_str(), -1, ios)) {
		//DEBUGER->DebugMessageBox("Begin()", "FBX Import Error");
		return false;
	}
	else {
		//DEBUGER->DebugMessageBox("Begin()", "FBX Import Success");
	}
	m_pImporter->Import(m_pScene);
	m_pImporter->Destroy();
	m_pImporter = nullptr;


	//DirectX방식의 축으로 변환
	//FbxAxisSystem OurAxisSystem = FbxAxisSystem::DirectX;
	//
	//FbxAxisSystem SceneAxisSystem = m_pScene->GetGlobalSettings().GetAxisSystem();
	//
	//if (SceneAxisSystem != OurAxisSystem){
	//	FbxAxisSystem::DirectX.ConvertScene(m_pScene);
	//}

	FbxSystemUnit SceneSystemUnit = m_pScene->GetGlobalSettings().GetSystemUnit();
	if (SceneSystemUnit.GetScaleFactor() != 1.0) {
		FbxSystemUnit::cm.ConvertScene(m_pScene);
	}
	FbxGeometryConverter GeomConverter(m_pManager);
	GeomConverter.Triangulate(m_pScene, true);

	//SetModelScale(modelScale);
	m_AnimStackData.Begin();
	m_AnimationData.Begin();
	m_SkeletonData.Begin();

	LoadFile();

	return true;
}

bool CFbxImporter::End() {
	//control point/
	m_vControlIndex.clear();
	m_vControlPoint.clear();
	//m_vPos.clear();
	m_nSubsetMesh = 0;
	//for (auto v : m_vvBlendWeightPair) {
	//	v.clear();
	//}
	//m_vvBlendWeightPair.clear();
	//control point
	m_AnimStackData.End();
	m_AnimationData.End();
	m_SkeletonData.End();

	//m_vAnimationData.clear();
	//mesh data
	m_MeshScale = 1;
	m_vMeshData.clear();

	m_bHasAnimationData = false;
	m_iMeshCount = 0;

	if (m_pScene) {
		m_pScene->Destroy();
		//	m_pScene = nullptr;
	}
	//if (m_pManager) {
	//	m_pManager->Destroy();
	//	//m_pManager = nullptr;
	//}
	return true;
}

//UINT CFbxImporter::GetTotalJointCnt(){
//	int nJoint = 0;
//	if(false == m_bHasAnimationData)
//		return nJoint;
//	
//	for (auto AnimData : m_vAnimationData) {
//		nJoint += AnimData.GetJointCnt();
//	}
//
//	return nJoint;
//}

void CFbxImporter::LoadFile() {
	//animation name. 얻어오기. 지금은 안씀
	SetAnimationNameArray();
	//skeleton 트리 정보 얻기
	LoadSkeleton(m_pScene->GetRootNode());

	m_AnimationData.SetJointCnt(m_SkeletonData.GetJointCnt());
	//fbx 정보 읽기
	LoadNodeRecursive(m_pScene->GetRootNode());

	//aabb 제작
	//vertex 정보 모음
	int nVertex = 0;
	for (int i = 0; i < m_vMeshData.size(); ++i) {
		nVertex += m_vMeshData[i].GetVertexCnt();
	}
	XMFLOAT3* xmf3Positions = new XMFLOAT3[nVertex];
	
	int index{ 0 };
	for (int i = 0; i < m_vMeshData.size(); ++i) {
		for (int j = 0; j < m_vMeshData[i].GetVertexCnt(); ++j) {
			xmf3Positions[index++] = m_vMeshData[i].GetVertexDatas()[j].GetPosition();
		}
	}
	//create xmaabb
	BoundingBox boundingBox;
	BoundingBox::CreateFromPoints(boundingBox, nVertex, xmf3Positions, sizeof(XMFLOAT3));
	delete xmf3Positions;
	//create aabb
	CBoundingBox* pAABB = new CBoundingBox;
	pAABB->Begin(XMLoadFloat3(&boundingBox.Center), XMVectorSet(boundingBox.Extents.x, boundingBox.Extents.y, boundingBox.Extents.z, 1.0f));
	//set aabb
	m_vMeshData[0].SetAABB(pAABB);

	//WriteAllData(m_FileName);
}
void CFbxImporter::SetAnimationNameArray() {
	m_pScene->FillAnimStackNameArray(m_sAnimationNames);
}
void CFbxImporter::LoadAnimStack() {

	FbxAnimStack* currAnimStack = m_pScene->GetSrcObject<FbxAnimStack>(0);
	if (nullptr == currAnimStack) return;
	FbxString animStackName = currAnimStack->GetName();
	m_AnimStackData.SetAnimationName(animStackName.Buffer());
	FbxTakeInfo* takeInfo = m_pScene->GetTakeInfo(animStackName);
	FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
	FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
	FbxTime::EMode timeMode = m_pScene->GetGlobalSettings().GetTimeMode();

	m_AnimStackData.SetTimeMode(timeMode);
	m_AnimStackData.SetTimeStart(start);
	m_AnimStackData.SetTimeEnd(end);
	m_AnimationData.SetAnimationLength(end.GetFrameCount(timeMode) - start.GetFrameCount(timeMode) + 1);

	m_AnimStackData.SetpAnimStack(currAnimStack);

	//m_vAnimationData.push_back(animData);

	//DEBUGER->DebugGMessageBox(L"LoadAnimStack()", L"Animation start : %d \n\
					//Animation end : %d \n\
//Animation length : %d \n", 
//m_AnimStackData.GetTimeStart(), m_AnimStackData.GetTimeEnd(), m_AnimStackData.GetAnimationLength());

}

void CFbxImporter::LoadSkeleton(FbxNode* pNode) {
	for (int childIndex = 0; childIndex < pNode->GetChildCount(); ++childIndex)
		ProcessSkeletonHierarchyRecursively(pNode->GetChild(childIndex), 0, -1);

	//DEBUGER->DebugGMessageBox(L"LoadSkeleton()", L"Skeleton NUM : %d", m_vAnimationData[0].GetJointDatas().size());
}

void CFbxImporter::ProcessSkeletonHierarchyRecursively(FbxNode * inNode, int myIndex, int inParentIndex) {
	if (inNode->GetNodeAttribute()
		&& inNode->GetNodeAttribute()->GetAttributeType()
		&& inNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		CFbxJointData currJoint;
		currJoint.SetParentIndex(inParentIndex);
		currJoint.SetJointName(inNode->GetName());
		currJoint.SetMyIndex(myIndex);
		m_SkeletonData.GetJointDatas().push_back(currJoint);
	}
	for (int i = 0; i < inNode->GetChildCount(); i++) {
		ProcessSkeletonHierarchyRecursively(inNode->GetChild(i), m_SkeletonData.GetJointDatas().size(), myIndex);
	}
}

void CFbxImporter::LoadNodeRecursive(FbxNode* pNode) {
	FbxNodeAttribute* pNodeAttribute = pNode->GetNodeAttribute();

	if (pNodeAttribute) {
		if (pNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
			FbxMesh* pMesh = pNode->GetMesh();
			if (pMesh) {
				//				ExportMeshData(pMesh);
				if (false == ExportMeshData(pMesh)) {
					DEBUGER->DebugMessageBox("LoadNodeRecursive()", "FAIL");
				}
				

			}//end pMesh if
		}//end if
	}//end if

	const int childCount = pNode->GetChildCount();
	for (int i = 0; i < childCount; ++i)	//Recursive
		LoadNodeRecursive(pNode->GetChild(i));
}
bool CFbxImporter::ExportMeshData(FbxMesh* pMesh) {

	if (!pMesh->GetNode()) return false;

	//	DEBUGER->DebugMessageBox("ExportMeshData()", "Start Export Mesh Data");
	CFbxMeshData MeshData;

	//name set
	//const char* pName = pMesh->GetName();
	//	MeshData.SetMeshName(pMesh->GetName());

	FbxGeometryElementNormal* vertexNormal = nullptr;
	FbxGeometryElementUV* vertexUV = nullptr;
	FbxGeometryElementTangent* vertexTangent = nullptr;
	FbxGeometryElementBinormal* vertexBiNormal = nullptr;
	FbxGeometryElement::EMappingMode eNormalMappingMode = FbxGeometryElement::eNone;
	FbxGeometryElement::EMappingMode eUVMappingMode = FbxGeometryElement::eNone;
	FbxGeometryElement::EMappingMode eTangentMappingMode = FbxGeometryElement::eNone;
	FbxGeometryElement::EMappingMode eBiNormalMappingMode = FbxGeometryElement::eNone;


	MeshData.SetHasNormal(pMesh->GetElementNormalCount() > 0);
	MeshData.SetHasUV(pMesh->GetElementUVCount() > 0);
	MeshData.SetHasT(pMesh->GetElementTangentCount() > 0);
	MeshData.SetHasB(pMesh->GetElementBinormalCount() > 0);

	if (MeshData.GetHasNormal()) {
		vertexNormal = pMesh->GetElementNormal(0);
		eNormalMappingMode = vertexNormal->GetMappingMode();
		if (eNormalMappingMode == FbxGeometryElement::eNone) {
			MeshData.SetHasNormal(false);
		}
		if (MeshData.GetHasNormal() && eNormalMappingMode != FbxGeometryElement::eByControlPoint) {
			MeshData.SetByControlPoint(false);
		}
	}
	if (MeshData.GetHasUV()) {
		vertexUV = pMesh->GetElementUV(0);
		eUVMappingMode = vertexUV->GetMappingMode();
		if (eUVMappingMode == FbxGeometryElement::eNone) {
			MeshData.SetHasUV(false);
		}
		if (MeshData.GetHasUV() && eUVMappingMode != FbxGeometryElement::eByControlPoint) {
			MeshData.SetByControlPoint(false);
		}
	}
	if (MeshData.GetHasT()) {
		vertexTangent = pMesh->GetElementTangent(0);
		eTangentMappingMode = vertexTangent->GetMappingMode();
		if (eTangentMappingMode == FbxGeometryElement::eNone) {
			MeshData.SetHasT(false);
		}
	}
	if (MeshData.GetHasB()) {
		vertexBiNormal = pMesh->GetElementBinormal(0);
		eBiNormalMappingMode = vertexBiNormal->GetMappingMode();
		if (eBiNormalMappingMode == FbxGeometryElement::eNone) {
			MeshData.SetHasB(false);
		}
	}

	const int polygonCount = pMesh->GetPolygonCount();
	int polygonVertexCount = 0;

	polygonVertexCount = pMesh->GetPolygonVertexCount();

	if (MeshData.GetByControlPoint())
		polygonVertexCount = pMesh->GetControlPointsCount();

	//DEBUGER->DebugGMessageBox(
	//	L"Export Mesh Data", 
	//	L"Polygon Count : %d \n\
						//	Polygon Vertex Count : %d", polygonCount, polygonVertexCount);

//m_vControlPoint.resize(polygonVertexCount);
//m_vControlIndex.resize(polygonCount * 3);

//MeshData.GetVertexDatas().reserve(polygonVertexCount);
//MeshData.GetIndexs().resize(polygonCount * 3);



	if (false == MeshData.GetByControlPoint()) {
		//1. 모든 controlpoint를 얻는다.
		GetControlPointData(pMesh);

		//2. INDEX정보 set
		GetIndexData(pMesh);

		//3. NORMAL정보 set
		GetNormalData(pMesh, vertexNormal);

		//4. UV
		GetUVData(pMesh, vertexUV);

		if(MeshData.GetHasT())
			GetTData(pMesh, vertexTangent);
		if(MeshData.GetHasB())
			GetBData(pMesh, vertexBiNormal);
		//DEBUGER->DebugGMessageBox(L"ExportMesh()", L"Success\n MeshIndex : %d\n Vertex Count : %d\n Index Count : %d", m_iMeshCount, MeshData.GetVertexDatas().size(), MeshData.GetIndexCnt());

		//m_vMeshData.push_back(MeshData);	
	}


	const bool lHasSkin = pMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
	const bool lHasDeformation = lHasSkin;

	m_bHasAnimationData = lHasDeformation;

	if (m_bHasAnimationData)
	{
		if (m_iMeshCount == 0)//animation Stack 정보 얻기
			LoadAnimStack();


		if (false == (ExportAnimationData(pMesh) && ReformBlendWeightPairInfo()))
			DEBUGER->DebugGMessageBox(L"ExportAnimationData()\n ReformBlendWeightPairInfo()", L"FAIL");
	}


	RegenerateMesh(pMesh, MeshData);

	return true;
}
//
//bool CFbxImporter::ExportAnimationData(FbxMesh * pMesh) {
//	FbxMesh* currMesh = pMesh;
//	unsigned int numOfDeformers = currMesh->GetDeformerCount();
//	CFbxAnimationData AnimationData;
//	AnimationData.SetGeometryTransform(GetGeometryTransformation(pMesh));
//
//	//deformer 1개당 animation정보 1개씩
//	for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
//	{
//		FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
//		if (!currSkin) {
//			continue;
//		}
//		//cluster 즉 이 mesh에 영향을 주는 joint수
//		unsigned int numOfClusters = currSkin->GetClusterCount();
//		
//		AnimationData.GetJointDatas().resize(numOfClusters);
//
////		UINT currMeshJointIndexOffset = GetCurrMeshJointIndexOffset();
//		//for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
//		for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
//		{
//			FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
//		
//			std::string currJointName = currCluster->GetLink()->GetName();
//			AnimationData.GetJointDatas()[clusterIndex].SetJointName(currJointName);
//
//			//새로운 joint index 구하기
//			//UINT currJointIndex = clusterIndex + currMeshJointIndexOffset;
//			UINT currJointIndex = clusterIndex;
//
//			// 현재 mesh의 현재 Joint의 모든 영향을 받는 점들에 대해
//			//BlendWeightPair를 저장
//			unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
//			for (unsigned int i = 0; i < numOfIndices; ++i){
//				CFbxBlendWeightPair currBlendingIndexWeightPair{ currJointIndex , currCluster->GetControlPointWeights()[i] };
//				m_vMeshData[m_iMeshCount].GetVertexDatas()[currCluster->GetControlPointIndices()[i]].GetBlendWeightPairs().push_back(currBlendingIndexWeightPair);
//			}
//			
//			FbxAMatrix transformMatrix;
//			FbxAMatrix transformLinkMatrix;
//			FbxAMatrix globalBindposeInverseMatrix;
//
//			currCluster->GetTransformMatrix(transformMatrix);	// The transformation of the mesh at binding time
//			currCluster->GetTransformLinkMatrix(transformLinkMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space
//			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix; //* AnimationData.GetGeometryTransform();
//		
//			globalBindposeInverseMatrix.SetT(
//				FbxVector4(globalBindposeInverseMatrix.GetT().mData[0] * m_MeshScale,
//					globalBindposeInverseMatrix.GetT().mData[1] * m_MeshScale,
//					globalBindposeInverseMatrix.GetT().mData[2] * m_MeshScale));
//
//
//			// 현재 Joint의 Offset 행렬 구하기 
//			AnimationData.GetJointDatas()[clusterIndex].SetOffsetMtx(ConvertFbxMtxToXMMATRIX(globalBindposeInverseMatrix));
//
//			for (FbxLongLong i = m_AnimStackData.GetTimeStart(); i <= m_AnimStackData.GetTimeEnd(); ++i) {
//				FbxTime currTime;
//				currTime.SetFrame(i, m_AnimStackData.GetTimeMode());
//				FbxAMatrix currentTransformOffset = pMesh->GetNode()->EvaluateGlobalTransform(currTime);// *AnimationData.GetGeometryTransform();
//				FbxAMatrix mGlobalTransform = currentTransformOffset.Inverse() * currCluster->GetLink()->EvaluateGlobalTransform(currTime);
//
//				mGlobalTransform.SetT(
//					FbxVector4(mGlobalTransform.GetT().mData[0] * m_MeshScale,
//						mGlobalTransform.GetT().mData[1] * m_MeshScale,
//						mGlobalTransform.GetT().mData[2] * m_MeshScale));
//
//
//				CKeyFrame KeyFrame{ (double)i, ConvertFbxMtxToXMMATRIX(mGlobalTransform) };//이게 프레임 타임
//				
//				AnimationData.GetJointDatas()[clusterIndex].GetKeyFrames().push_back(KeyFrame);
//			}
//			
//		}
//	}
//
//	//BlendWeightPair의 수가 4보다 joint가 적으면 채워준다. 0으로
//	CFbxBlendWeightPair currBlendingIndexWeightPair{ 0,0 };
//	for (auto itr = m_vMeshData[m_iMeshCount].GetVertexDatas().begin(); itr != m_vMeshData[m_iMeshCount].GetVertexDatas().end(); ++itr){
//		for (size_t i = itr->GetBlendWeightPairs().size(); i <= 4; ++i){
//			itr->GetBlendWeightPairs().push_back(currBlendingIndexWeightPair);
//		}
//	} 
//
//	m_vAnimationData.push_back(AnimationData);
//	return true;
//}

bool CFbxImporter::ExportAnimationData(FbxMesh * pMesh) {

	FbxMesh* currMesh = pMesh;
	unsigned int numOfDeformers = currMesh->GetDeformerCount();
	FbxAMatrix curGeometryMtx = GetGeometryTransformation(pMesh);

	//deformer 1개당 animation정보 1개씩
	for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
	{
		FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!currSkin) {
			continue;
		}

		unsigned int numOfClusters = currSkin->GetClusterCount();
		//DEBUGER->DebugGMessageBox(L"ExportAnimationData()", L"numOfClusters : %d", numOfClusters);
		int test = m_SkeletonData.GetJointCnt();

		//AnimationData.GetJointDatas().resize(m_AnimStackData.GetJointCnt());
		//for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
		for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
		{
			FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
			std::string currJointName = currCluster->GetLink()->GetName();
			unsigned int currJointIndex = FindJointIndexUsingName(currJointName);

			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			currCluster->GetTransformMatrix(transformMatrix);	// The transformation of the mesh at binding time
			currCluster->GetTransformLinkMatrix(transformLinkMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * curGeometryMtx;

			// 현재 Joint의 Offset 행렬 구하기 
			m_SkeletonData.GetJointDatas()[currJointIndex].SetOffsetMtx(ConvertFbxMtxToXMMATRIX(globalBindposeInverseMatrix));

			// 현재 Joint의 모든 영향을 받는 점들에 대해
			//BlendWeightPair를 저장
			unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
			for (unsigned int i = 0; i < numOfIndices; ++i) {
				CFbxBlendWeightPair currBlendingIndexWeightPair{ currJointIndex , currCluster->GetControlPointWeights()[i] };
				m_vControlPoint[currCluster->GetControlPointIndices()[i]].
					m_vBlendWeightPair.push_back(currBlendingIndexWeightPair);
			}
			//현재 Joint의 현재 ANimation의 모든 frameMtx를 생성, 저장
			for (FbxLongLong i = m_AnimStackData.GetTimeStart(); i <= m_AnimStackData.GetTimeEnd(); ++i) {
				FbxTime currTime;
				currTime.SetFrame(i, m_AnimStackData.GetTimeMode());
				FbxAMatrix currentTransformOffset = pMesh->GetNode()->EvaluateGlobalTransform(currTime) * curGeometryMtx;
				FbxAMatrix mGlobalTransform = currentTransformOffset.Inverse() * currCluster->GetLink()->EvaluateGlobalTransform(currTime);

				CKeyFrame KeyFrame{ (double)i, ConvertFbxMtxToXMMATRIX(mGlobalTransform) };
				m_AnimationData.GetKeyFrames(currJointIndex).push_back(KeyFrame);
			}
		}
	}

	//BlendWeightPair의 수가 4보다 joint가 적으면 채워준다. 0으로
	CFbxBlendWeightPair currBlendingIndexWeightPair{ 0,0 };
	for (auto itr = m_vControlPoint.begin(); itr != m_vControlPoint.end(); ++itr) {
		//for (auto itr = m_vMeshData[0].GetVertexDatas().begin(); itr != m_vMeshData[0].GetVertexDatas().end(); ++itr){
		for (unsigned int i = itr->m_vBlendWeightPair.size(); i <= 4; ++i) {
			itr->m_vBlendWeightPair.push_back(currBlendingIndexWeightPair);
		}
	}

	//m_vAnimationData.push_back(AnimationData);
	return true;
}


bool CFbxImporter::ReformBlendWeightPairInfo() {

	for (auto& Data : m_vControlPoint) {
		int boneIndices[4] = { 0, 0, 0, 0 };
		float boneWeights[4] = { 0.0, 0.0, 0.0, 0.0 };
		if (Data.m_vBlendWeightPair.size() > 4) {
			//내림차순 
			sort(Data.m_vBlendWeightPair.begin(), Data.m_vBlendWeightPair.end(), [](CFbxBlendWeightPair& lhs, CFbxBlendWeightPair& rhs) {
				return lhs.GetWeight() > rhs.GetWeight();
			});

			//nomalize
			float sumWeight = 0.0f, interpolateWeight = 0.0f;
			for (int i = 0; i < 4; ++i) {
				sumWeight += static_cast<float>(Data.m_vBlendWeightPair[i].GetWeight());
			}

			interpolateWeight = 1.0f - sumWeight;
			//nomalize

			//3.
			auto removeIter = Data.m_vBlendWeightPair.begin() + 4;
			Data.m_vBlendWeightPair.erase(removeIter, Data.m_vBlendWeightPair.end());
			//4.
			Data.m_vBlendWeightPair[0].SetWeight(Data.m_vBlendWeightPair[0].GetWeight() + interpolateWeight);
		}
	}
	return true;
}


UINT CFbxImporter::FindJointIndexUsingName(const std::string& inJointName) {
	for (unsigned int i = 0; i < m_SkeletonData.GetJointDatas().size(); ++i) {
		if (m_SkeletonData.GetJointDatas()[i].GetJointName() == inJointName) {
			return i;
		}
	}

	throw std::exception("Skeleton information in FBX file is corrupted.");
}
//UINT CFbxImporter::GetCurrMeshJointIndexOffset(){
//	int offset = 0;
//	for (int i = 0; i < m_iMeshCount; ++i) {
//		offset += m_vAnimationData[i].GetJointCnt();
//	}
//	return offset;
//}

FbxAMatrix CFbxImporter::GetGeometryTransformation(FbxMesh* pMesh)
{
	FbxNode* pNode = pMesh->GetNode();
	if (!pNode)
	{
		throw std::exception("Null for mesh geometry");
	}

	const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}

void CFbxImporter::GetControlPointData(FbxMesh * pMesh) {
	int nVertices = pMesh->GetControlPointsCount();
	//XMFLOAT3* pVertices = new XMFLOAT3[nVertices];
	//mesh data resize

	//m_vPos.resize(nVertices);
	m_vControlPoint.resize(nVertices);
	//m_vNor.resize(nVertices);
	//m_vUV.resize(nVertices);
	
	//m_vvBlendWeightPair.resize(nVertices);
	XMFLOAT3 xmf3Pos;
	for (int j = 0; j < nVertices; ++j) {
		xmf3Pos.x = static_cast<float>(pMesh->GetControlPointAt(j).mData[0]);
		xmf3Pos.y = static_cast<float>(pMesh->GetControlPointAt(j).mData[1]);
		xmf3Pos.z = static_cast<float>(pMesh->GetControlPointAt(j).mData[2]);

		m_vControlPoint[j].m_xmf3Pos = xmf3Pos;
	}//end for
	 //delete[] pVertices;
}
void CFbxImporter::GetIndexData(FbxMesh * pMesh) {
	int nIndices = pMesh->GetPolygonVertexCount();
	m_vControlIndex.resize(nIndices);
	m_vMaterialIndex.resize(nIndices);

	UINT* pIndices = (UINT*)pMesh->GetPolygonVertices();
	for (int j = 0; j < nIndices; ++j) {
		m_vControlIndex[j] = pIndices[j];
		//m_vControlPoint[j].m_xmf3Pos = m_vPos[m_vControlIndex[j]];
	}//end for
	 //INDEX
}
void CFbxImporter::GetNormalData(FbxMesh * pMesh, FbxGeometryElementNormal* vertexNormal) {
	vector<XMFLOAT3> vNormals;
	int lIndexByPolygonVertex = 0;
	for (int lPolygonIndex = 0; lPolygonIndex < pMesh->GetPolygonCount(); lPolygonIndex++) {
		int lPolygonSize = pMesh->GetPolygonSize(lPolygonIndex);
		for (int i = 0; i < lPolygonSize; i++) {
			int lNormalIndex = 0;
			if (vertexNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
				lNormalIndex = lIndexByPolygonVertex;

			if (vertexNormal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
				lNormalIndex = vertexNormal->GetIndexArray().GetAt(lIndexByPolygonVertex);

			FbxVector4 lNormal = vertexNormal->GetDirectArray().GetAt(lNormalIndex);
			vNormals.push_back(XMFLOAT3{ static_cast<float>(lNormal[0]), static_cast<float>(lNormal[1]), static_cast<float>(lNormal[2]) });

			lIndexByPolygonVertex++;
		}//end for i //lPolygonSize
	}//end for lPolygonIndex //PolygonCount

	if (vNormals.size() == m_vControlPoint.size()) {
		for (int i = 0; i < vNormals.size(); ++i) {
			m_vControlPoint[i].m_xmf3Nor = vNormals[i];
		}
	}
	else {
		for (int i = 0; i < vNormals.size(); ++i) {
			m_vControlPoint[m_vControlIndex[i]].m_xmf3Nor = vNormals[i];
		}
	}
	vNormals.clear();
}
void CFbxImporter::GetUVData(FbxMesh * pMesh, FbxGeometryElementUV* vertexUV) {
	for (int lUVSetIndex = 0; lUVSetIndex < 1; lUVSetIndex++) {
		//uv 정보를 채울 벡터
		vector<XMFLOAT2> vUV;
		if (!vertexUV)
			continue;
		//index를 사용하는지 검사.
		// only support mapping mode eByPolygonVertex and eByControlPoint
		if (vertexUV->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
			vertexUV->GetMappingMode() != FbxGeometryElement::eByControlPoint)
			return;

		FbxStringList UVNames;
		pMesh->GetUVSetNames(UVNames);

		const bool lUseIndex = vertexUV->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int lIndexCount = (lUseIndex) ? vertexUV->GetIndexArray().GetCount() : m_vControlPoint.size();

		int maxIndex{ 0 };
		//int vertexUVIndex{ 0 };
		const int lPolyCount = pMesh->GetPolygonCount();
		int lPolyIndexCounter = 0;

		FbxLayerElementMaterial* layerMaterials = pMesh->GetLayer(0)->GetMaterials();
		vector<int> vMaterialID;

		for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex) {
			const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
			for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
			{
				FbxVector2 lUVValue;
				int lUVIndex = lUseIndex ? vertexUV->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

				if (maxIndex < lUVIndex) {
					maxIndex = lUVIndex;
				}

				lUVValue = vertexUV->GetDirectArray().GetAt(lUVIndex);
				vUV.push_back(XMFLOAT2(static_cast<float>(lUVValue.mData[0]), static_cast<float>(1 - lUVValue.mData[1])));
				if (NULL != layerMaterials) {
					int materialId = layerMaterials->GetIndexArray().GetAt(lPolyIndex);

					if (m_nSubsetMesh < materialId) {
						m_nSubsetMesh = materialId;
					}
					vMaterialID.push_back(materialId);
					//vertexInfo.materialIndex = materialId;
				}
				lPolyIndexCounter++;

			}
		}


		//uv data set
		if (vUV.size() == m_vControlPoint.size()) {
			for (int i = 0; i < vUV.size(); ++i) {
				m_vControlPoint[i].m_xmf2UV = vUV[i];
				//m_vControlPoint[i].m_nMaterialIndex = vMaterialID[i];
				m_vMaterialIndex[i] = vMaterialID[i];
			}
		}
		else {
			for (int i = 0; i < vUV.size(); ++i) {
				m_vControlPoint[m_vControlIndex[i]].m_xmf2UV = vUV[i];
				//m_vControlPoint[m_vControlIndex[i]].m_nMaterialIndex = vMaterialID[i];
				m_vMaterialIndex[i] = vMaterialID[i];
			}
		}
		vUV.clear();
	}
}
void CFbxImporter::GetTData(FbxMesh * pMesh, FbxGeometryElementTangent * vertexTangent){
	vector<XMFLOAT3> vTangents;
	int lIndexByPolygonVertex = 0;
	for (int lPolygonIndex = 0; lPolygonIndex < pMesh->GetPolygonCount(); lPolygonIndex++) {
		int lPolygonSize = pMesh->GetPolygonSize(lPolygonIndex);
		for (int i = 0; i < lPolygonSize; i++) {
			int lNormalIndex = 0;
			if (vertexTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
				lNormalIndex = lIndexByPolygonVertex;

			if (vertexTangent->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
				lNormalIndex = vertexTangent->GetIndexArray().GetAt(lIndexByPolygonVertex);

			FbxVector4 lNormal = vertexTangent->GetDirectArray().GetAt(lNormalIndex);
			vTangents.push_back(XMFLOAT3{ static_cast<float>(lNormal[0]), static_cast<float>(lNormal[1]), static_cast<float>(lNormal[2]) });

			lIndexByPolygonVertex++;
		}//end for i //lPolygonSize
	}//end for lPolygonIndex //PolygonCount

	if (vTangents.size() == m_vControlPoint.size()) {
		for (int i = 0; i < vTangents.size(); ++i) {
			if ((vTangents[i].x < 0.0001 && vTangents[i].x > -0.0001) &&
				(vTangents[i].y < 0.0001 && vTangents[i].y > -0.0001) &&
				(vTangents[i].z < 0.0001 && vTangents[i].z > -0.0001)) {
				vTangents[i].x = 1.0f;
			}
			m_vControlPoint[i].m_xmf3T = vTangents[i];
		}
	}
	else {
		for (int i = 0; i < vTangents.size(); ++i) {
			if ((vTangents[i].x < 0.0001 && vTangents[i].x > -0.0001) &&
				(vTangents[i].y < 0.0001 && vTangents[i].y > -0.0001) &&
				(vTangents[i].z < 0.0001 && vTangents[i].z > -0.0001)) {
				vTangents[i].x = 1.0f;
			}
			m_vControlPoint[m_vControlIndex[i]].m_xmf3T = vTangents[i];
		}
	}
	vTangents.clear();
}
void CFbxImporter::GetBData(FbxMesh * pMesh, FbxGeometryElementBinormal * vertexBiNormal){
	vector<XMFLOAT3> vBiNormal;
	int lIndexByPolygonVertex = 0;
	for (int lPolygonIndex = 0; lPolygonIndex < pMesh->GetPolygonCount(); lPolygonIndex++) {
		int lPolygonSize = pMesh->GetPolygonSize(lPolygonIndex);
		for (int i = 0; i < lPolygonSize; i++) {
			int lNormalIndex = 0;
			if (vertexBiNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
				lNormalIndex = lIndexByPolygonVertex;

			if (vertexBiNormal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
				lNormalIndex = vertexBiNormal->GetIndexArray().GetAt(lIndexByPolygonVertex);

			FbxVector4 lNormal = vertexBiNormal->GetDirectArray().GetAt(lNormalIndex);
			vBiNormal.push_back(XMFLOAT3{ static_cast<float>(lNormal[0]), static_cast<float>(lNormal[1]), static_cast<float>(lNormal[2]) });

			lIndexByPolygonVertex++;
		}//end for i //lPolygonSize
	}//end for lPolygonIndex //PolygonCount

	if (vBiNormal.size() == m_vControlPoint.size()) {
		for (int i = 0; i < vBiNormal.size(); ++i) {
			if ((vBiNormal[i].x < 0.0001 && vBiNormal[i].x > -0.0001) &&
				(vBiNormal[i].y < 0.0001 && vBiNormal[i].y > -0.0001) &&
				(vBiNormal[i].z < 0.0001 && vBiNormal[i].z > -0.0001)) {
				vBiNormal[i].y = 1.0f;
			}
			m_vControlPoint[i].m_xmf3B = vBiNormal[i];
		}
	}
	else {
		for (int i = 0; i < vBiNormal.size(); ++i) {
			if ((vBiNormal[i].x < 0.0001 && vBiNormal[i].x > -0.0001) &&
				(vBiNormal[i].y < 0.0001 && vBiNormal[i].y > -0.0001) &&
				(vBiNormal[i].z < 0.0001 && vBiNormal[i].z > -0.0001)) {
				vBiNormal[i].y = 1.0f;
			}
			m_vControlPoint[m_vControlIndex[i]].m_xmf3B = vBiNormal[i];
		}
	}
	vBiNormal.clear();
}
void CFbxImporter::RegenerateMesh(FbxMesh* pMesh, CFbxMeshData& MeshData) {

	FbxNode* pNode = pMesh->GetNode();

	//int numMaterials = layerElement->GetDirectArray().GetCount();
	if (m_vControlPoint.size() == m_vControlIndex.size()) {
		//같으면 control point index필요없게 재배열
		vector<stFBXControlPoint> vTempCP;
		vTempCP.resize(m_vControlPoint.size());
		for (int cpi = 0; cpi < m_vControlPoint.size(); ++cpi) {
			vTempCP[cpi] = m_vControlPoint[cpi];
		}
		for (int j = 0; j < m_vControlIndex.size(); ++j) {
			m_vControlPoint[j] = vTempCP[m_vControlIndex[j]];
		}
	}
	
	vector<vector<CVertexData>> vSubsetVertex;
	vector<vector<UINT>> vSubsetIndex;
	int nMaterial = m_nSubsetMesh + 1;

	vSubsetVertex.resize(nMaterial);
	vSubsetIndex.resize(nMaterial);

	//subset mesh 생성
	if(m_vControlIndex.size() != m_vControlPoint.size()){
		for (UINT i = 0; i < m_vControlIndex.size(); ++i) {
			CVertexData vd;
			UINT vertex_index = m_vControlIndex[i];
			vd.SetPosition(m_vControlPoint[vertex_index].m_xmf3Pos);
			vd.SetNormal(m_vControlPoint[vertex_index].m_xmf3Nor);
			vd.SetUV(m_vControlPoint[vertex_index].m_xmf2UV);
			vd.SetT(m_vControlPoint[vertex_index].m_xmf3T);
			vd.SetB(m_vControlPoint[vertex_index].m_xmf3B);
			for (int j = 0; j < m_vControlPoint[vertex_index].m_vBlendWeightPair.size(); ++j) {
				vd.GetBlendWeightPairs().push_back(m_vControlPoint[vertex_index].m_vBlendWeightPair[j]);
			}
			int subset_index = m_vMaterialIndex[i];
		
			UINT new_index{ 0 };
			auto iter = vSubsetVertex[subset_index].begin();
			for (iter; iter != vSubsetVertex[subset_index].end(); ++iter) {
				if ((*iter) == vd) {
					break;
				}
				new_index++;
			}
			if (iter != vSubsetVertex[subset_index].end()) {
				//있으면
				vSubsetIndex[subset_index].push_back(new_index);
			}
			else {
				//없으면
				vSubsetVertex[subset_index].push_back(vd);
				vSubsetIndex[subset_index].push_back(new_index);
			}
		}
	}
	else {
		for (UINT i = 0; i < m_vControlPoint.size(); ++i) {
			CVertexData vd;

			vd.SetPosition(m_vControlPoint[i].m_xmf3Pos);
			vd.SetNormal(m_vControlPoint[i].m_xmf3Nor);
			vd.SetUV(m_vControlPoint[i].m_xmf2UV);
			vd.SetT(m_vControlPoint[i].m_xmf3T);
			vd.SetB(m_vControlPoint[i].m_xmf3B);
			for (int j = 0; j < m_vControlPoint[i].m_vBlendWeightPair.size(); ++j) {
				vd.GetBlendWeightPairs().push_back(m_vControlPoint[i].m_vBlendWeightPair[j]);
			}
			int subset_index = m_vMaterialIndex[i];

			vSubsetVertex[subset_index].push_back(vd);
			//vSubsetIndex[subset_index].push_back(i);

		}
	}
	//subset mesh 생성

	CFbxMeshData SubsetMesh;
	wstring wsPath; wsPath.assign(m_FileName.begin(), m_FileName.end());
	for (int i = wsPath.size(); i > 0; --i) {
		if (wsPath[i] == L'/' || wsPath[i] == L'\\') {
			wsPath[i + 1] = L'\0';
			break;
		}
	}
	const WCHAR* pPath = wsPath.c_str();
	for (int i = 0; i < nMaterial; ++i) {
		//texture
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);
		if (pMaterial) {
			FbxProperty Property = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (Property.IsValid()) {
				FbxFileTexture* pTexture = Property.GetSrcObject<FbxFileTexture>();
				if (pTexture) {
					const FbxString strFileName = pTexture->GetFileName();
					CHAR FileName[64];
					for (int i = strFileName.Size(); i > 0; --i) {
						if (strFileName[i] == '\\' || strFileName[i] == '/') {
							int index{ 0 };
							for (int j = i+1; j < strFileName.Size(); ++j) {
								FileName[index++] = strFileName[j];
							}
							FileName[index] = '\0';
							break;
						}
					}
					//diffuse
					TCHAR			szFullPath[128] = L"";
					TCHAR			szFileName[128] = L"";
					lstrcpy(szFullPath, pPath);
					MultiByteToWideChar(CP_ACP, 0, FileName, strlen(FileName)
						, szFileName, 128);
					lstrcat(szFullPath, szFileName);
					if (false == file_exist(szFullPath)) {
						lstrcpy(szFullPath, L"../../Assets/default.jpg");
					}
					SubsetMesh.GetTexturePaths().push_back(szFullPath);

					//Spec
					TCHAR				szSpecFullPath[128] = L"";
					TCHAR				szSpecFileName[128] = L"";
					TCHAR				szSpecSave[32] = L"";
					TCHAR				szBar[2] = L"_";

					int					istrLen = 0;

					lstrcpy(szSpecFileName, szFileName);
					istrLen = lstrlen(szSpecFileName) - 1;

					for (int k = istrLen; k > 0; --k) {
						if (szSpecFileName[k] != szBar[0])
							continue;

						for (int j = 0; j < istrLen - k + 1; ++j) {
							szSpecSave[j] = szSpecFileName[k + j];
						}
						szSpecFileName[k - 1] = NULL;
						break;
					}

					lstrcat(szSpecFileName, L"SP");
					lstrcat(szSpecFileName, szSpecSave);
					lstrcpy(szSpecFullPath, pPath);
					lstrcat(szSpecFullPath, szSpecFileName);

					if (false == file_exist(szSpecFullPath)) {
						lstrcpy(szSpecFullPath, L"../../Assets/default.jpg");
					}
					SubsetMesh.GetTexturePaths().push_back(szSpecFullPath);

					//C
					TCHAR				szChanelFullPath[128] = L"";
					TCHAR				szChaneFileName[128] = L"";
					TCHAR				szChaneSave[32] = L"";

					istrLen = 0;

					lstrcpy(szChaneFileName, szFileName);
					istrLen = lstrlen(szChaneFileName) - 1;

					for (int k = istrLen; k > 0; --k)
					{
						if (szChaneFileName[k] != szBar[0])
							continue;

						for (int j = 0; j < istrLen - k + 1; ++j)
						{
							szChaneSave[j] = szChaneFileName[k + j];
						}
						szChaneFileName[k - 1] = NULL;
						break;
					}

					lstrcat(szChaneFileName, L"C");
					lstrcat(szChaneFileName, szChaneSave);
					lstrcpy(szChanelFullPath, pPath);
					lstrcat(szChanelFullPath, szChaneFileName);

					if (false == file_exist(szChanelFullPath)) {
						lstrcpy(szChanelFullPath, L"../../Assets/default.jpg");
					}
					SubsetMesh.GetTexturePaths().push_back(szChanelFullPath);

					//Normal
					TCHAR				szNormalFullPath[128] = L"";
					TCHAR				szNormalFileName[128] = L"";
					TCHAR				szNormalSave[32] = L"";

					istrLen = 0;

					lstrcpy(szNormalFileName, szFileName);
					istrLen = lstrlen(szNormalFileName) - 1;

					for (int k = istrLen; k > 0; --k)
					{
						if (szNormalFileName[k] != szBar[0])
							continue;

						for (int j = 0; j < istrLen - k + 1; ++j)
						{
							szNormalSave[j] = szNormalFileName[k + j];
						}
						szNormalFileName[k - 1] = NULL;
						break;
					}

					lstrcat(szNormalFileName, L"N");
					lstrcat(szNormalFileName, szNormalSave);
					lstrcpy(szNormalFullPath, pPath);
					lstrcat(szNormalFullPath, szNormalFileName);

					if (false == file_exist(szNormalFullPath)) {
						lstrcpy(szNormalFullPath, L"../../Assets/default.jpg");
					}
					SubsetMesh.GetTexturePaths().push_back(szNormalFullPath);
				}
			}
		}
		//texture

		SubsetMesh.SetByControlPoint(MeshData.GetByControlPoint());
		SubsetMesh.SetHasNormal(MeshData.GetHasNormal());
		SubsetMesh.SetHasUV(MeshData.GetHasUV());

		SubsetMesh.GetVertexDatas().resize(vSubsetVertex[i].size());
		for (int j = 0; j < vSubsetVertex[i].size(); ++j) {
			SubsetMesh.GetVertexDatas()[j] = vSubsetVertex[i][j];
		}

		if (m_vControlIndex.size() == m_vControlPoint.size()) {
			SubsetMesh.SetByControlPoint(true);
			m_iMeshCount++;
			m_vMeshData.push_back(SubsetMesh);
			SubsetMesh.GetTexturePaths().clear();
			continue;
		}

		SubsetMesh.GetIndexs().resize(vSubsetIndex[i].size());
		for (int j = 0; j < vSubsetIndex[i].size(); ++j) {
			SubsetMesh.GetIndexs()[j] = vSubsetIndex[i][j];
		}

		m_iMeshCount++;
		m_vMeshData.push_back(SubsetMesh);
		SubsetMesh.GetTexturePaths().clear();
	}

	m_nSubsetMesh = 0;
	m_vControlPoint.clear();
	m_vControlIndex.clear();
	m_vMaterialIndex.clear();

	//for (auto data : m_vvBlendWeightPair) {
	//	data.clear();
	//}
	//MeshData.GetTexturePaths().clear();
}
CFbxImporter::CFbxImporter() :CSingleTonBase<CFbxImporter>("fbximportersingleton") {

	m_pManager = FbxManager::Create();//1

									  //if (!m_pManager)
									  //{
									  //	DEBUGER->DebugMessageBox("Create()", "FBX Manager Create Fail");
									  //	exit(1);
									  //}
									  //else {
									  //	DEBUGER->DebugMessageBox("Create()", "FBX Manager Create Success");
									  //}

									  //m_pIoSettings = FbxIOSettings::Create(m_pManager, IOSROOT);//2.
}

CFbxImporter::~CFbxImporter() {

	if (m_pManager) {
		m_pManager->Destroy();
		m_pManager = nullptr;
	}
	if (m_pScene) {
		m_pScene->Destroy();
		m_pScene = nullptr;
	}
}
