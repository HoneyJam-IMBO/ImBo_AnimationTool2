#include "stdafx.h"
#include "GameObject.h"

#include "RenderContainer.h"
#include "RenderContainerSeller.h"
#include "FileBasedMesh.h"
#include "TerrainContainer.h"
#include "Layer.h"
#include "Animater.h"

//SetSelectMeshCallback
void TW_CALL SetSelectMeshCallback(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	float index = *static_cast<const float*>(value);
	pObj->SetSelectMeshIndex(index);
	pObj->CreateMenuMeshTextureUI();
	for (int i = 0; i < pObj->GetRenderContainer()->GetvMesh().size(); ++i) {
		if (i == pObj->GetSelectMeshIndex()) {
			pObj->GetRenderContainer()->GetMesh(i)->SetMeshMaterial(RESOURCEMGR->GetMaterial("RED"));
			continue;
		}
		pObj->GetRenderContainer()->GetMesh(i)->SetMeshMaterial(RESOURCEMGR->GetMaterial("DEFAULT"));
	}
}
void TW_CALL GetSelectMeshCallback(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	*static_cast<float *>(value) = pObj->GetSelectMeshIndex();
	float x = *static_cast<float *>(value);
}

void TW_CALL SetMeshTextureButtonCallback(void * clientData) {
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	pObj->CreateMenuMeshTextureUI();

	//make texture/ set texture
	//pObj->GetRenderContainer()->GetMesh(pObj->GetSelectMeshIndex());
}
void TW_CALL SetDynamicTagButtonCallback(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	if(*static_cast<const bool *>(value)){
		pObj->SetTag(tag::TAG_DYNAMIC_OBJECT);
	}
}
void TW_CALL GetDynamicTagButtonCallback(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	*static_cast<bool *>(value) = (tag::TAG_DYNAMIC_OBJECT == pObj->GetTag());
}
void TW_CALL SetStaticTagButtonCallback(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	if (*static_cast<const bool *>(value)) {
		pObj->SetTag(tag::TAG_STATIC_OBJECT);
	}
}
void TW_CALL GetStaticTagButtonCallback(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	*static_cast<bool *>(value) = (tag::TAG_STATIC_OBJECT == pObj->GetTag());
}

void TW_CALL SetStaticTagButtonCallback(void* clientData) {
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	pObj->GetRenderContainer()->GetMesh(0)->SetTag(tag::TAG_STATIC_OBJECT);
}
/////////////////////
void TW_CALL GetDefaultTexButtonCallback(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	*static_cast<bool *>(value) = (0 == pObj->GetMeshMode());
}

void TW_CALL SetDefaultTexButtonCallback(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	if (*static_cast<const bool *>(value)) {
		pObj->SetMeshMode(0);
		pObj->CreateMenuMeshTextureUI();
	}

}

void TW_CALL GetSpecTexButtonCallback(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	*static_cast<bool *>(value) = (1 == pObj->GetMeshMode());
}

void TW_CALL SetSpecTexButtonCallback(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	if (*static_cast<const bool *>(value)) {
		pObj->SetMeshMode(1);
		pObj->CreateMenuMeshTextureUI();

	}
}
void TW_CALL GetCPTexButtonCallback(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	*static_cast<bool *>(value) = (2 == pObj->GetMeshMode());
}

void TW_CALL SetCPTexButtonCallback(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	if (*static_cast<const bool *>(value)) {
		pObj->SetMeshMode(2);
		pObj->CreateMenuMeshTextureUI();

	}
}
/////////////////////////
void TW_CALL LoadTextureFileCallback(void* clientData) {
	/*
	pSampler = make_shared<CSampler>(m_pd3dDevice, m_pd3dDeviceContext);
	pSampler->Begin(PS_TEXTURE_SAMPLER, BIND_PS);
	m_mSampler.insert(pairSampler("DEFAULT", pSampler));
	UINT DefaultSlot = { PS_TEXTURE };
	UINT DefaultFlag = { BIND_PS };
	D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/default.jpg"), NULL, NULL, &pd3dSRV, NULL);
	pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
	pTexture->Begin(pd3dSRV, pSampler, DefaultSlot, DefaultFlag);
	m_mTexture.insert(pairTexture("DEFAULT", pTexture));
	*/
	StructLoadTextureFile* pData = reinterpret_cast<StructLoadTextureFile*>(clientData);
	//string path = "../inputdata/";
	//path += pData->m_sName;
	wstring wPath{ L"" };
	wPath.assign(pData->m_sName.cbegin(), pData->m_sName.cend());

	char name[64];
	if (0 == pData->m_indexSelectTexture) {
		sprintf(name, "TestDefault%d", dynamic_cast<CFileBasedMesh*>(pData->m_pMesh.get())->GetMeshIndex());
		pData->m_pMesh->SetMeshTexture(pData->m_indexSelectTexture, 
			RESOURCEMGR->CreateTexture(name, wPath.c_str(), RESOURCEMGR->GetSampler("DEFAULT"), 0));
	}
	else if(1 == pData->m_indexSelectTexture) {
		sprintf(name, "TestSpec%d", dynamic_cast<CFileBasedMesh*>(pData->m_pMesh.get())->GetMeshIndex());
		pData->m_pMesh->SetMeshTexture(pData->m_indexSelectTexture, 
			RESOURCEMGR->CreateTexture(name, wPath.c_str(), RESOURCEMGR->GetSampler("DEFAULT"), 1));		
	}
	else if (2 == pData->m_indexSelectTexture) {
		sprintf(name, "TestCP%d", dynamic_cast<CFileBasedMesh*>(pData->m_pMesh.get())->GetMeshIndex());
		pData->m_pMesh->SetMeshTexture(pData->m_indexSelectTexture, 
			RESOURCEMGR->CreateTexture(name, wPath.c_str(), RESOURCEMGR->GetSampler("DEFAULT"), 2));
	}


	pData->m_pMesh->SetMeshMaterial(RESOURCEMGR->GetMaterial("DEFAULT"));
}
bool CGameObject::Begin() {

	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());

	XMStoreFloat4x4(&m_xmf4x4MeshOffset, XMMatrixIdentity());

	m_pRenderContainer = RCSELLER->GetRenderContainer(m_objectID);
	//if (m_pRenderContainer->GetvMesh().size() > 0)//mesh가 있으면
	//{//aabb 해당 mesh에서 aabb를 얻어온다.
		m_OriBoundingBox = m_pRenderContainer->GetMesh()->GetAABB();
	//}
	//else {//없으면 최대 최소 aabb를 얻어온다.
	BoundingBox::CreateFromPoints(m_OriBoundingBox, XMVectorSet(+100.f, +100.f, +100.f, 0.f), XMVectorSet(-100.f, -100.f, -100.f, 0.f));
	//}

	if (m_pRenderContainer->GetAnimater()){
		m_pAnimater = m_pRenderContainer->GetAnimater();
		m_xmf4x4MeshOffset = m_pAnimater->GetMeshOffsetMtx();
	}

	XMStoreFloat4(&m_xmf4Quaternion, XMQuaternionIdentity());
	return true;
}
bool CGameObject::End() {

	//-------------------------------component---------------------------
	ClearComponents();
	//-------------------------------component---------------------------
	return true;
}

void CGameObject::Animate(float fTimeElapsed) {
	if (m_pAnimater) {
		//obb animate
		for (auto OBB : m_pAnimater->GetAnimationInfo()->GetActiveBoundingBox()) {
			//XMMATRIX xmmtxOffset = XMLoadFloat4x4(&m_pAnimater->GetMeshOffsetMtx());
			BoundingOrientedBox obb;
			OBB.Transform(obb, GetWorldMtx());
			DEBUGER->RegistOBB(obb);
		}
	}

	DEBUGER->RegistCoordinateSys(GetWorldMtx());

	//모든 컴포넌트를 돌면서 Update실행
	for (auto i : m_mapComponents) {
		i.second->Update(fTimeElapsed);
	}
}
void CGameObject::Move(XMVECTOR xmvDir, float fDistance) {

	XMFLOAT3 xmfDir;
	XMStoreFloat3(&xmfDir, XMVector3Normalize(xmvDir)*fDistance);

	m_xmf4x4World._41 += xmfDir.x;
	m_xmf4x4World._42 += xmfDir.y;
	m_xmf4x4World._43 += xmfDir.z;
}
void CGameObject::Rotate(XMMATRIX xmMtx) {
	XMMATRIX xmmtxRotate = XMMatrixMultiply(xmMtx, XMLoadFloat4x4(&m_xmf4x4World));
	XMStoreFloat4x4(&m_xmf4x4World, xmmtxRotate);
	SetQuaternion(XMQuaternionRotationMatrix(GetWorldMtx()));
}
void CGameObject::Rotate(float x, float y, float z) {
	XMMATRIX xmmtxRotate;
	if (x != 0.0f)
	{
		xmmtxRotate = XMMatrixRotationAxis(GetRight(), (float)XMConvertToRadians(x));
		XMStoreFloat4x4(&m_xmf4x4World, XMMatrixMultiply(xmmtxRotate, XMLoadFloat4x4(&m_xmf4x4World)));
		//SetRotationQuaternion(XMQuaternionRotationAxis(GetRight(), x));
	}
	if (y != 0.0f)
	{
		//플레이어의 로컬 y-축을 기준으로 회전하는 행렬을 생성한다.
		xmmtxRotate = XMMatrixRotationAxis(GetUp(), (float)XMConvertToRadians(y));
		XMStoreFloat4x4(&m_xmf4x4World, XMMatrixMultiply(xmmtxRotate, XMLoadFloat4x4(&m_xmf4x4World)));
		//SetRotationQuaternion(XMQuaternionRotationAxis(GetUp(), y));
	}
	if (z != 0.0f)
	{
		//플레이어의 로컬 z-축을 기준으로 회전하는 행렬을 생성한다.
		xmmtxRotate = XMMatrixRotationAxis(GetLook(), (float)XMConvertToRadians(z));
		XMStoreFloat4x4(&m_xmf4x4World, XMMatrixMultiply(xmmtxRotate, XMLoadFloat4x4(&m_xmf4x4World)));
		//SetRotationQuaternion(XMQuaternionRotationAxis(GetLook(), z));
	}
	
	SetQuaternion(XMQuaternionRotationMatrix(GetWorldMtx()));
}

void CGameObject::RotateWorldAxis(float x, float y, float z) {
	XMMATRIX xmmtxRotate;
	if (x != 0.0f)
	{
		xmmtxRotate = XMMatrixRotationAxis(XMVectorSet(1.f,0.f,0.f,0.f), (float)XMConvertToRadians(x));
		XMStoreFloat4x4(&m_xmf4x4World, XMMatrixMultiply(xmmtxRotate, XMLoadFloat4x4(&m_xmf4x4World)));
		//SetRotationQuaternion(XMQuaternionRotationAxis(GetRight(), x));
	}
	if (y != 0.0f)
	{
		//플레이어의 로컬 y-축을 기준으로 회전하는 행렬을 생성한다.
		xmmtxRotate = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), (float)XMConvertToRadians(y));
		XMStoreFloat4x4(&m_xmf4x4World, XMMatrixMultiply(xmmtxRotate, XMLoadFloat4x4(&m_xmf4x4World)));
		//SetRotationQuaternion(XMQuaternionRotationAxis(GetUp(), y));
	}
	if (z != 0.0f)
	{
		//플레이어의 로컬 z-축을 기준으로 회전하는 행렬을 생성한다.
		xmmtxRotate = XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), (float)XMConvertToRadians(z));
		XMStoreFloat4x4(&m_xmf4x4World, XMMatrixMultiply(xmmtxRotate, XMLoadFloat4x4(&m_xmf4x4World)));
		//SetRotationQuaternion(XMQuaternionRotationAxis(GetLook(), z));
	}

	m_xmf3Rotate = XMFLOAT3{ m_xmf3Rotate.x + x, m_xmf3Rotate.y + y, m_xmf3Rotate.z + z };
	SetQuaternion(XMQuaternionRotationMatrix(GetWorldMtx()));
}
void CGameObject::SetPosition(XMVECTOR pos) {
	XMStoreFloat3(&m_xmf3Position, pos);

	m_xmf4x4World._41 = m_xmf3Position.x;
	m_xmf4x4World._42 = m_xmf3Position.y;
	m_xmf4x4World._43 = m_xmf3Position.z;
	if (m_pTerrainContainer) {
		m_xmf4x4World._42 = GetTerrainHeight();
		m_xmf3Position.y = GetTerrainHeight();
	}
}
void CGameObject::SetPositionX(const float pos){
	m_xmf4x4World._41 = pos;
	m_xmf3Position.x = pos;
}
void CGameObject::SetPositionY(const float pos) {
	m_xmf4x4World._42 = pos;
	m_xmf3Position.y = pos;
}
void CGameObject::SetPositionZ(const float pos) {
	m_xmf4x4World._43 = pos;
	m_xmf3Position.z = pos;
}
void CGameObject::SetWorldMtx(XMMATRIX mtxWorld) {
	XMStoreFloat4x4(&m_xmf4x4World, mtxWorld);
}

void CGameObject::SetScale(XMVECTOR xmv){
	XMStoreFloat4(&m_xmf4Scale, xmv);
	SetWorldMtx(XMMatrixAffineTransformation(GetScale(), XMQuaternionIdentity(), GetQuaternion(), GetPosition()));
}

void CGameObject::SetRotation(XMMATRIX mtxRotation){
	XMFLOAT4X4 xmf4x4;
	XMStoreFloat4x4(&xmf4x4, mtxRotation);
	m_xmf4x4World._11 = xmf4x4._11, m_xmf4x4World._12 = xmf4x4._12, m_xmf4x4World._13 = xmf4x4._13;
	m_xmf4x4World._21 = xmf4x4._21, m_xmf4x4World._22 = xmf4x4._22, m_xmf4x4World._23 = xmf4x4._23;
	m_xmf4x4World._31 = xmf4x4._31, m_xmf4x4World._32 = xmf4x4._32, m_xmf4x4World._33 = xmf4x4._33;

}

void CGameObject::SetRight(XMVECTOR xmvRight) {
	XMFLOAT3 xmfRight;
	XMStoreFloat3(&xmfRight, xmvRight);
	m_xmf4x4World._11 = xmfRight.x;
	m_xmf4x4World._12 = xmfRight.y;
	m_xmf4x4World._13 = xmfRight.z;
}
void CGameObject::SetUp(XMVECTOR xmvUp) {
	XMFLOAT3 xmfUp;
	XMStoreFloat3(&xmfUp, xmvUp);
	m_xmf4x4World._21 = xmfUp.x;
	m_xmf4x4World._22 = xmfUp.y;
	m_xmf4x4World._23 = xmfUp.z;
}
void CGameObject::SetLook(XMVECTOR xmvLook) {
	XMFLOAT3 xmfLook;
	XMStoreFloat3(&xmfLook, xmvLook);
	m_xmf4x4World._31 = xmfLook.x;
	m_xmf4x4World._32 = xmfLook.y;
	m_xmf4x4World._33 = xmfLook.z;
}
//--------------------------------getter--------------------------------
XMVECTOR CGameObject::GetRight() {
	XMFLOAT3 xmvPos;

	xmvPos.x = m_xmf4x4World._11;
	xmvPos.y = m_xmf4x4World._12;
	xmvPos.z = m_xmf4x4World._13;

	return XMLoadFloat3(&xmvPos);
}
XMVECTOR CGameObject::GetUp() {
	XMFLOAT3 xmvPos;

	xmvPos.x = m_xmf4x4World._21;
	xmvPos.y = m_xmf4x4World._22;
	xmvPos.z = m_xmf4x4World._23;

	return XMLoadFloat3(&xmvPos);
}
XMVECTOR CGameObject::GetLook() {
	XMFLOAT3 xmvPos;

	xmvPos.x = m_xmf4x4World._31;
	xmvPos.y = m_xmf4x4World._32;
	xmvPos.z = m_xmf4x4World._33;

	return XMLoadFloat3(&xmvPos);
}
XMVECTOR CGameObject::GetPosition() {
	XMFLOAT3 xmvPos;

	xmvPos.x = m_xmf4x4World._41;
	xmvPos.y = m_xmf4x4World._42;
	xmvPos.z = m_xmf4x4World._43;

	return XMLoadFloat3(&xmvPos);
}
XMMATRIX CGameObject::GetWorldMtx() {
	return XMLoadFloat4x4(&m_xmf4x4World);
}
//--------------------------------getter--------------------------------

//-------------------------------componenet----------------------------
//같은 Family의 component는 set할 수 없다. 
bool CGameObject::SetComponent(CComponent* pComponenet) {
	if (!pComponenet) return false;

	//component id검사 이상한 id이면 패스
	//family검사도 해준다.
	for (int i = 0; i < COMPONENTID_END; ++i) {
		if (i == pComponenet->GetID()) {
			for (auto data : m_mapComponents) {
				//family id가 같은것이 있다면 안넣어 줌
				if (data.second->GetFamilyID() == pComponenet->GetFamilyID())
					return false;
			}

			//최종적으로 검사 다 끝나면 넣어줌
			m_mapComponents.insert(pairComponent(pComponenet->GetID(), pComponenet));
			pComponenet->SetOwner(this);
			return true;
		}
	}

	return false;
}

//Get
CComponent* CGameObject::GetComponenet(const component_id& componenetID) {
	mapComponent::const_iterator  find_iterator = m_mapComponents.find(componenetID);
	return find_iterator->second;
}
const CComponent* CGameObject::GetComponenet(const component_id& componenetID)const {
	mapComponent::const_iterator  find_iterator = m_mapComponents.find(componenetID);
	return find_iterator->second;
}

//Clear
void CGameObject::ClearComponents() {
	//모든 컴포넌트를 돌면서 Update실행
	for (auto i : m_mapComponents) {
		i.second->End();
		delete i.second;
	}
	m_mapComponents.clear();
}
//-------------------------------componenet----------------------------

//instance buffer controll base
void CGameObject::SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera) {
	//형변환
	VS_VB_INSTANCE *pnInstances = (VS_VB_INSTANCE *)ppMappedResources[0];
	
	//transpose 이후 정보 주입
	if (m_pAnimater) {
		XMMATRIX xmmtx = XMLoadFloat4x4(&m_pAnimater->GetMeshOffsetMtx());;
		pnInstances[nInstance].m_xmmtxWorld = XMMatrixTranspose(GetWorldMtx()*xmmtx);
	}
	else {
		pnInstances[nInstance].m_xmmtxWorld = XMMatrixTranspose(GetWorldMtx());// *GetMeshOffsetMtx());
	}
	//

}
void CGameObject::RegistToContainer() {
	m_pRenderContainer->AddObject(this);
}

void CGameObject::RegistToDebuger(){
	BoundingBox BoundingBox;
	GetMainBoundingBox(BoundingBox);
	DEBUGER->RegistAABB(BoundingBox);
}

//void CGameObject::SetRenderContainer(CRenderContainerSeller * pSeller) {
//	m_pRenderContainer = pSeller->GetRenderContainer(m_objectID);
//	if (m_pRenderContainer->GetMesh())//mesh가 있으면
//	{//aabb 해당 mesh에서 aabb를 얻어온다.
//		m_OriBoundingBox = m_pRenderContainer->GetMesh()->GetBoundingBox();
//	}
//	else {//없으면 최대 최소 aabb를 얻어온다.
//		BoundingBox::CreateFromPoints(m_OriBoundingBox, XMLoadFloat3(&XMFLOAT3(+FLT_MAX, +FLT_MAX, +FLT_MAX)), XMLoadFloat3(&XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX)));
//	}
//}
float CGameObject::GetTerrainHeight(){

	return m_pTerrainContainer->GetHeight(GetPosition());
//	return 100.0f;
}
//flustum culling
bool CGameObject::IsVisible(shared_ptr<CCamera> pCamera){
	BoundingBox BoundingBox;

	m_bIsVisible = (m_bActive) ? true : false;
	if (m_bActive){
		GetMainBoundingBox(BoundingBox);
		if (pCamera) m_bIsVisible = pCamera->IsInFrustum(BoundingBox);
	}
	return(m_bIsVisible);
}

void CGameObject::GetMainBoundingBox(BoundingBox& out){
	out = m_pRenderContainer->GetMesh()->GetAABB();
	if (m_pAnimater) {
		out = m_pAnimater->GetMainAABB()->GetAABB();
	}

	out.Transform(out, GetMeshOffsetMtx() * GetWorldMtx());
}

bool CGameObject::CheckPickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float & distance){
	BoundingBox BoundingBox;
	GetMainBoundingBox(BoundingBox);
	return BoundingBox.Intersects(xmvWorldCameraStartPos, xmvRayDir, distance);
}

void CGameObject::PickingProc(){
	CreateObjectUI();

	CreateMeshUI();


	if (m_pAnimater) {
		m_pAnimater->CreateAnimationUI();
	}
	else {
		m_pRenderContainer->GetMesh()->CreateMeshUI();
		m_pRenderContainer->GetMesh()->CreateOBBUI();
	}
}

void CGameObject::CreateObjectUI(){
	const char* barName = "PickingBar";
	TWBARMGR->DeleteBar(barName);
	TWBARMGR->AddBar(barName);
	//set param
	TWBARMGR->SetBarSize(barName, 250, 200);
	TWBARMGR->SetBarPosition(barName, 0, 0);
	TWBARMGR->SetBarColor(barName, 255, 0, 0);
	TWBARMGR->SetBarContained(barName, true);
	TWBARMGR->SetBarMovable(barName, false);
	TWBARMGR->SetBarResizable(barName, false);
	//set param
	TWBARMGR->AddRotationMinMaxBar(barName, "Rotation World", "Rotate", this);
	TWBARMGR->AddPositionBar(barName, "Position", "Position", this, 0.f, SPACE_SIZE - 1.0f, 1.0f);
	TWBARMGR->AddScaleBar(barName, "Scale", "Scale", this, 0.1f, 100.f, 0.1f);
	
}

void CGameObject::CreateMeshUI(){
	const char* barName = "MeshInfo";
	TWBARMGR->DeleteBar(barName);
	TWBARMGR->AddBar(barName);
	//set param
	TWBARMGR->SetBarSize(barName, 250, 200);
	TWBARMGR->SetBarPosition(barName, 750, 0);
	TWBARMGR->SetBarColor(barName, 255, 0, 255);
	TWBARMGR->SetBarContained(barName, true);
	TWBARMGR->SetBarMovable(barName, false);
	TWBARMGR->SetBarResizable(barName, false);
	//set param
	
	TWBARMGR->AddBoolBarCB(barName, "MODE", "DEFAULT", SetDefaultTexButtonCallback, GetDefaultTexButtonCallback, this);
	TWBARMGR->AddBoolBarCB(barName, "MODE", "SPEC", SetSpecTexButtonCallback, GetSpecTexButtonCallback, this);
	TWBARMGR->AddBoolBarCB(barName, "MODE", "CP", SetCPTexButtonCallback, GetCPTexButtonCallback, this);

	TWBARMGR->AddMinMaxBarCB(barName, "SelectMesh", "SelectMeshIndex",
		SetSelectMeshCallback, GetSelectMeshCallback, this, 0.f, m_pRenderContainer->GetvMesh().size() - 1, 1.f);
	//TWBARMGR->AddButtonCB(barName, "SET_TAG", "DYNAMIC", SetDynamicTagButtonCallback, this);
	TWBARMGR->AddBoolBarCB(barName, "SET_TAG", "DYNAMIC", SetDynamicTagButtonCallback, GetDynamicTagButtonCallback, this);
	TWBARMGR->AddBoolBarCB(barName, "SET_TAG", "STATIC", SetStaticTagButtonCallback, GetStaticTagButtonCallback, this);
	//TWBARMGR->AddButtonCB(barName, "SET_TAG", "STATIC", SetStaticTagButtonCallback, this);
//	TWBARMGR->AddButtonCB(barName, "SetSelect", "MeshTexture", SetMeshTextureButtonCallback, this);
	//char menuName[64];
	//int i{ 0 };
	//for (auto pMesh : m_pRenderContainer->GetvMesh()) {
	//	sprintf(menuName, "Mesh%d", i);
	//	TWBARMGR->AddButtonCB(barName, "SetTextureButton", menuName, SetMeshTextureButtonCallback, m_pRenderContainer->GetMesh(i++).get());
	//}
}

void CGameObject::CreateMenuMeshTextureUI(){

	const char* barName{ "LoadTextureFile" };
	TWBARMGR->AddBar(barName);
	//set param
	TWBARMGR->SetBarSize(barName, 500, 400);
	TWBARMGR->SetBarContained(barName, true);;
	//set param

	vector<wstring> vFile;
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets", true, true, L".jpg");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets", true, true, L".JPG");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets", true, true, L".png");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets", true, true, L".PNG");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets", true, true, L".tga");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets", true, true, L".TGA");

	int cnt{ 0 };
	m_vStructLoadTextureFile.resize(vFile.size());
	for (auto data : vFile) {
		//file directory store;
		data = DIRECTORYFINDER->ReplaceString(data, L"\\", L"/");
		string filsDirectory{ "" };
		filsDirectory.assign(data.cbegin(), data.cend());
		m_vStructLoadTextureFile[cnt].m_pMesh = m_pRenderContainer->GetMesh(m_indexSelectMesh);
		m_vStructLoadTextureFile[cnt].m_sName = filsDirectory;
		m_vStructLoadTextureFile[cnt].m_indexSelectTexture = m_indexSelectTexture;

		//menu name = file name
		string menuNameString{ "" };
		menuNameString.assign(data.cbegin(), data.cend());

		//group name = directory name
		data = DIRECTORYFINDER->ReplaceString(data, L"/", L"\\");
		LPWSTR str = (LPWSTR)data.c_str();
		PathRemoveFileSpec(str);

		wstring wGroupName{ str };
		string groupName;
		groupName.assign(wGroupName.cbegin(), wGroupName.cend());
		TWBARMGR->AddButtonCB(barName, groupName.c_str(), menuNameString.c_str(), LoadTextureFileCallback, &m_vStructLoadTextureFile[cnt]);
		cnt++;
	}

}

void CGameObject::SetMeshMode(int mode){
	m_indexSelectTexture = mode;
	CreateMenuMeshTextureUI();
}


//생성자는 위에서부터 
CGameObject::CGameObject(string name, tag t) : CObject(name, t) {
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
}
CGameObject ::~CGameObject() {

}