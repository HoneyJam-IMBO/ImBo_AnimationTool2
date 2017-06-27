#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include "SingleTon.h"

struct stControlPoint {
	XMFLOAT3 m_xmf3Pos;
	XMFLOAT3 m_xmf3Nor;
	XMFLOAT2 m_xmf2UV;
};

class CDotXImporter: public CSingleTonBase<CDotXImporter> {

public:
	bool Begin(const TCHAR* pFilePath, tag Type = TAG_STATIC_OBJECT);
	bool End();

	//mesh
	UINT GetMeshCnt() { return static_cast<UINT>(m_vMeshData.size()); }
	vector<CFbxMeshData>& GetMeshDatas() { return m_vMeshData; }
	//anim
	CFbxAnimStackData& GetAnimStackData() { return m_AnimStackData; }
	CAnimationData& GetAnimationData() { return m_AnimationData; }
	CSkeletonData& GetSkeletonData() { return m_SkeletonData; }
	bool GetHasAnimation() { return m_bHasAnimationData; }


	//ready import func
	void ReadyImport(HWND hWnd, UINT width, UINT height);

private:
	//control points info
	int m_nFace{ 0 };
	int m_nControlPoint{ 0 };
	int m_nControlIndex{ 0 };
	XMFLOAT3* m_pControlPoint{ nullptr };
	XMFLOAT3* m_pControlNormal{ nullptr };
	XMFLOAT2* m_pControlUV{ nullptr };
	WORD* m_pControlIndex{ nullptr };
	//control points info
	//control texture path
	TCHAR** m_ppDifusePath{ nullptr };
	TCHAR** m_ppSpecPath{ nullptr };
	TCHAR** m_ppCPPath{ nullptr };
	//create mesh
	bool Create_Mesh(const TCHAR* pFilePath, tag Type);
	bool CreateStaticMesh(const TCHAR* pPath);

	bool CreateControlPoint(LPD3DXMESH pMesh);
	//bool CreateControlNormal(LPD3DXMESH pMesh, int stride, int offset);
	//bool CreateControlUV(LPD3DXMESH pMesh, int stride, int offset);
	bool CreateControlIndex(LPD3DXMESH pMesh);
	bool CreateControlTextures(LPD3DXMESH pMesh, LPD3DXBUFFER pSubset, const TCHAR * pPath);

	//위에 만든 controll data를 가지고 mesh info를 채운다.
	bool GenerateMeshInfo(LPD3DXMESH pMesh);

	enum WINMODE { MODE_FULL, MODE_WIN };
	void SetParameters(D3DPRESENT_PARAMETERS& d3dpp, HWND hWnd, UINT width, UINT height);
	LPDIRECT3D9					m_pSDK;
	LPDIRECT3DDEVICE9			m_pDevice; // 장치를 대표하는 객체다.

	
	//animation data
	CFbxAnimStackData m_AnimStackData;
	CAnimationData m_AnimationData;
	CSkeletonData m_SkeletonData;

	//mesh data
	UINT m_MeshScale{ 1 };
	vector<CFbxMeshData>			 m_vMeshData;
	FbxArray<FbxString*>			 m_sAnimationNames;
	bool							 m_bHasAnimationData{ false };
	int m_iMeshCount{ 0 };

public:
	
	CDotXImporter();
	//생성한 FBX객체 소멸 
	virtual ~CDotXImporter();


};
