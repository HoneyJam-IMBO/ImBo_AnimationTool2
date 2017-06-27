#include "stdafx.h"
#include "RenderContainer.h"



//---------------------------dxobject---------------------------------
bool CRenderContainer::Begin() {
	if(false == m_vpBuffer.empty()){
		m_ppBufferData = new void*[m_vpBuffer.size()];
	}
	if (false == m_vpGlobalBuffer.empty()) {
		m_ppGlobalBufferData = new void*[m_vpGlobalBuffer.size()];
	}

	return true;
}
bool CRenderContainer::End() {
	m_lpObjects.clear();
	m_vpTexture.clear();
	m_vpBuffer.clear();
	m_vpMesh.clear();
	
	delete[] m_ppBufferData;
	delete[] m_ppGlobalBufferData;

	return true;
}
//---------------------------dxobject---------------------------------

//--------------------------container---------------------------------
void CRenderContainer::UpdateShaderState(shared_ptr<CCamera> pCamera) {
	m_pShader->UpdateShaderState();
	for (auto p : m_vpTexture) {
		p->UpdateShaderState();
	}
	for (auto p : m_vpMaterial) {
		p->UpdateShaderState();
	}
	for (auto p : m_vpBuffer) {
		p->UpdateShaderState();
	}
	
	//if (m_pGlobalBuffer) m_pGlobalBuffer->UpdateShaderState();
	//----------------------------update instance buffer--------------------------

	if (m_vpBuffer.empty()) return;

	int nInstance = 0;
	
	int nBuffer = 0;
	//map
	for (auto p : m_vpBuffer) {
		m_ppBufferData[nBuffer++] = p->Map();
	}
	
	for (auto pObject : m_lpObjects) {
		if (pObject->IsVisible(pCamera)) {
			DEBUGER->RegistToDebugRenderContainer(pObject);
			pObject->RegistToDebuger();
			pObject->SetBufferInfo(m_ppBufferData, nInstance, pCamera);
			nInstance++;
		}
	}

	m_nInstance = nInstance;
	
	//unmap
	for (auto p : m_vpBuffer) {
		p->Unmap();
	}
	//----------------------------update instance buffer--------------------------


}
void CRenderContainer::SetShaderState() {
	m_pShader->SetShaderState();
	for (auto p : m_vpTexture) {
		p->SetShaderState();
	}
	for (auto p : m_vpMaterial) {
		p->SetShaderState();
	}
	for (auto p : m_vpBuffer) {
		p->SetShaderState();
	}
	for (auto p : m_vpGlobalBuffer) {
		p->SetShaderState();
	}
	//if (m_pGlobalBuffer) m_pGlobalBuffer->SetShaderState();
	if (m_pAnimater)m_pAnimater->SetShaderState();

}

void CRenderContainer::RenderExcute() {
	for (auto p : m_vpMesh) {
		p->Render(m_nInstance);
	}
}
void CRenderContainer::RenderExcuteWithOutObject(){
	for (auto p : m_vpMesh) {
		p->Render(1);
	}
}
void CRenderContainer::CleanShaderState() {
	m_pShader->CleanShaderState();
	
	for (auto p : m_vpTexture) {
		p->CleanShaderState();
	}
	for (auto p : m_vpMaterial) {
		p->CleanShaderState();
	}
	for (auto p : m_vpBuffer) {
		p->CleanShaderState();
	}
	for (auto p : m_vpGlobalBuffer) {
		p->CleanShaderState();
	}

	if (m_pAnimater)m_pAnimater->CleanShaderState();
	//if (m_pGlobalBuffer) m_pGlobalBuffer->CleanShaderState();//global buffer
}


//--------------------------container 불변 함수---------------------------------
void CRenderContainer::Render(shared_ptr<CCamera> pCamera) {

	//shader State Update/ Instancing Buffet Update
	UpdateShaderState(pCamera);

	SetShaderState();

	//Render!
	RenderExcute();

	CleanShaderState();

}

void CRenderContainer::RenderWithOutObject(shared_ptr<CCamera> pCamera){
	//shader State Update/ Instancing Buffet Update
	UpdateShaderState(pCamera);

	SetShaderState();

	//Render!
	RenderExcuteWithOutObject();

	CleanShaderState();
}

void CRenderContainer::ClearMesh(){
	m_vpMesh.clear();
}

void CRenderContainer::ClearAnimater(){
	m_pAnimater = nullptr;
	//animater를 지울일이 없음.. 사용하는거지 내가 관리할 녀석ㅇ ㅣ아니거든..
}

void CRenderContainer::AddMesh(shared_ptr<CMesh> pMesh){
	if (!pMesh) return;

	m_vpMesh.push_back(pMesh);
}

//void CRenderContainer::SetMesh(shared_ptr<CMesh> pMesh, shader_value_num svn) {
//	if (!pMesh) return;
//
//	m_vpMesh[svn] = pMesh;
//
//}
//void CRenderContainer::SetShader(shared_ptr<CRenderShader> pShader, shader_value_num svn) {
//	if (!pShader) return;
//	
//	m_vpShader[svn] = pShader;
//
//}
void CRenderContainer::SetMesh(shared_ptr<CMesh> pMesh) {
	if (!pMesh) return;

	m_vpMesh.push_back(pMesh);
}
void CRenderContainer::SetShader(shared_ptr<CRenderShader> pShader) {
	if (!pShader)return;

	m_pShader = pShader;
}
void CRenderContainer::AddTexture(shared_ptr<CTexture> pTexture) {
	if (!pTexture)return;

	m_nTexture++;
	m_vpTexture.emplace_back(pTexture);
}

//-----------------------------------------buffer-----------------------------------------
void CRenderContainer::AddBuffer(shared_ptr<CBuffer> pBuffer){
	if (!pBuffer) return;

	//void**를 초기화 하기 위해 필요한놈
	m_nBuffer++;
	m_vpBuffer.emplace_back(pBuffer);
}
void CRenderContainer::AddInstanceBuffer(shared_ptr<CBuffer> pBuffer){
	if (!pBuffer) return;

	//void**를 초기화 하기 위해 필요한놈
	m_nBuffer++;
	m_vpBuffer.emplace_back(pBuffer);
	ID3D11Buffer* ppBuffers[1] = { pBuffer->GetBuffer() };
	UINT ppStrides[1] = { pBuffer->GetBufferStride() };
	UINT ppOffset[1] = { pBuffer->GetOffset() };

	for (auto p : m_vpMesh) {
		p->AssembleToVertexBuffer(1, ppBuffers, ppStrides, ppOffset);
	}
	
}
//-----------------------------------------buffer-----------------------------------------

//-----------------------------------------global buffer-----------------------------------------
void CRenderContainer::AddGlobalBuffer(shared_ptr<CGlobalBuffer> pGlobalBuffer) {
	if (!pGlobalBuffer) return;

	m_vpGlobalBuffer.emplace_back(pGlobalBuffer);
}
void CRenderContainer::UpdateGlobalBuffer() {
	if (m_vpGlobalBuffer.empty()) return;

	int nBuffer = 0;
	//map
	for (auto p : m_vpGlobalBuffer) {
		m_ppGlobalBufferData[nBuffer++] = p->Map();
	}
	//set data
	m_pGlobalObject->SetGlobalBufferInfo(m_ppGlobalBufferData);
	//unmap
	for (auto p : m_vpGlobalBuffer) {
		p->Unmap();
	}

}
//global buffer data는 globalObject에서 관리한다.
//void CRenderContainer::SetGlobalBufferData(void * pData){
//	m_pGlobalBuffer->SetData(pData);
//}
//-----------------------------------------global buffer-----------------------------------------

void CRenderContainer::AddMaterial(shared_ptr<CMaterial> pMaterial) {
	if (!pMaterial)return;

	m_nMaterial++;
	m_vpMaterial.emplace_back(pMaterial);
}

void CRenderContainer::SetObejcts(int n, CGameObject** ppObjects) {
	if (!ppObjects) return;

	for (int i = 0; i < n; ++i) {
		m_lpObjects.emplace_back(ppObjects[i]);
	}
}

void CRenderContainer::AddObject(CGameObject* pObject) {
	if (!pObject) return;

	m_lpObjects.emplace_back(pObject);
}
void CRenderContainer::RemoveObject(CGameObject* pObject) {
	if (!pObject) return;
	//if (0 == m_lpObjects.size()) return;

	m_lpObjects.remove_if([&pObject](CGameObject* pO) {
		return pObject == pO;
	});
}
//--------------------------container 불변 함수---------------------------------

CRenderContainer::CRenderContainer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : DXObject("rendercontainer", pd3dDevice, pd3dDeviceContext){
}
CRenderContainer::~CRenderContainer() {

}