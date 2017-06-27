#include "stdafx.h"
#include "DotXImporter.h"

bool CDotXImporter::Begin(const TCHAR* pFilePath, tag Type){
	return Create_Mesh(pFilePath, Type);
	return true;
}

bool CDotXImporter::End(){
	m_nFace = 0;
	m_nControlPoint = 0 ;
	m_nControlIndex = 0;
	if (m_pControlPoint) delete m_pControlPoint;
	m_pControlPoint = nullptr;
	if (m_pControlNormal) delete m_pControlNormal;
	m_pControlNormal = nullptr;
	if (m_pControlUV)delete m_pControlUV;
	m_pControlUV = nullptr;
	if (m_pControlIndex) delete m_pControlIndex;
	m_pControlIndex = nullptr;
	//control points info
	//control texture path
	if (m_ppDifusePath) {
		for (int i = 0; i < m_iMeshCount; ++i) {
			delete m_ppDifusePath[i];
		}
		delete m_ppDifusePath;
	}
	m_ppDifusePath = nullptr;
	if (m_ppSpecPath) {
		for (int i = 0; i < m_iMeshCount; ++i) {
			delete m_ppSpecPath[i];
		}
		delete m_ppSpecPath;
	}
	m_ppSpecPath = nullptr;
	if (m_ppCPPath) {
		for (int i = 0; i < m_iMeshCount; ++i) {
			delete m_ppCPPath[i];
		}
		delete m_ppCPPath;
	}
	m_ppCPPath = nullptr;

	for (auto data : m_vMeshData) {
		data.End();
	}
	m_vMeshData.clear();

	return true;
}

bool CDotXImporter::Create_Mesh(const TCHAR* pFilePath, tag Type){
	switch (Type)
	{
	case TAG_STATIC_OBJECT:
		return CreateStaticMesh(pFilePath);
		break;

	case TAG_DYNAMIC_OBJECT:
		//CreateDynamicMesh(pFilePath, pFileName);
		return false;
		break;
	}
	
	return false;
}

bool CDotXImporter::CreateStaticMesh(const TCHAR * pPath){
	//mesh
	LPD3DXMESH				pMesh;
	LPD3DXBUFFER			pAdjancey; // 인접폴리곤에 대한 정보다.
	LPD3DXBUFFER			pSubset;
	unsigned long nSubset{ 0 };
	HRESULT		hr = NULL;
	TCHAR		szFullPath[128] = L"";

	lstrcpy(szFullPath, pPath);
	//lstrcat(szFullPath, pFileName);

	if (FAILED(D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pDevice, &pAdjancey, &pSubset, NULL, &nSubset, &pMesh)))
		return false;
	m_iMeshCount = nSubset;
	
	//create control data
	if (false == CreateControlPoint(pMesh)) return false;
	//CreateControlNormal(pMesh, stride, wNormalOffset);
	//CreateControlUV(pMesh, stride, wUVOffset);
	CreateControlIndex(pMesh);

	wstring wsPath{ pPath };
	for (int i = wsPath.size(); i > 0; --i) {
		if (wsPath[i] == L'/' || wsPath[i] == L'\\') {
			wsPath[i+1] = L'\0';
			break;
		}
	}
	
	CreateControlTextures(pMesh, pSubset, wsPath.c_str());
	//create control data

	GenerateMeshInfo(pMesh);

	return true;
}

bool CDotXImporter::CreateControlPoint(LPD3DXMESH pMesh){
	D3DVERTEXELEMENT9				Element[MAX_FVF_DECL_SIZE];
	ZeroMemory(Element, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);
	pMesh->GetDeclaration(Element);
	int wPositionOffset, wNormalOffset, wUVOffset = -1;
	int stride = 0;
	
	for (int i = 0; i < MAX_FVF_DECL_SIZE; ++i) {
		if (Element[i].Stream == 255) break;
		//D3DDECLUSAGE_BLENDWEIGHT,   // 1
		//D3DDECLUSAGE_BLENDINDICES,  // 2
		//D3DDECLUSAGE_NORMAL,        // 3
		//D3DDECLUSAGE_TEXCOORD,      // 5
		if (Element[i].Usage == D3DDECLUSAGE_TANGENT || Element[i].Usage == D3DDECLUSAGE_BINORMAL) {
			stride += sizeof(XMFLOAT3);
		}
		else if (Element[i].Usage == D3DDECLUSAGE_POSITION) {
			wPositionOffset = Element[i].Offset;
			stride += sizeof(XMFLOAT3);
		}
		else if (Element[i].Usage == D3DDECLUSAGE_NORMAL) {
			wNormalOffset = Element[i].Offset;
			stride += sizeof(XMFLOAT3);
		}
		else if (Element[i].Usage == D3DDECLUSAGE_TEXCOORD) {
			wUVOffset = Element[i].Offset;
			stride += sizeof(XMFLOAT2);
		}
		else {
			switch (Element[i].Type) {
			case 0:
				stride += sizeof(float);
				break;
			case 1:
				stride += sizeof(XMFLOAT2);
				break;
			case 2:
				stride += sizeof(XMFLOAT3);
				break;
			case 3:
				stride += sizeof(XMFLOAT4);
				break;
			case 4:
				stride += sizeof(float);
				break;
			}
			
		}

	}
	if (wUVOffset < 0 || wPositionOffset < 0 || wNormalOffset < 0) {
		DEBUGER->DebugGMessageBox(L"dotx", L"no info");
		return false;
	}


	// 메시를 이루고있는 정점의 전체 갯수를 얻어온다.
	unsigned long dwNumVertices = pMesh->GetNumVertices();
	if (dwNumVertices < 0) return false;

	m_nControlPoint = pMesh->GetNumVertices();
	m_nFace = pMesh->GetNumFaces();
	m_nControlIndex = m_nFace * 3;

	//control point 제작
	m_pControlPoint = new XMFLOAT3[dwNumVertices];
	m_pControlNormal = new XMFLOAT3[dwNumVertices];
	m_pControlUV = new XMFLOAT2[dwNumVertices];
	//control point 제작

	// 메시를 이루고있는 정점들(배열)의 가장 앞주소를 저장
	byte*			pVertex = NULL;

	// 메시를 락!! 왜? 정점의 정보를 얻어올려고
	pMesh->LockVertexBuffer(0, (void**)&pVertex);
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 UV;
	// pVertex + wPositionOffset : 메시를 이루고있는 정점들(배열)의 가장 앞주소에다가 wPositionOffset를 더하면 위치정보를 담고있는 공간의 주소
	vector<XMFLOAT2> vUV1;
	vector<XMFLOAT2> vUV2;
	vector<XMFLOAT2> vUV3;
	vector<XMFLOAT2> vUV4;
	for (int i = 0; i < m_nControlPoint; ++i) {
		Position = *((XMFLOAT3*)(pVertex + wPositionOffset + (stride * i)));
		Normal = *((XMFLOAT3*)(pVertex + wNormalOffset + (stride* i)));
		UV = *((XMFLOAT2*)(pVertex + wUVOffset + (stride* i)));
		m_pControlPoint[i] = Position;
		m_pControlNormal[i] = Normal;
		m_pControlUV[i] = UV;
		vUV1.push_back(UV);
		UV = *((XMFLOAT2*)(pVertex + wUVOffset + (stride* i) + sizeof(XMFLOAT2)));
		vUV2.push_back(UV);
		UV = *((XMFLOAT2*)(pVertex + wUVOffset + (stride* i) + sizeof(XMFLOAT2)*2));
		vUV3.push_back(UV);
		UV = *((XMFLOAT2*)(pVertex + wUVOffset + (stride* i) + sizeof(XMFLOAT2)*3));
		vUV4.push_back(UV);
	}
	pMesh->UnlockVertexBuffer();

	return true;
} 
bool CDotXImporter::CreateControlIndex(LPD3DXMESH pMesh){
	//if (m_nControlPoint == m_nFace * 3) return false;

	m_pControlIndex = new WORD[m_nControlIndex];
	WORD* pIndices;
	pMesh->LockIndexBuffer(0, (void**)&pIndices);

	for (int i = 0; i < m_nControlIndex; ++i) {
		m_pControlIndex[i] = pIndices[i];
	}
	pMesh->UnlockIndexBuffer();

	return false;
}

bool CDotXImporter::CreateControlTextures(LPD3DXMESH pMesh, LPD3DXBUFFER pSubset, const TCHAR * pPath) {
	//texture
	LPDIRECT3DTEXTURE9*		ppTexture;
	LPDIRECT3DTEXTURE9*		ppNormalTexture;
	LPDIRECT3DTEXTURE9*		ppSpecularTexture;
	LPDIRECT3DTEXTURE9*		ppCTexture;
	D3DMATERIAL9*			pMtrl;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
	LPD3DXMATERIAL	pSubSetInfo = (D3DXMATERIAL*)pSubset->GetBufferPointer();

	m_ppDifusePath = new TCHAR*[m_iMeshCount];
	for (int i = 0; i < m_iMeshCount; ++i) {
		m_ppDifusePath[i] = new TCHAR[128];
	}
	m_ppSpecPath = new TCHAR*[m_iMeshCount];
	for (int i = 0; i < m_iMeshCount; ++i) {
		m_ppSpecPath[i] = new TCHAR[128];
	}
	m_ppCPPath = new TCHAR*[m_iMeshCount];
	for (int i = 0; i < m_iMeshCount; ++i) {
		m_ppCPPath[i] = new TCHAR[128];
	}
	LPDIRECT3DTEXTURE9 testTexture;

	for (unsigned long i = 0; i < m_iMeshCount; ++i)
	{
		//diffuse
		TCHAR			szFullPath[128] = L"";
		TCHAR			szFileName[128] = L"";
		lstrcpy(szFullPath, pPath);
		MultiByteToWideChar(CP_ACP, 0, pSubSetInfo[i].pTextureFilename, strlen(pSubSetInfo[i].pTextureFilename)
			, szFileName, 128);

		lstrcat(szFullPath, szFileName);
		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, &testTexture))) {
			lstrcpy(szFullPath, L"../../Assets/default.jpg");
		}
		lstrcpy(m_ppDifusePath[i], szFullPath);
		//diffuse


		//Spec
		TCHAR				szSpecFullPath[128] = L"";
		TCHAR				szSpecFileName[128] = L"";
		TCHAR				szSpecSave[32] = L"";
		TCHAR				szBar[2] = L"_";

		int					istrLen = 0;

		lstrcpy(szSpecFileName, szFileName);
		istrLen = lstrlen(szSpecFileName) - 1;

		for (int k = istrLen; k > 0; --k){
			if (szSpecFileName[k] != szBar[0])
				continue;

			for (int j = 0; j < istrLen - k + 1; ++j){
				szSpecSave[j] = szSpecFileName[k + j];
			}
			szSpecFileName[k - 1] = NULL;
			break;
		}

		lstrcat(szSpecFileName, L"SP");
		lstrcat(szSpecFileName, szSpecSave);
		lstrcpy(szSpecFullPath, pPath);
		lstrcat(szSpecFullPath, szSpecFileName);

		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szSpecFullPath, &testTexture))){
			lstrcpy(szSpecFullPath, L"../../Assets/default.jpg");
		}
		lstrcpy(m_ppSpecPath[i], szSpecFullPath);

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

		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szChanelFullPath, &testTexture))){
			lstrcpy(szChanelFullPath, L"../../Assets/default.jpg");
		}
		lstrcpy(m_ppCPPath[i], szChanelFullPath);
	}
	return true;

}
bool IsSame(const XMFLOAT3& src, const XMFLOAT3& dest, float offset = 0.0001) {
	if (((src.x + offset) > dest.x) && ((src.x - offset) < dest.x))
		if (((src.y + offset) > dest.y) && ((src.y - offset) < dest.y))
			if (((src.z + offset) > dest.z) && ((src.z - offset) < dest.z))
				return true;
	return false;
}
bool IsSame(const XMFLOAT2& src, const XMFLOAT2& dest, float offset = 0.0001) {
	if (((src.x + offset) > dest.x) && ((src.x - offset) < dest.x))
		if (((src.y + offset) > dest.y) && ((src.y - offset) < dest.y))
			return true;
	return false;
}
bool IsSame(const stControlPoint& src, const stControlPoint& dest, float offset = 0.0001) {
	if (IsSame(src.m_xmf3Pos, dest.m_xmf3Pos, offset))
		if (IsSame(src.m_xmf3Nor, dest.m_xmf3Nor, offset))
			if (IsSame(src.m_xmf2UV, dest.m_xmf2UV, offset))
				return true;
	return false;
}
bool CDotXImporter::GenerateMeshInfo(LPD3DXMESH pMesh){
	pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT, 0, 0, 0, 0);
	DWORD numSubsets{ 0 };
	pMesh->GetAttributeTable(0, &numSubsets);
	D3DXATTRIBUTERANGE* table = new D3DXATTRIBUTERANGE[numSubsets];
	pMesh->GetAttributeTable(table, &numSubsets);

	if (numSubsets != m_iMeshCount) return false;

	int prevVc{ 0 };
	for (int i = 0; i < numSubsets; ++i) {
		vector<stControlPoint> vCP;
		vector<UINT> vIndex;

		//create mesh data!
		CFbxMeshData MeshData;
		MeshData.SetHasNormal(true);
		MeshData.SetHasUV(true);
		MeshData.SetByControlPoint(false);
	
		if (i != 0) prevVc = table[i - 1].VertexCount;
		int vc = table[i].VertexCount;
		int vs = table[i].VertexStart;
		WORD startIndex = table[i].FaceStart * 3;
		WORD nIndex = (table[i].FaceCount) * 3;
		WORD endIndex = startIndex + nIndex;

		//for (int i = 0; i < vc; ++i) {
		//	vPos.push_back(m_pControlPoint[vs + i]);
		//	vNor.push_back(m_pControlNormal[vs + i]);
		//	vUV.push_back(m_pControlUV[vs + i]);
		//}
		//for (int j = startIndex; j < endIndex; ++j) {
		//	int cur = m_pControlIndex[j] -prevVc;
		//	//if (cur < 0)
		//	//	continue;
		//	vIndex.push_back(cur);
		//}
		vector<stControlPoint>::iterator iter;
		//start ~ end
		for (int j = startIndex; j < endIndex; ++j) {
			UINT curControlIndex = m_pControlIndex[j];
			stControlPoint cp;
			cp.m_xmf3Pos = m_pControlPoint[curControlIndex];
			cp.m_xmf3Nor = m_pControlNormal[curControlIndex];
			cp.m_xmf2UV = m_pControlUV[curControlIndex];

			//새로운 index의 pos를 vPos에서 찾는다.
			UINT newIndex{ 0 };
	
			for (iter = vCP.begin(); iter != vCP.end(); ++iter) {
				if (IsSame(cp, *iter)) break;
				newIndex++;
			}

			if (vCP.end() != iter) {//안에 있다면 
				//현재 index만을 넣고 
				vIndex.push_back(newIndex);
			}
			else {//안에 없으면
				//index 넣고
				vIndex.push_back(newIndex);
				//pos도 넣는다.
				vCP.push_back(cp);
			}
		}//end for
		MeshData.GetVertexDatas().resize(vCP.size());
		for (int i = 0; i < vCP.size(); ++i) {
			MeshData.GetVertexDatas()[i].SetPosition(vCP[i].m_xmf3Pos);
			MeshData.GetVertexDatas()[i].SetNormal(vCP[i].m_xmf3Nor);
			MeshData.GetVertexDatas()[i].SetUV(vCP[i].m_xmf2UV);
		}
		for (int i = 0; i < vIndex.size(); ++i) {
			MeshData.GetIndexs().push_back(vIndex[i]);
		}
		
		MeshData.GetTexturePaths().push_back(m_ppDifusePath[i]);
		MeshData.GetTexturePaths().push_back(m_ppSpecPath[i]);
		MeshData.GetTexturePaths().push_back(m_ppCPPath[i]);

		m_vMeshData.push_back(MeshData);

		vIndex.clear();
		vCP.clear();
	}//end for

	return true;
}

void CDotXImporter::ReadyImport(HWND hWnd, UINT width, UINT height){
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9		devicecaps;
	ZeroMemory(&devicecaps, sizeof(D3DCAPS9));

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &devicecaps)))
	{
		DEBUGER->DebugGMessageBox(L"dotx", L"GetDeviceCaps Failed");
		return;
	}

	DWORD		vp;

	if (devicecaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	vp |= D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp, hWnd, width, height);

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL
		, d3dpp.hDeviceWindow, vp, &d3dpp, &m_pDevice)))
	{
		DEBUGER->DebugGMessageBox(L"dotx", L"CreateDevice Failed");
		return;
	}

	return;
}

void CDotXImporter::SetParameters(D3DPRESENT_PARAMETERS & d3dpp, HWND hWnd, UINT width, UINT height){
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dpp.MultiSampleQuality = 0;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = TRUE;

	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = MODE_WIN;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

CDotXImporter::CDotXImporter() :CSingleTonBase<CDotXImporter>("dotximportersingleton") {
	
}

CDotXImporter::~CDotXImporter()
{
}
