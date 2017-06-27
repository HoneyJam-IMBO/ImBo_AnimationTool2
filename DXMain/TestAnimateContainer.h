#pragma once

#include "AnimateContainer.h"
#include "TestObject.h"
#include "Core.h"
#include "WeaponPart.h"
#include "BoostPart.h"
#include "AdaptorPart.h"
#include "Plane.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "CapsuleLight.h"
#include "SpotLight.h"

#include "RenderContainerSeller.h"

class CTestAnimateContainer : public CAnimateContainer {
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

public:
	//animate container는 카메라 프러스텀 컬링을 진행하지 않으므로 camera가 필요 없다.
	CTestAnimateContainer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CTestAnimateContainer();


};
