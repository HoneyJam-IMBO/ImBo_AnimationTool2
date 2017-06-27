#pragma once
#include "DXObject.h"
#include "GameObject.h"
#include "RenderContainerSeller.h"

class CAnimateContainer : public DXObject{
public:
	//---------------------------dxobject---------------------------------
	virtual bool Begin(CRenderContainerSeller* pSeller);
	virtual bool End();

	virtual void SetShaderState() {};
	virtual void CleanShaderState() {};

	virtual void UpdateShaderState() {};
	//---------------------------dxobject---------------------------------

	//--------------------------animate container---------------------------------
	//animate 검사 후 해당 컨테이너에서 사라지는 객체는 ExcludeContainer에 집어 넣는다.
	virtual void Animate(float fTimeElapsed, CAnimateContainer* pExcludeContainer);

	void SetObejcts(int n, CGameObject** ppObjects);
	void AddObject(CGameObject* pObject);
	void RemoveObject(CGameObject* pObject);
	list<CGameObject*>& GetObjectList() { return m_lpObjects; }
	void ClearObjectList() { m_lpObjects.clear(); }//그림 다 그리고 clear
	//--------------------------animate container---------------------------------

protected:

	//--------------------------관리 대상------------------------------
	//컨테이너의 객체들은 동적으로 변할 것이다. 그러므로 리스트로 관리하는 것이 맞다
	list<CGameObject*> m_lpObjects;
	//--------------------------관리 대상------------------------------

public:
	//animate container는 카메라 프러스텀 컬링을 진행하지 않으므로 camera가 필요 없다.
	CAnimateContainer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CAnimateContainer();


};