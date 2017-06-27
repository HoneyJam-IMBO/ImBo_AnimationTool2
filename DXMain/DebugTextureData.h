#pragma once

#include "Object.h"


class CDebugTextureData : public CObject {
public:

public:
	ID3D11ShaderResourceView* m_pSRV{ nullptr };;
	XMFLOAT2 lt;
	XMFLOAT2 rb;
	
public:
	CDebugTextureData(ID3D11ShaderResourceView* pSRV, XMFLOAT2 lt, XMFLOAT2 rb) :
		m_pSRV(pSRV), lt(lt), rb(rb), CObject("debugtexturedata"){}
	
	virtual ~CDebugTextureData() {}
};