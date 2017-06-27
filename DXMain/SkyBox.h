#pragma once
#include "GameObject.h"
#include "Camera.h"

class CSkyBox : public CGameObject {

public:
	//----------------------------dxobject-----------------------------
	bool Begin();
	virtual bool End();
	//----------------------------dxobject-----------------------------

	//object
	virtual void Animate(float fTimeElapsed);

	//skybox
	void SetCamera(shared_ptr<CCamera> pCamera);

	virtual bool IsVisible(shared_ptr<CCamera> pCamera) { return true; }
private:
	shared_ptr<CCamera> m_pCamera;

public:
	CSkyBox();
	~CSkyBox();
};
