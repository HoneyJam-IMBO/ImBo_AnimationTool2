#include "stdafx.h"
#include "DebugTexture.h"

bool CDebugTexture::Begin()
{
	//m_objectID = OBJECT_DEBUG_TEXTURE;
	//CGameObject::Begin();
	return true;
}

void CDebugTexture::SetBufferInfo(void ** ppMappedResources, int & nInstance, shared_ptr<CCamera> pCamera) {
	//형변환
	VS_VB_DEBUG_TEXTURE_INSTANCE* pnInstances = (VS_VB_DEBUG_TEXTURE_INSTANCE*)ppMappedResources[0];

	//정보 주입
	pnInstances[nInstance].m_xmf2LeftTop = m_xmf2LeftTop;
	pnInstances[nInstance].m_xmf2RightBottom = m_xmf2RightBottom;
}

void CDebugTexture::SetTextureInfo(XMFLOAT2 lefttop, XMFLOAT2 rightbottom) {
	/*
	sp.x/w, sp.y/h
	ep.x/w, ep.y/w 
	하면  0과 1사이 값이 나올 것이고
	이것을 또
	*2 -1 하면 -1부터 1사이 값 즉 화면 좌표계로 변환한 것.
	
	여기서 y값은 우리가 생각하는 의미가 반대니까 
	*-1 해주면 0은0 1은 -1 -1은 1로 딱 반대가 된다.
	*/
	lefttop.x = lefttop.x / m_rcClient.right;
	lefttop.y = lefttop.y / m_rcClient.bottom;
	rightbottom.x = rightbottom.x / m_rcClient.right;
	rightbottom.y = rightbottom.y / m_rcClient.bottom;

	lefttop.x = lefttop.x * 2 - 1;
	lefttop.y = lefttop.y * 2 - 1;
	rightbottom.x = rightbottom.x * 2 - 1;
	rightbottom.y = rightbottom.y * 2 - 1;

	lefttop.y *= -1;
	rightbottom.y *= -1;

	m_xmf2LeftTop = lefttop;
	m_xmf2RightBottom = rightbottom;
}

CDebugTexture::CDebugTexture() :CGameObject("debugtexture") {

}

CDebugTexture::~CDebugTexture(){

}
