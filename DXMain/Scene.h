#pragma once

#include "DXObject.h"
#include "GameObject.h"

class CScene{

public:
	virtual bool Begin() { return true; };
	virtual bool End() { return true; };

	virtual void Animate(float fTimeElapsed) = 0;
	virtual void ObjectRender() = 0;
	virtual void LightRender() = 0;

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual void ProcessInput(float fTimeElapsed) = 0;

	virtual CGameObject* PickObjectPointedByCursor(int xClient, int yClient) = 0;

protected:
	string m_sName;
	//resource의 root dir는 assets이다. 추가
	wstring m_wsMeshRoot{ L"../../Assets" };

private:

public:
	CScene::CScene(string name) { m_sName = name; }
	~CScene() {}
};