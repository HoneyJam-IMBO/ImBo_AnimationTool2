#pragma once
//#include "Camera.h"
#include "SingleTon.h"

#include "RenderContainer.h"
#include "DebugRenderContainer.h"

class CRenderContainerSeller : public CSingleTonBase<CRenderContainerSeller>{
public:
	bool Begin(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	bool End();

	CRenderContainer* GetRenderContainer(object_id objectid);

	//void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
private:
	//이게 진짜 rendercontainer
	mapRenderContainer m_mRenderContainer;

	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pd3dDeviceContext;
public:
	CRenderContainerSeller() : CSingleTonBase<CRenderContainerSeller>("inputmanagersingleton") {}

};
