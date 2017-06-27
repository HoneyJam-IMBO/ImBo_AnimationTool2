#pragma once
#include "Mesh.h"

#define START_MESH_INDEX 0

class CFileBasedMesh : public CMesh {

public:
	//----------------------------dxobject-----------------------------
	bool Begin();
	virtual bool End();
	//----------------------------dxobject-----------------------------

	static shared_ptr<CFileBasedMesh> CreateMesh(wstring path, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext, UINT index, bool bHasAnimation = true);
	static shared_ptr<CFileBasedMesh> CreateMeshFromFBXFile(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext, UINT index, bool bHasAnimation = true);
	static shared_ptr<CFileBasedMesh> CreateMeshFromGJMFile(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext, UINT index, bool bHasAnimation = true);
	static shared_ptr<CFileBasedMesh> CreateMeshFromDotXFile(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext, UINT index, bool bHasAnimation = true);
	//---------------------------mesh----------------------------------
	//begin func
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
//	void ProcessGetVertexBuffer(FbxNode* pNode);
	//begin func
	void RenderExcute(UINT nInstance);
	//---------------------------mesh----------------------------------
	//get
	UINT GetMeshIndex() { return m_MeshIndex; }

	XMFLOAT3* GetNormals() { return m_pNormals; }
	XMFLOAT2* GetUVs() { return m_pUVs; }
	XMFLOAT3* GetTs() { return m_pTs; }
	XMFLOAT3* GetBs() { return m_pBs; }
	//animation data
	XMFLOAT4* GetJointIndices() { return m_pxmf4BoneIndex; }
	XMFLOAT3* GetWeights() { return m_pxmf3Weight; }

	//set
	void SetMeshIndex(UINT MeshIndex){ m_MeshIndex = MeshIndex; }
	//vertex data
	void SetpNormals(XMFLOAT3* pNormals) { m_pNormals = pNormals; }
	void SetpUVs(XMFLOAT2* pUVs) { m_pUVs = pUVs; }
	void SetpTs(XMFLOAT3* pTs) { m_pTs = pTs; }
	void SetpBs(XMFLOAT3* pBs) { m_pBs = pBs; }
	//animation
	void SetpBoneIndex(XMFLOAT4* pxmf4BoneIndex) { m_pxmf4BoneIndex = pxmf4BoneIndex; }
	void SetpWeights(XMFLOAT3* pxmf3Weight) { m_pxmf3Weight = pxmf3Weight; }
protected:

	//vertex data
	XMFLOAT3* m_pNormals{ nullptr };
	XMFLOAT2* m_pUVs{ nullptr };
	XMFLOAT3* m_pTs{ nullptr };
	XMFLOAT3* m_pBs{ nullptr };
	//animation
	XMFLOAT4* m_pxmf4BoneIndex{ nullptr };
	XMFLOAT3* m_pxmf3Weight{ nullptr };
	//vertex data
	//----------------------vertex buffers---------------------------
	ID3D11Buffer* m_pd3dPositionBuffer{ nullptr };
	ID3D11Buffer* m_pd3dNormalBuffer{ nullptr };
	ID3D11Buffer* m_pd3dUVBuffer{ nullptr };
	ID3D11Buffer* m_pd3dTBuffer{ nullptr };
	ID3D11Buffer* m_pd3dBBuffer{ nullptr };
	//animation
	ID3D11Buffer* m_pd3dBoneIndexBuffer{ nullptr };
	ID3D11Buffer* m_pd3dWeightBuffer{ nullptr };
	//----------------------vertex buffers---------------------------
private:
	//helper func
	//void GetPositionData(FbxMesh* pMesh);
	//void GetNormalData(FbxMesh* pMesh);
	//void GetUVData(FbxMesh* pMesh);
public:
	CFileBasedMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CFileBasedMesh();
};