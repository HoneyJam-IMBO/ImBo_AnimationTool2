#pragma once
#include "Mesh.h"

class CUseFBXMesh : public CMesh {

public:
	//----------------------------dxobject-----------------------------
	bool Begin(string path);
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState() { }
	//----------------------------dxobject-----------------------------

	//---------------------------mesh----------------------------------
	virtual void RenderExcute(UINT nnInstance);
	//begin func
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
	//begin func
	//---------------------------mesh----------------------------------
private:
	//fbx data
	XMFLOAT3* m_pNormals{ nullptr };
	int m_nNormals{ 0 };
	XMFLOAT2* m_pUVs{ nullptr };
	int m_nUVs{ 0 };
	//----------------------vertex buffers---------------------------
	ID3D11Buffer* m_pd3dPositionBuffer{ nullptr };
	ID3D11Buffer* m_pd3dNormalBuffer{ nullptr };
	ID3D11Buffer* m_pd3dTBuffer{ nullptr };
	ID3D11Buffer* m_pd3dBBuffer{ nullptr };
	ID3D11Buffer* m_pd3dUVBuffer{ nullptr };
	//----------------------vertex buffers---------------------------

	//fbx data
	FbxManager *m_manager{ nullptr };
	FbxIOSettings *m_ioSettings{ nullptr };
	
	FbxImporter *m_importer{ nullptr };
	FbxScene *m_scene{ nullptr };
	//node
	FbxNode* m_pRootNode{ nullptr };

	//helper func
	void GetPositionData(FbxNode* pNode);
	void GetNormalData(FbxNode* pNode);
	void GetUVData(FbxNode* pNode);
public:
	CUseFBXMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CUseFBXMesh();
};