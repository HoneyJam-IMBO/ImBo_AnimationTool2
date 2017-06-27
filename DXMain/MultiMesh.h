#pragma once
#include "Mesh.h"
#include "StaticBuffer.h"

class CMultiMesh : public CMesh {
public:
	//----------------------------dxobject-----------------------------
	bool Begin(string path);
	virtual bool End();

	//global like Animation
	virtual void SetShaderState();
	virtual void CleanShaderState();
	//global like Animation
	//----------------------------dxobject-----------------------------

	//---------------------------mesh----------------------------------
	virtual void RenderExcute(UINT nInstance);


	//begin func
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
	//begin func

	//test
	//buffer
	CStaticBuffer* m_pAnimBuffer{ nullptr };

	//vertex buffer
	ID3D11Buffer* m_pd3dBoneIndexBuffer{ nullptr };
	ID3D11Buffer* m_pd3dWeightBuffer{ nullptr };

	//test
	XMFLOAT4* m_pxmf4BoneIndex;
	XMFLOAT3* m_pxmf3Weight;
	ID3D11RasterizerState* m_pPreRasterizerState{ nullptr };
	int m_nFrame{ 0 };
	int m_nTime{ 0 };

	ID3D11Buffer* m_pd3dPositionBuffer{ nullptr };
	ID3D11Buffer* m_pd3dNormalBuffer{ nullptr };
	ID3D11Buffer* m_pd3dUVBuffer{ nullptr };
	//test

private:
	vector<shared_ptr<CMesh>> m_vpMesh;
	int m_nMeshCnt{ 0 };

public:
	CMultiMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	~CMultiMesh();

};
