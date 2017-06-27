#pragma once

#include "SingleTon.h"
#include "FbxMeshData.h"
#include "FbxAnimStackData.h"
#include "SkeletonData.h"
#include "AnimationData.h"

#define ANIMATION_NUM 1

inline bool file_exist(const std::wstring& name) {
	wifstream in(name);
	if (!in) {
		return false;
	}
	in.close();
	return true;
}


struct stFBXControlPoint {
	XMFLOAT3 m_xmf3Pos;
	XMFLOAT3 m_xmf3Nor;
	XMFLOAT2 m_xmf2UV;
	XMFLOAT3 m_xmf3T;
	XMFLOAT3 m_xmf3B;
	int m_nMaterialIndex;
	vector<CFbxBlendWeightPair> m_vBlendWeightPair;
};

class CFbxImporter : public CSingleTonBase<CFbxImporter> {

public:
	//FBX Scene하고 Importer제작
	//bool Begin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CDirectXFramework* pFrameWork);
	bool Begin(string path);
	//FBX Scene하고 Importer소멸
	bool End();

	//get setter

	//mesh
	UINT GetMeshCnt() { return static_cast<UINT>(m_vMeshData.size()); }
	vector<CFbxMeshData>& GetMeshDatas() { return m_vMeshData; }
	//anim
	CFbxAnimStackData& GetAnimStackData() { return m_AnimStackData; }
	CAnimationData& GetAnimationData() { return m_AnimationData; }
	CSkeletonData& GetSkeletonData() { return m_SkeletonData; }

	//UINT GetAnimationCnt() { return static_cast<UINT>(m_vAnimationData.size()); }
	//UINT GetTotalJointCnt();
	//vector<CFbxAnimationData>& GetAnimationDatas() { return m_vAnimationData; }

	bool GetHasAnimation() { return m_bHasAnimationData; }
	//get setter
	FbxScene* GetScene() { return m_pScene; }
	
	//read fbx file func
	void LoadFile();

	//helper func
	//애니메이션 이름 set
	void SetAnimationNameArray();
	//이니메이션 stack
	void LoadAnimStack();
	//get skeleton info 
	void LoadSkeleton(FbxNode* pNode);
	void ProcessSkeletonHierarchyRecursively(FbxNode * inNode, int myIndex, int inParentIndex);
	//get skeleton info 

	//본격적인 fbx info get
	void LoadNodeRecursive(FbxNode* pNode);
	bool ExportMeshData(FbxMesh* pMesh);
	//animation
	bool ExportAnimationData(FbxMesh* pMesh);
	bool ReformBlendWeightPairInfo();

	UINT FindJointIndexUsingName(const string& JointName);
	//UINT GetCurrMeshJointIndexOffset();
	FbxAMatrix GetGeometryTransformation(FbxMesh* pMesh);
	//helper func
private:
	//fbx data
	string m_FileName;
	FbxManager *m_pManager;

	FbxImporter *m_pImporter{ nullptr };
	FbxScene *m_pScene{ nullptr };
	//fbx data

	ID3D11RasterizerState* m_pRasterizerState{ nullptr };
	
	//animation data
	int m_nTestIndex = 5;
	CFbxAnimStackData				 m_AnimStackData;
	CAnimationData m_AnimationData;
	CSkeletonData m_SkeletonData;

	//vector<CFbxAnimationData>		 m_vAnimationData;
	//mesh data
	UINT m_MeshScale{ 1 };
	vector<CFbxMeshData>			 m_vMeshData;
	FbxArray<FbxString*>			 m_sAnimationNames;
	bool							 m_bHasAnimationData{ false };
	int m_iMeshCount{ 0 };

	//control point data
	int m_nSubsetMesh{0};
	//vector<XMFLOAT3> m_vPos;
	//vector<XMFLOAT3> m_vNor;
	//vector<XMFLOAT2> m_vUV;
	vector<int> m_vMaterialIndex;
	//vector<vector<CFbxBlendWeightPair>> m_vvBlendWeightPair;
	vector<stFBXControlPoint > m_vControlPoint;
	vector<UINT> m_vControlIndex;
	//control point data

	//helper func
	//void GetControlPointData(FbxMesh* pMesh, CFbxMeshData& MeshData);
	//void GetIndexData(FbxMesh* pMesh, CFbxMeshData& MeshData);
	//void GetNormalData(FbxMesh* pMesh, FbxGeometryElementNormal* vertexNormal, CFbxMeshData& MeshData);
	//void GetUVData(FbxMesh* pMesh, FbxGeometryElementUV* vertexUV, CFbxMeshData& MeshData);
	//void RegenerateMesh(FbxMesh* pMesh, CFbxMeshData& MeshData);

	//void RegenerateMesh(FbxMesh* pMesh, CFbxMeshData& MeshData){};
	void GetControlPointData(FbxMesh* pMesh);
	void GetIndexData(FbxMesh* pMesh);
	void GetNormalData(FbxMesh* pMesh, FbxGeometryElementNormal* vertexNormal);
	void GetUVData(FbxMesh* pMesh, FbxGeometryElementUV* vertexUV);
	void GetTData(FbxMesh* pMesh, FbxGeometryElementTangent* vertexTangent);
	void GetBData(FbxMesh* pMesh, FbxGeometryElementBinormal* vertexBiNormal);


	void RegenerateMesh(FbxMesh* pMesh, CFbxMeshData& MeshData);
public:
	XMMATRIX ConvertFbxMtxToXMMATRIX(FbxAMatrix FbxMtx) {
		return XMMATRIX{
			static_cast<float>(FbxMtx.Get(0,0)), static_cast<float>(FbxMtx.Get(0,1)), static_cast<float>(FbxMtx.Get(0,2)), static_cast<float>(FbxMtx.Get(0,3)),
			static_cast<float>(FbxMtx.Get(1,0)), static_cast<float>(FbxMtx.Get(1,1)), static_cast<float>(FbxMtx.Get(1,2)), static_cast<float>(FbxMtx.Get(1,3)),
			static_cast<float>(FbxMtx.Get(2,0)), static_cast<float>(FbxMtx.Get(2,1)), static_cast<float>(FbxMtx.Get(2,2)), static_cast<float>(FbxMtx.Get(2,3)),
			static_cast<float>(FbxMtx.Get(3,0)), static_cast<float>(FbxMtx.Get(3,1)), static_cast<float>(FbxMtx.Get(3,2)), static_cast<float>(FbxMtx.Get(3,3))
		};
	}
	//FBX객체 생성
	CFbxImporter();
	//생성한 FBX객체 소멸 
	virtual ~CFbxImporter();


};
