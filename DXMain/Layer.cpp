#include "stdafx.h"
#include "Layer.h"

CLayer::CLayer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : DXObject("layer", pd3dDevice, pd3dDeviceContext) {
}
CLayer::~CLayer() {

}

void CLayer::Render(shared_ptr<CCamera> pCamera) {
	UpdateShaderState(pCamera);
	SetShaderState(pCamera);
	RenderExcute(pCamera);
	CleanShaderState(pCamera);
}
