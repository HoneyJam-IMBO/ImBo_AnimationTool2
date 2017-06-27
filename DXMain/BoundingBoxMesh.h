#pragma once
#include "Mesh.h"


class CBoundingBoxMesh : public CMesh {

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
	ID3D11Buffer* m_pd3dSizeBuffer{ nullptr };
	//----------------------vertex buffers---------------------------

public:
	CBoundingBoxMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CBoundingBoxMesh();
};