#include "stdafx.h"
#include "CoordinateSys.h"

void CCoordinateSys::SetCoordinateSysInfo(XMMATRIX mtx) {
	SetWorldMtx(mtx);
}

CCoordinateSys::CCoordinateSys() :CGameObject("coordinatesys") {

}

CCoordinateSys::~CCoordinateSys()
{
}
