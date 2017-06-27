#pragma once
#include "Mesh.h"

class CAdaptorMesh : public CMesh {

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
	ID3D11Buffer* m_pd3dColorBuffer{ nullptr };
	//----------------------vertex buffers---------------------------

public:
	CAdaptorMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CAdaptorMesh();
};