#pragma once

#include "SingleTon.h"

class CGlobalValueManager : public CSingleTonBase<CGlobalValueManager> {

public:
	bool Begin(HINSTANCE hInst, HWND hWnd);
	bool End();

	//mode
	void SetWireFrameMode(bool b) { m_bWireFrame = b; }
	bool GetWireFrameMode() { return m_bWireFrame; }
	void SetDebugMode(bool b) { m_bDebug = b; }
	bool GetDebugMode() { return m_bDebug; }

	//wnd
	HINSTANCE GethInst() { return m_hInst; }
	HWND GethWnd() { return m_hWnd; }
	void SetrcClient(LONG right, LONG bottom) { m_rcClient.right = right; m_rcClient.bottom = bottom; }
	RECT GetrcClient() { return m_rcClient; }

	//device
	ID3D11Device* GetDevice() { return m_pd3dDevice; }
	ID3D11DeviceContext* GetDeviceContext() { return m_pd3dDeviceContext; }
	IDXGISwapChain* GetSwapChain() { return m_pdxgiSwapChain; }


	bool GetIsGJM() { return m_bGJM; }
	void SetIsGJM(bool b) { m_bGJM = b; }
private:
	bool m_bGJM{ false };
	//begin func
	//wnd
	void SethInst(HINSTANCE hInst) { m_hInst = hInst; }
	void SethWnd(HWND hWnd) { m_hWnd = hWnd; }
	//begin func

	//mode
	bool m_bWireFrame{ false };
	bool m_bDebug{ false };

	//wnd
	HINSTANCE m_hInst;
	HWND m_hWnd;
	RECT m_rcClient;

	//device
	IDXGISwapChain			*	m_pdxgiSwapChain{ nullptr };
	ID3D11Device			*	m_pd3dDevice{ nullptr };
	ID3D11DeviceContext		*	m_pd3dDeviceContext{ nullptr };


public:
	CGlobalValueManager();
	virtual ~CGlobalValueManager();

};
