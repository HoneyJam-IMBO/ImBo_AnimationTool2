#pragma once
#include "DXObject.h"



struct GBUFFER_UNPACKING_DATA {
	XMFLOAT4 PerspectiveValues;
	XMFLOAT4X4 ViewInv;
};

struct stCameraBuffer {
	XMFLOAT4X4 m_xmf4x4ViewProjection;
	//directional
	XMFLOAT4 m_CameraPos;
};

class CPlayer;

class CCamera :public DXObject{
public:
	//----------------------------dxobject-----------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void SetShaderState() {	};
	virtual void CleanShaderState() { }

	void UpdateShaderState();
	//----------------------------dxobject-----------------------------


	//flustum culling
	bool IsInFrustum(BoundingBox& boundingBox);
	//---------------------------------------카메라 조정---------------------------------
	//lookat
	void SetLookAt(XMVECTOR& xmvPos, XMVECTOR& xmvLookAt, XMVECTOR& xmvUp);
	//position 강제 지정
	virtual void SetPosition(XMVECTOR pos);
	//회전
	virtual void Rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f) = 0;
	//Ani 카메라 액션 = CObject의 Animate - Component사용
	

	//----------------------------------------자주 불릴 함수들------------------------
	//player따라다니기
	virtual void Update(float fTimeElapsed) = 0;
	virtual void ProcessInput(float fTimeElapsed) = 0;
	//----------------------------------------자주 불릴 함수들------------------------

	//debug모드시 카메라 그리기 
	virtual void Render() {};
	//world mtx 제작 후 반환 render할 때 사용 
	virtual XMMATRIX GetWorldMtx();
	//---------------------------------------카메라 조정---------------------------------


	

	//---------------------------------------카메라 버퍼 ------------------------------
	//projectionMtx생성
	void GenerateProjectionMatrix(float fFov, float fRatio, float fNear, float fFar);
	//viewmtx 갱신
	void UpdateViewMtx();
	//버퍼는 아니지만 viewport
	void SetViewport(DWORD xTopLeft, DWORD yTopLeft, DWORD nWidth, DWORD nHeight, float fMinZ, float fMaxZ);
	//---------------------------------------카메라 버퍼 ------------------------------

	//player
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	CPlayer* GetPlayer() { return m_pPlayer; }
	//player

	ID3D11Buffer* GetViewProjectionBuffer() { return m_pViewProjectionBuffer; }
	ID3D11Buffer* GetGBufferUnpackingBuffer() { return m_pGBufferUnpackingBuffer; }
	XMMATRIX GetViewMtx() { return XMLoadFloat4x4(&m_xmf4x4View); }
	XMMATRIX GetProjectionMtx() { return XMLoadFloat4x4(&m_xmf4x4Projection); }

	D3D11_VIEWPORT& GetViewport() { return m_d3dViewport; }

	RECT GetClientSize() { return m_rcClient; };
	XMVECTOR GetPosition() { return XMLoadFloat3(&m_xmf3Pos); }
protected:
	//aabb flustum
	BoundingFrustum m_OriBoundingFrustum;
	BoundingFrustum m_BoundingFrustum;

	//rect size
	RECT m_rcClient;
	//player
	CPlayer* m_pPlayer{ nullptr };

	//카메라 상수버퍼
	stCameraBuffer m_stCameraBufferData;
	ID3D11Buffer* m_pViewProjectionBuffer;
	XMFLOAT4X4 m_xmf4x4View;
	XMFLOAT4X4 m_xmf4x4Projection;

	//viewport
	D3D11_VIEWPORT m_d3dViewport;

	//카메라 4벡터 CObject의 worldmtx를 가지고 만든다.
	XMFLOAT3 m_xmf3Right;
	XMFLOAT3 m_xmf3Up;
	XMFLOAT3 m_xmf3Look;
	XMFLOAT3 m_xmf3Pos;

	//-----------------------------------deferred light test--------------------
	GBUFFER_UNPACKING_DATA m_GBufferUnpackingData;
	ID3D11Buffer* m_pGBufferUnpackingBuffer{ nullptr };
public:
	CCamera(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CCamera();

};