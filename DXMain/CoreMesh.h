#pragma once
#include "Mesh.h"

class CCoreMesh : public CMesh {

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
	CCoreMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CCoreMesh();
};