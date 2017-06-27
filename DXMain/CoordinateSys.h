#pragma once

#include "GameObject.h"

class CCoordinateSys : public CGameObject {
public:
	void SetCoordinateSysInfo(XMMATRIX mtx);

	virtual bool IsVisible(shared_ptr<CCamera> pCamera) { return true; };//계층구조의 녀석들은 다시 만들어줄 필요가 있음
private:

public:
	CCoordinateSys();
	~CCoordinateSys();
};
