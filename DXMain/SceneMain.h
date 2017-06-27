#pragma once

#include "DirectXFramework.h"
#include "Scene.h"

//layers
#include "LightLayer.h"
#include "ObjectLayer.h"
#include "PostProcessingLayer.h"

//seller
#include "RenderContainerSeller.h"

//object
#include "FreeCamera.h"
#include "ThirdPersonCamera.h"
#include "Bunny.h"
#include "TestCube.h"
//player
#include "Player.h"
//space
#include "SpaceContainer.h"
//terrain
#include "TerrainContainer.h"
//light
#include "PointLight.h"
#include "SpotLight.h"
#include "CapsuleLight.h"
#include "DirectionalLight.h"
#include <iostream>

static XMFLOAT4 xmf4DirectionalLightColor;

class CSceneMain;
struct LoadFileStruct {
	CSceneMain* m_pScene;
	string Filename;
};

class CSceneMain :public CScene{

public:
	//-----------------------------------scene--------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void Animate(float fTimeElapsed);
	//-----------------------render func-------------------
	virtual void ObjectRender();
	virtual void LightRender();
	//-----------------------render func-------------------

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void ProcessInput(float fTimeElapsed);


	CGameObject* PickObjectPointedByCursor(int xClient, int yClient);

	//animation tool을 위한 함수
	void CreateControllObject(string path);
	void CreateLoadFileUI();
	void AddFBXAnimationInfo(string path);
	void CreateAddInfoUI();
	void ClearAllFBXObject();

	void ClearAllFBXUI();

	void AutoWriteCallback();
	//변수
	vector<LoadFileStruct> m_LoadFileStruct;
	//fbx object
	CTestObject* m_pFBXObject{ nullptr };

	void SetMeshRoot(wstring wsMeshRoot);
private:
	int m_MeshCnt{ 0 };
	//framework
	CDirectXFramework* m_pFrameWork{ nullptr };

	//진짜 카메라는 framework에 있다.
	shared_ptr<CCamera> m_pCamera{ nullptr };

	//global object
	CSkyBox* m_pSkyBox{ nullptr };
	CDirectionalLight* m_pDirectionalLight{ nullptr };
	//global object

	//container seller
	//CRenderContainerSeller* m_RenderContainerSeller{ nullptr };
	//container seller

	//layers
	CLightLayer* m_pLightLayer{ nullptr };
	CObjectLayer* m_pObjectLayer{ nullptr };
	CTestCube* m_pRotationTestObject{ nullptr };
	//layers
	
	//space
	CSpaceContainer* m_pSpaceContainer{ nullptr };
	//space
	
	//terrain
	CTerrainContainer* m_pTerrainContainer{ nullptr };
	//terrain
	
	//player
	CPlayer* m_pPlayer{ nullptr };
	//player

	//---------------------------d3ddevice & hWnd------------------------
	ID3D11Device* m_pd3dDevice{ nullptr };
	ID3D11DeviceContext* m_pd3dDeviceContext{ nullptr };
	HWND m_hWnd{ nullptr };
	//---------------------------d3ddevice & hWnd------------------------
	
	
	//----------------------create func--------------------
	void CreateRenderLayers();
	//void SetForwardRenderTargets();
	//----------------------create func--------------------
	//---------------------------deferred rendering-----------------------

	//picking
	CGameObject* m_pPickingObject{ nullptr };
public:
	CSceneMain(CDirectXFramework* pFrameWork);
	~CSceneMain();

};