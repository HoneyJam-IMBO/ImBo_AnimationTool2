#pragma once
#include "Mesh.h"

class CTerrainMesh : public CMesh {

public:
	//----------------------------dxobject-----------------------------
	//----------------------------dxobject-----------------------------

	//---------------------------mesh----------------------------------
	//begin func
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
	//begin func
	//---------------------------mesh----------------------------------
private:
	//----------------------vertex buffers---------------------------
	ID3D11Buffer* m_pd3dPositionBuffer{ nullptr };
	ID3D11Buffer* m_pd3dTexCoordBuffer{ nullptr };
	ID3D11Buffer* m_pd3dTBuffer{ nullptr };
	ID3D11Buffer* m_pd3dBBuffer{ nullptr };
	//----------------------vertex buffers---------------------------

public:
	CTerrainMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	~CTerrainMesh();
};