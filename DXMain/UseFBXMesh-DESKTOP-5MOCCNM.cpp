#include "stdafx.h"
#include "UseFBXMesh.h"

static bool gVerbose = true;

bool CUseFBXMesh::Begin(string path) {
	//manager랑 
	m_manager = FbxManager::Create();
	
	//io setting은 한번 만들고 계속 사용한다.
	m_ioSettings = FbxIOSettings::Create(m_manager, IOSROOT);
	m_manager->SetIOSettings(m_ioSettings);
	
	//scene importer는 재사용 불가 
	m_scene = FbxScene::Create(m_manager, "tempName");
	
	m_importer = FbxImporter::Create(m_manager, "");
	if (!m_importer->Initialize(path.c_str(), -1, m_manager->GetIOSettings()))
	{
		std::string ErrorTitle = "FBX Import Error";
		DEBUGER->DebugMessageBox(ErrorTitle, "path");
	}
	
	m_importer->Import(m_scene);
	m_importer->Destroy();
	
	m_pRootNode = m_scene->GetRootNode();
	if (!m_pRootNode) {
		std::string ErrorTitle = "FBX Import Error";
		DEBUGER->DebugMessageBox(ErrorTitle, "node");
	}

	CMesh::Begin();

	return true;
}

bool CUseFBXMesh::End() {
	if (m_importer)
	{
		m_importer->Destroy();
		m_importer = nullptr;
	}
	
	if (m_scene)
	{
		m_scene->Destroy();
		m_scene = nullptr;
	}
	
	if (m_manager)
	{
		m_manager->Destroy();
		m_manager = nullptr;
	}
	
	if (m_pRootNode)
	{
		m_pRootNode->Destroy();
		m_pRootNode = nullptr;
	}
	return CMesh::End();
}

bool CUseFBXMesh::CreateVertexBuffer() {

	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//fbx model load
	m_nChilds = m_pRootNode->GetChildCount();
	GetPositionData(m_pRootNode);
	GetNormalData(m_pRootNode);
	XMFLOAT2* pxmf2Uv = new XMFLOAT2[m_nIndices];
	for (unsigned int i = 0; i < m_nIndices; ++i) {
		pxmf2Uv[i] = XMFLOAT2(0, 0);
	}
	//fbx model load

	m_pd3dPositionBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pVertices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nNormals, m_pNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dUVBuffer = CreateBuffer(sizeof(XMFLOAT2), m_nIndices, pxmf2Uv, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	//-----------------------------------버퍼 조립-----------------------------------
	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dUVBuffer };
	UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[3] = { 0, 0, 0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	//-----------------------------------버퍼 조립-----------------------------------
	
	//create space mesh aabb
	BoundingBox::CreateFromPoints(m_BoundingBox, (size_t)m_nVertices, m_pVertices, (size_t)sizeof(XMFLOAT3));
	//BoundingBox::CreateFromPoints(m_BoundingBox, XMLoadFloat3(&XMFLOAT3(-fx, -fy, -fz)), XMLoadFloat3(&XMFLOAT3(fx, fy, fz)));

	if (m_ppd3dVertexBuffers)	return true;

	return false;
}
bool CUseFBXMesh::CreateIndexBuffer() {
	
	//인덱스
	FbxNode *pChildNode = nullptr;
	//모든 child 수 만큼 루프를 돌면서
	for (int i = 0; i < m_nChilds; i++) {
	
		pChildNode = m_pRootNode->GetChild(i);
		//mesh를 얻어온다.
		FbxMesh *pChildMesh = pChildNode->GetMesh();
		if (pChildMesh != nullptr) {
	
			m_nIndices = pChildMesh->GetPolygonVertexCount();
			m_pnIndices = new UINT[m_nIndices];
			m_pnIndices = (UINT*)pChildMesh->GetPolygonVertices();
		}
	}
	
	m_pd3dIndexBuffer = CreateBuffer(sizeof(int), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);


	if (m_pd3dIndexBuffer) return true;

	return false;
}


void CUseFBXMesh::SetShaderState() {
	//메쉬
	m_pd3dDeviceContext->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	// 꼭지점 버퍼를 연결합니다.
	m_pd3dDeviceContext->IASetVertexBuffers(m_nStartSlot, m_nVertexBuffers, m_ppd3dVertexBuffers, m_pnVertexStrides, m_pnVertexOffsets);

	// 인덱스 버퍼를 연결합니다.
	m_pd3dDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, m_d3dIndexFormat, m_nIndexOffset);


}

void CUseFBXMesh::RenderExcute(UINT nInstance) {

	if (m_pd3dIndexBuffer)
		m_pd3dDeviceContext->DrawIndexedInstanced(m_nIndices, nInstance, m_nStartIndexLocation, m_nBaseVertexLocation, m_nStartInstanceLocation);
	else
		m_pd3dDeviceContext->DrawInstanced(m_nVertices, nInstance, m_nStartVertexLocation, m_nStartInstanceLocation);

}

void CUseFBXMesh::GetPositionData(FbxNode * pNode){
	FbxMesh* lMesh = pNode->GetMesh();
	if (lMesh) {
		m_nPolygons = lMesh->GetPolygonCount();

		//정점
		m_nVertices = lMesh->GetControlPointsCount();
		m_pVertices = new XMFLOAT3[m_nVertices];
		//모든 정점 수 만큼
		for (unsigned int j = 0; j < m_nVertices; ++j) {
			m_pVertices[j].x = static_cast<float>(lMesh->GetControlPointAt(j).mData[0]);
			m_pVertices[j].y = static_cast<float>(lMesh->GetControlPointAt(j).mData[1]);
			m_pVertices[j].z = static_cast<float>(lMesh->GetControlPointAt(j).mData[2]);
		}//end for
	}//end if lMesh

	 //recursively traverse each node in the scene
	int i, lCount = pNode->GetChildCount();
	for (i = 0; i < lCount; i++){
		GetPositionData(pNode->GetChild(i));
	}

}

//get mesh normals info
void CUseFBXMesh::GetNormalData(FbxNode* pNode)
{
	if (!pNode)
		return;

	FbxMesh* currMesh = pNode->GetMesh();
	if (currMesh){

		m_nPolygons = currMesh->GetPolygonCount();
		int vertexCounter = 0;
		vector<XMFLOAT3> vNormal;

		for (unsigned int i = 0; i < m_nPolygons; ++i){
			for (unsigned int j = 0; j < 3; ++j){
				XMFLOAT3 xmf3Normal;
				int ctrlPointIndex = currMesh->GetPolygonVertex(i, j);

				ReadNormal(currMesh, ctrlPointIndex, vertexCounter, xmf3Normal);
				vNormal.push_back(xmf3Normal);
				++vertexCounter;
			}
		}

		//normal buffer data 제작
		m_nNormals = vNormal.size();
		m_pNormals = new XMFLOAT3[m_nNormals];
		for (int i = 0; i < m_nNormals; ++i) {
			m_pNormals[i] = vNormal[i];
		}
	}
	//recursively traverse each node in the scene
	int i, lCount = pNode->GetChildCount();
	for (i = 0; i < lCount; i++){
		GetNormalData(pNode->GetChild(i));
	}
//	//asdasd
//	//get mesh
//	FbxMesh* lMesh = pNode->GetMesh();
//	if (lMesh)
//	{
//		vector<XMFLOAT3> vNormals;
//		//print mesh node name
//		FBXSDK_printf("current mesh node: %s\n", pNode->GetName());
//
//		//get the normal element
//		FbxGeometryElementNormal* lNormalElement = lMesh->GetElementNormal();
//		if (lNormalElement)
//		{
//			//mapping mode is by control points. The mesh should be smooth and soft.
//			//we can get normals by retrieving each control point
//			if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
//			{
//				//Let's get normals of each vertex, since the mapping mode of normal element is by control point
//				for (int lVertexIndex = 0; lVertexIndex < lMesh->GetControlPointsCount(); lVertexIndex++)
//				{
//					int lNormalIndex = 0;
//					//reference mode is direct, the normal index is same as vertex index.
//					//get normals by the index of control vertex
//					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
//						lNormalIndex = lVertexIndex;
//
//					//reference mode is index-to-direct, get normals by the index-to-direct
//					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
//						lNormalIndex = lNormalElement->GetIndexArray().GetAt(lVertexIndex);
//
//					//Got normals of each vertex.
//					FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
//					if (gVerbose) FBXSDK_printf("normals for vertex[%d]: %f %f %f %f \n", lVertexIndex, lNormal[0], lNormal[1], lNormal[2], lNormal[3]);
//					vNormals.push_back(XMFLOAT3{ (float)(lNormal[0]), (float)(lNormal[1]), (float)(lNormal[2]) });
//					//add your custom code here, to output normals or get them into a list, such as KArrayTemplate<FbxVector4>
//					//. . .
//				}//end for lVertexIndex
//			}//end eByControlPoint
//			 //mapping mode is by polygon-vertex.
//			 //we can get normals by retrieving polygon-vertex.
//			else if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
//			{
//				int lIndexByPolygonVertex = 0;
//				//Let's get normals of each polygon, since the mapping mode of normal element is by polygon-vertex.
//				for (int lPolygonIndex = 0; lPolygonIndex < lMesh->GetPolygonCount(); lPolygonIndex++)
//				{
//					//get polygon size, you know how many vertices in current polygon.
//					int lPolygonSize = lMesh->GetPolygonSize(lPolygonIndex);
//					//retrieve each vertex of current polygon.
//					for (int i = 0; i < lPolygonSize; i++)
//					{
//						int lNormalIndex = 0;
//						//reference mode is direct, the normal index is same as lIndexByPolygonVertex.
//						if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
//							lNormalIndex = lIndexByPolygonVertex;
//
//						//reference mode is index-to-direct, get normals by the index-to-direct
//						if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
//							lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);
//
//						//Got normals of each polygon-vertex.
//						FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
//						if (gVerbose) FBXSDK_printf("normals for polygon[%d]vertex[%d]: %f %f %f %f \n",
//							lPolygonIndex, i, lNormal[0], lNormal[1], lNormal[2], lNormal[3]);
//						vNormals.push_back(XMFLOAT3{ static_cast<float>(lNormal[0]), static_cast<float>(lNormal[1]), static_cast<float>(lNormal[2]) });
//						//add your custom code here, to output normals or get them into a list, such as KArrayTemplate<FbxVector4>
//						//. . .
//
//						lIndexByPolygonVertex++;
//					}//end for i //lPolygonSize
//				}//end for lPolygonIndex //PolygonCount
//
//			}//end eByPolygonVertex
//		}//end if lNormalElement
//
//		//normal buffer data 제작
//		m_nNormals = vNormals.size();
//		m_pNormals = new XMFLOAT3[m_nNormals];
//		for (int i = 0; i < m_nNormals; ++i) {
//			m_pNormals[i] = vNormals[i];
//		}
//	}//end if lMesh
//
//	 //recursively traverse each node in the scene
//	int i, lCount = pNode->GetChildCount();
//	for (i = 0; i < lCount; i++)
//	{
//		GetNormalData(pNode->GetChild(i));
//	}
}


void CUseFBXMesh::GetUVData(FbxMesh* pChildMesh){
	//=================================================================================================================
	//텍스쳐 uv
	m_pUVs = new XMFLOAT2[m_nVertices];
	//UV 이름 리스트를 가져온다. 이름은 UV Channel 1 식으로 나열.
	FbxStringList lUVSetNameList;
	pChildMesh->GetUVSetNames(lUVSetNameList);
	
	//iterating over all uv sets
	for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
	{
		//get lUVSetIndex-th uv set
		const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
		const FbxGeometryElementUV* lUVElement = pChildMesh->GetElementUV(lUVSetName);
	
		if (!lUVElement)
			continue;
	
		// only support mapping mode eByPolygonVertex and eByControlPoint
		if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
			lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
			return;
	
		//index array, where holds the index referenced to the uv data
		const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
	
		const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;
	
		if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			for (int lPolyIndex = 0; lPolyIndex < m_nPolygons; ++lPolyIndex)
			{
				// build the max index array that we need to pass into MakePoly
				const int lPolySize = pChildMesh->GetPolygonSize(lPolyIndex);//이게 아마 폴리곤의 점수
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)//점 수 만큼 도는데
				{
					FbxVector2 lUVValue;
	
					//get the index of the current vertex in control points array
					int lPolyVertexIndex = pChildMesh->GetPolygonVertex(lPolyIndex, lVertIndex);//지금 점수와 폴리곤의 점수를 알면
																								//정점의 index를 알 수 있지.
	
																								//index를 사용하면 index마다 uv가 필요하니까 조사 후
																								//사용중이면 uv의 index를 구해온다. 안쓰면 vertex index가 곧 uv index
					int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertexIndex) : lPolyVertexIndex;
					//uv index를 가지고 uv정보를 가져온다.
					lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
	
					m_pUVs[lUVIndex] = (XMFLOAT2(static_cast<float>(lUVValue.mData[0]), static_cast<float>(lUVValue.mData[1])));
				}
			}
		}
		else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int lPolyIndexCounter = 0;
			for (int lPolyIndex = 0; lPolyIndex < m_nPolygons; ++lPolyIndex) {//모든 폴리곤 수 만큼
																			  // build the max index array that we need to pass into MakePoly
				const int lPolySize = pChildMesh->GetPolygonSize(lPolyIndex);
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex) {//한면의 정점 수 만큼
					if (lPolyIndexCounter < lIndexCount)
					{
						FbxVector2 lUVValue;
	
						//the UV index depends on the reference mode
						int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;
	
						lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
	
						m_pUVs[lUVIndex] = XMFLOAT2(static_cast<float>(lUVValue.mData[0]), static_cast<float>(lUVValue.mData[1]));
	
						lPolyIndexCounter++;
					}
				}
			}
		}
		m_pd3dUVBuffer = CreateBuffer(sizeof(XMFLOAT2), lIndexCount, m_pUVs, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	}

}
void CUseFBXMesh::ReadNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outNormal)
{
	if (inMesh->GetElementNormalCount() < 1)
	{
		throw std::exception("Invalid Normal Number");
	}

	FbxGeometryElementNormal* vertexNormal = inMesh->GetElementNormal(0);
	switch (vertexNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inCtrlPointIndex);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inVertexCounter);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}
void CUseFBXMesh::ProcessMesh(FbxNode* inNode)
{
	FbxMesh* currMesh = inNode->GetMesh();

	m_nPolygons = currMesh->GetPolygonCount();
	int vertexCounter = 0;
	vector<XMFLOAT3> vNormal;

	for (unsigned int i = 0; i < m_nPolygons; ++i)
	{
		for (unsigned int j = 0; j < 3; ++j)
		{
			XMFLOAT3 xmf3Normal;
			int ctrlPointIndex = currMesh->GetPolygonVertex(i, j);
			//CtrlPoint* currCtrlPoint = mControlPoints[ctrlPointIndex];


			ReadNormal(currMesh, ctrlPointIndex, vertexCounter, xmf3Normal);
			vNormal.push_back(xmf3Normal);
			// We only have diffuse texture
			//for (int k = 0; k < 1; ++k)
			//{
			//	ReadUV(currMesh, ctrlPointIndex, currMesh->GetTextureUVIndex(i, j), k, UV[j][k]);
			//}
			++vertexCounter;
		}
	}

	//normal buffer data 제작
	m_nNormals = vNormal.size();
	m_pNormals = new XMFLOAT3[m_nNormals];
	for (int i = 0; i < m_nNormals; ++i) {
		m_pNormals[i] = vNormal[i];
	}
}

CUseFBXMesh::CUseFBXMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CUseFBXMesh::~CUseFBXMesh(){

}