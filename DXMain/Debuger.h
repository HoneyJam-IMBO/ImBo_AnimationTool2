#pragma once

#include "SingleTon.h"

#include "GameObject.h"
#include "BoundingBox.h"
#include "CoordinateSys.h"

#include "RenderContainer.h"
//#include "RenderContainerSeller.h"
#include "DirectXFramework.h"

//font
#include "../../Assets/import/FW1FontWrapper_1_1/FW1FontWrapper.h"
//debug data
#include "DebugFontData.h"
#include "DebugTextureData.h"
#include "DebugTexture.h"

class CDebuger : public CSingleTonBase<CDebuger>{

public:
	bool Begin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool End();

	void RegistToDebugRenderContainer(CGameObject* pObject);
	void RegistCoordinateSys(FXMMATRIX mtx);
	void RegistAABB(BoundingBox& aabb);
	void RegistOBB(BoundingOrientedBox& obb);

	void DebugRender(shared_ptr<CCamera> pCamera);
	
	void RenderAABB(shared_ptr<CCamera> pCamera);
	void RenderCoordinateSys(shared_ptr<CCamera> pCamera);
	void RenderLightVolume(shared_ptr<CCamera> pCamera);
	void ClearDebuger();

	void ResizeDisplay(RECT rc);
	//utill func
	void AddText(float fontSize, float posX, float posY, UINT32 color, TCHAR * msg, ...);
	void RenderText();
	void AddTexture(XMFLOAT2 fLeftTop, XMFLOAT2 fRightBottom, ID3D11ShaderResourceView* pSRV);

	void RenderTexture();

	//messagebox
	int DebugMessageBox(std::string _title, std::string _message);
	int DebugGMessageBox(TCHAR* title, TCHAR* message, ...);

private:
	RECT m_rcClient;

	ID3D11Device* m_pd3dDevice{ nullptr };
	ID3D11DeviceContext* m_pd3dDeviceContext{ nullptr };

	mapRenderContainer m_mDebugRenderContainer;
	
	ID3D11RasterizerState* m_pRasterizerState{ nullptr };

	//aabb 랜더를 위한 객체.
	CBoundingBox** m_ppBoundingBox{ nullptr };
	UINT m_nAABB{ 0 };
	//CoordinateSys 랜더를 위한 객체.
	CCoordinateSys** m_ppCoordinateSys{ nullptr };
	UINT m_nCoordinateSys{ 0 };

	//조명 계산을 위한 블랜드 state.
	ID3D11BlendState* m_pLightBlendState{ nullptr };
	ID3D11DepthStencilState* m_pLightDepthStencilState{ nullptr };
	ID3D11RasterizerState* m_pLightRasterizerState{ nullptr };
	//이전 상태 저장
	ID3D11BlendState* m_pPreBlendState{ nullptr };
	float* m_pPreBlendFactor{ nullptr };
	UINT m_PreSampleMask{ 0 };
	ID3D11DepthStencilState* m_pPreDepthStencilState{ nullptr };
	UINT m_PreStencilRef{ 0 };
	ID3D11RasterizerState* m_pPreRasterizerState{ nullptr };

//font
	IFW1Factory*								 m_pFW1FontFactory{ nullptr };

	IFW1FontWrapper*							m_pFW1Font{ nullptr };
	IFW1TextRenderer*							 m_pFWTextRender{ nullptr };
	IFW1GlyphProvider*							 m_pFW1Glyphrovider{ nullptr };

	queue<CDebugFontData>						m_qDebugFontData;
	UINT m_pivot{ FW1_TOP | FW1_LEFT };

//debug texture
	CDebugTexture* m_pDebugTextureObj{ nullptr };
	shared_ptr<CSampler> m_pDebugTextureSampler{ nullptr };
	shared_ptr<CTexture> m_pDebugTexture{ nullptr };

	queue<CDebugTextureData>					m_qDebugTextureData;
public:
	CDebuger();
	virtual ~CDebuger();


};
