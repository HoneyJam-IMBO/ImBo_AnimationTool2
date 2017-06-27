#pragma once

#include "GameObject.h"


struct VS_VB_DEBUG_TEXTURE_INSTANCE {
	XMFLOAT2 m_xmf2LeftTop;
	XMFLOAT2 m_xmf2RightBottom;
};

class CDebugTexture : public CGameObject {
public:
	bool Begin();
	virtual bool End() { return CGameObject::End(); };

	//buffer controll
	virtual void SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera);

	void SetTextureInfo(XMFLOAT2 lefttop, XMFLOAT2 rightbottom);
	
	virtual bool IsVisible(shared_ptr<CCamera> pCamera) { return true; };//계층구조의 녀석들은 다시 만들어줄 필요가 있음
	void ResizeDisplay(RECT rc) { m_rcClient = rc; }

private:
	XMFLOAT2 m_xmf2LeftTop;
	XMFLOAT2 m_xmf2RightBottom;
	RECT m_rcClient;

public:
	CDebugTexture();
	~CDebugTexture();
};
