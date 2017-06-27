#include "stdafx.h"
#include "TwBarManager.h"


bool CTwBarManager::Begin(ID3D11Device * pd3dDevice, ID3D11DeviceContext * pd3dDeviceContext, const char* def) {
	m_pd3dDevice = pd3dDevice;
	m_pd3dDeviceContext = pd3dDeviceContext;
	
	//ui
	// Initialize AntTweakBar
	if (!TwInit(TW_DIRECT3D11, m_pd3dDevice)) {
		DEBUGER->DebugMessageBox("AntTweakBar initialization", "failed");
		return false;
	}

	TwDefine(def); // Message added to the help bar.
//	SetBarContained("GLOBAL", true);
//	SetBarResizable("GLOBAL", false);
	return true;
}

bool CTwBarManager::End() {
	//ui
	TwTerminate();

	m_mTwBar.clear();
	return true;
}


bool CTwBarManager::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	//ui
	// Send event message to AntTweakBar
	if (TwEventWin(hWnd, nMessageID, wParam, lParam))
		return true; // Event has been handled by AntTweakBar

	return false;
}

void CTwBarManager::Render(){
	//ui
	// Draw tweak bars
	TwDraw();
}

void CTwBarManager::AddBar(const char* barName){
	if (m_mTwBar.end() != m_mTwBar.find(barName)) return;//있으면 return

	TwBar *bar = TwNewBar(barName);
	m_mTwBar.insert(pairTwBar(barName, bar));
}

void CTwBarManager::SetParam(const char * barName, const char * paramName, TwParamValueType type, UINT inValueCnt, const void * inValue){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);

	TwSetParam(m_mTwBar[barName], barName, paramName, type, inValueCnt, inValue);
}

void CTwBarManager::AddVarCB(const char * barName, const char * manuName, TwType type, TwSetVarCallback setCallback, TwGetVarCallback getCallback, void * clientData, const char * def){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);

	TwAddVarCB(m_mTwBar[barName], manuName, type, setCallback, getCallback, clientData, def);
}

void CTwBarManager::AddVarRW(const char * barName, const char * manuName, TwType type, void * var, const char * def){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);

	TwAddVarRW(m_mTwBar[barName], manuName, type, var, def);
}

void CTwBarManager::AddDirBar(const char * barName, const char* groupName, const char * menuName, void * var){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "opened=true axisz=-z showval=false group=%s", groupName);

	TwAddVarRW(m_mTwBar[barName], menuName, TW_TYPE_DIR3F, var, buff);
}

void CTwBarManager::AddRotationBar(const char * barName, const char* groupName, const char * menuName, void * var){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "opened=true axisz=-z group=%s", groupName);

	TwAddVarRW(m_mTwBar[barName], menuName, TW_TYPE_QUAT4F, var, buff);
}

void CTwBarManager::AddColorBar(const char * barName, const char* groupName, const char * menuName, void * var){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "colormode=hls group=%s", groupName);

	TwAddVarRW(m_mTwBar[barName], menuName, TW_TYPE_COLOR4F, var, buff);
}

void CTwBarManager::AddBoolBar(const char * barName, const char* groupName, const char * menuName, void * var){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "group=%s", groupName);

	TwAddVarRW(m_mTwBar[barName], menuName, TW_TYPE_BOOLCPP, var, buff);
}

void CTwBarManager::AddBoolBarCB(const char * barName, const char * groupName, const char * menuName, TwSetVarCallback setCallback, TwGetVarCallback getCallback, void * clientData){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "group=%s", groupName);

	AddVarCB(barName, menuName, TW_TYPE_BOOLCPP, setCallback, getCallback, clientData, buff);
}

void CTwBarManager::AddMinMaxBarRW(const char * barName, const char * groupName, const char * menuName, void * var, float min, float max, float step){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "min=%f max=%f step=%f group=%s keyincr=PGUP keydecr=PGDOWN",min, max, step, groupName);

	TwAddVarRW(m_mTwBar[barName], menuName, TW_TYPE_FLOAT, var, buff);
}

void CTwBarManager::AddMinMaxBarCB(const char * barName, const char * groupName, const char * menuName, TwSetVarCallback setCallback, TwGetVarCallback getCallback, void * clientData, float min, float max, float step){
	//bool값과 obb의 크기 값을 수정 가능해야 한다. 
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "min=%f max=%f step=%f group=%s keyincr=+ keydecr=-", min, max, step, groupName);

	AddVarCB(barName, menuName, TW_TYPE_FLOAT, setCallback, getCallback, clientData, buff);
}

void CTwBarManager::AddButtonCB(const char * barName, const char * groupName, const char * menuName, TwButtonCallback buttonCallback, void * clientData){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "group=%s", groupName);

	TwAddButton(m_mTwBar[barName], menuName, buttonCallback, clientData, buff);
}

void CTwBarManager::AddSeparator(const char * barName, const char * groupName, const char * menuName){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "group=%s", groupName);

	TwAddSeparator(m_mTwBar[barName], menuName, buff);
}

//set param func
void CTwBarManager::SetBarSize(const char * barName, int x, int y){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) return;
	char buff[256];
	sprintf(buff, "%s size='%d %d'", barName, x, y);

	TwDefine(buff);
}

void CTwBarManager::SetBarColor(const char * barName, int r, int g, int b){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) return;
	if (r < 0 || g < 0 || b < 0 || 256 < r || 256 < g || 256 < b) return;
	char buff[256];
	sprintf(buff, "%s color='%d %d %d'", barName, r, g, b);

	TwDefine(buff);
}

void CTwBarManager::SetBarPosition(const char * barName, int x, int y){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) return;
	char buff[256];
	sprintf(buff, "%s position='%d %d'", barName, x, y);

	TwDefine(buff);
}

void CTwBarManager::SetBarRefresh(const char * barName, float refreshTime){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) return;
	char buff[256];
	sprintf(buff, "%s refresh=%f", barName, refreshTime);

	TwDefine(buff);
	//TwDefine(" mybar refresh=1.5 "); // refresh the bar every 1.5 seconds
}

void CTwBarManager::SetBarVisible(const char * barName, bool b){
	// mybar is hidden
	if (m_mTwBar.end() == m_mTwBar.find(barName)) return;
	char buff[256];

	if (b) {
		sprintf(buff, "%s visible=true", barName);
	}
	else {
		sprintf(buff, "%s visible=false", barName);
	}

	TwDefine(buff);
	//TwDefine(" mybar visible=false ");  
}

void CTwBarManager::SetBarResizable(const char * barName, bool b){
	// mybar cannot be resized
	if (m_mTwBar.end() == m_mTwBar.find(barName)) return;
	char buff[256];

	if (b) {
		sprintf(buff, "%s resizable=true", barName);
	}
	else {
		sprintf(buff, "%s resizable=false", barName);
	}

	TwDefine(buff);
}

void CTwBarManager::SetBarContained(const char * barName, bool b){
	//GLOBAL contained=true bars cannot move outside of the window
	if (m_mTwBar.end() == m_mTwBar.find(barName)) return;
	char buff[256];

	if (b) {
		sprintf(buff, "%s contained=true", barName);
	}
	else {
		sprintf(buff, "%s contained=false", barName);
	}

	TwDefine(buff);
}
void CTwBarManager::SetBarMovable(const char * barName, bool b){
	//TwDefine(" mybar movable=false "); // mybar cannot be moved
	if (m_mTwBar.end() == m_mTwBar.find(barName)) return;
	char buff[256];

	if (b) {
		sprintf(buff, "%s movable=true", barName);
	}
	else {
		sprintf(buff, "%s movable=false", barName);
	}

	TwDefine(buff);
}
//set param func

void CTwBarManager::AddDirBar(const char * barName, const char * groupName, const char * menuName, CGameObject* pObj){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "opened=true axisz=-z showval=false group=%s", groupName);

	AddVarCB(barName, menuName, TW_TYPE_DIR3F, SetQuaternionToTwBar, GetQuaternionToTwBar, pObj, buff);
}

void CTwBarManager::AddRotationBar(const char * barName, const char * groupName, const char * menuName, CGameObject* pObj){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "opened=true axisz=-z group=%s", groupName);

	AddVarCB(barName, menuName, TW_TYPE_QUAT4F, SetQuaternionToTwBar, GetQuaternionToTwBar, pObj, buff);
}

void CTwBarManager::AddRotationMinMaxBar(const char * barName, const char * groupName, const char * menuName, CGameObject * pObj){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "min=%f max=%f step=%f group=%s keyincr=+ keydecr=-", -360.f, 360.f, 0.1f, groupName);

	char subMenuNameX[64];
	sprintf(subMenuNameX, "%sX", menuName);
	char subMenuNameY[64];
	sprintf(subMenuNameY, "%sY", menuName);
	char subMenuNameZ[64];
	sprintf(subMenuNameZ, "%sZ", menuName);

	AddVarCB(barName, subMenuNameX, TW_TYPE_FLOAT, SetRotationXToTwBar, GetRotationXToTwBar, pObj, buff);
	AddVarCB(barName, subMenuNameY, TW_TYPE_FLOAT, SetRotationYToTwBar, GetRotationYToTwBar, pObj, buff);
	AddVarCB(barName, subMenuNameZ, TW_TYPE_FLOAT, SetRotationZToTwBar, GetRotationZToTwBar, pObj, buff);
}

void CTwBarManager::AddPositionBar(const char * barName, const char * groupName, const char * menuName, CGameObject * pObj,
	float min, float max, float step){

	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "min=%f max=%f step=%f group=%s keyincr=+ keydecr=-", min, max, step, groupName);

	char subMenuNameX[64];
	sprintf(subMenuNameX, "%sX", menuName);
	char subMenuNameY[64];
	sprintf(subMenuNameY, "%sY", menuName);
	char subMenuNameZ[64];
	sprintf(subMenuNameZ, "%sZ", menuName);

	AddVarCB(barName, subMenuNameX, TW_TYPE_FLOAT, SetPositionXToTwBar, GetPositionXToTwBar, pObj, buff);
	AddVarCB(barName, subMenuNameY, TW_TYPE_FLOAT, SetPositionYToTwBar, GetPositionYToTwBar, pObj, buff);
	AddVarCB(barName, subMenuNameZ, TW_TYPE_FLOAT, SetPositionZToTwBar, GetPositionZToTwBar, pObj, buff);
}

void CTwBarManager::AddOBBBar(const char * barName, const char * groupName, const char * menuName, BoundingOrientedBox * pJointData){
	//bool값과 obb의 크기 값을 수정 가능해야 한다. 
	char subMenuNameScale[64];
	sprintf(subMenuNameScale, "%sScale", menuName);
	char subMenuNameX[64];
	sprintf(subMenuNameX, "%sX", menuName);
	char subMenuNameY[64];
	sprintf(subMenuNameY, "%sY", menuName);
	char subMenuNameZ[64];
	sprintf(subMenuNameZ, "%sZ", menuName);

	
	AddMinMaxBarCB(barName, groupName, subMenuNameScale, SetScaleToTwBar, GetScaleToTwBar, pJointData, 0.f, 100.0f, 0.1f);
	AddMinMaxBarRW(barName, groupName, subMenuNameScale, &pJointData->Extents.x, 0.f, 100.0f, 0.1f);
	AddMinMaxBarRW(barName, groupName, subMenuNameX, &pJointData->Extents.x, 0.f, 100.0f, 0.1f);
	AddMinMaxBarRW(barName, groupName, subMenuNameY, &pJointData->Extents.y, 0.f, 100.0f, 0.1f);
	AddMinMaxBarRW(barName, groupName, subMenuNameZ, &pJointData->Extents.z, 0.f, 100.0f, 0.1f);
}
void CTwBarManager::AddScaleBar(const char * barName, const char * groupName, const char * menuName, CGameObject * pObj,
	float min, float max, float step) {
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);

	char subMenuNameScale[64];
	sprintf(subMenuNameScale, "%sScale", menuName);
	char subMenuNameX[64];
	sprintf(subMenuNameX, "%sX", menuName);
	char subMenuNameY[64];
	sprintf(subMenuNameY, "%sY", menuName);
	char subMenuNameZ[64];
	sprintf(subMenuNameZ, "%sZ", menuName);

	AddMinMaxBarCB(barName, groupName, subMenuNameScale, SetScaleToTwBar, GetScaleToTwBar, pObj, min, max, step);
	AddMinMaxBarCB(barName, groupName, subMenuNameX, SetScaleXToTwBar, GetScaleXToTwBar, pObj, min, max, step);
	AddMinMaxBarCB(barName, groupName, subMenuNameY, SetScaleYToTwBar, GetScaleYToTwBar, pObj, min, max, step);
	AddMinMaxBarCB(barName, groupName, subMenuNameZ, SetScaleZToTwBar, GetScaleZToTwBar, pObj, min, max, step);
}

//void CTwBarManager::AddBoundingBoxActiveBar(const char * barName, const char * groupName, const char * menuName, CFbxJointData* pJoint){
//	AddBoolBarCB(barName, groupName, menuName, SetBoundingBoxActiveToTwBar, GetBoundingBoxActiveToTwBar, pJoint);
//}

void CTwBarManager::DeleteBar(const char * barName){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) return;//없으면 return;

	TwDeleteBar(m_mTwBar[barName]);
	m_mTwBar.erase(barName);
}

void CTwBarManager::DeleteAllBars(){
	TwDeleteAllBars();
}

void CTwBarManager::DeleteVar(const char * barName, const char * menuName){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) return;//없으면 return;

	TwRemoveVar(m_mTwBar[barName], menuName);
}

void CTwBarManager::DeleteAllVars(const char * barName){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) return;//없으면 return;

	TwRemoveAllVars(m_mTwBar[barName]);
}


//proc
//quaternion
void TW_CALL SetQuaternionToTwBar(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	pObj->SetQuaternion(XMLoadFloat4(static_cast<const XMFLOAT4 *>(value)));

	pObj->SetWorldMtx(XMMatrixAffineTransformation(pObj->GetScale(), XMQuaternionIdentity(), pObj->GetQuaternion(), pObj->GetPosition()));
}
void TW_CALL GetQuaternionToTwBar(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	XMFLOAT4 xmf4;
	XMStoreFloat4(&xmf4, pObj->GetQuaternion());
	
	*static_cast<XMFLOAT4 *>(value) = xmf4;
}
//x
void TW_CALL SetPositionXToTwBar(const void * value, void * clientData){
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	pObj->SetPositionX(*static_cast<const float *>(value));
}
void TW_CALL GetPositionXToTwBar(void * value, void * clientData){
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	*static_cast<float *>(value) = pObj->GetPositionX();
}
//y
void TW_CALL SetPositionYToTwBar(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	pObj->SetPositionY(*static_cast<const float *>(value));
}
void TW_CALL GetPositionYToTwBar(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	*static_cast<float *>(value) = pObj->GetPositionY();
}
//z
void TW_CALL SetPositionZToTwBar(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	pObj->SetPositionZ(*static_cast<const float *>(value));
}
void TW_CALL GetPositionZToTwBar(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	*static_cast<float *>(value) = pObj->GetPositionZ();
}

void TW_CALL SetRotationXToTwBar(const void * value, void * clientData){
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	float rotate = (*static_cast<const float *>(value)) - pObj->GetRotateX();
	pObj->RotateWorldAxis(rotate, 0.f, 0.f);
}

void TW_CALL GetRotationXToTwBar(void * value, void * clientData){
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	*static_cast<float *>(value) = pObj->GetRotateX();
}

void TW_CALL SetRotationYToTwBar(const void * value, void * clientData)
{
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	float rotate = (*static_cast<const float *>(value)) - pObj->GetRotateY();
	pObj->RotateWorldAxis(0.f, rotate, 0.f);
}

void TW_CALL GetRotationYToTwBar(void * value, void * clientData){
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	*static_cast<float *>(value) = pObj->GetRotateY();
}

void TW_CALL SetRotationZToTwBar(const void * value, void * clientData)
{
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	float rotate = (*static_cast<const float *>(value))- pObj->GetRotateZ();
	pObj->RotateWorldAxis(0.f, 0.f, rotate);
}

void TW_CALL GetRotationZToTwBar(void * value, void * clientData){
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	*static_cast<float *>(value) = pObj->GetRotateZ();
}

//scale
void TW_CALL SetScaleToTwBar(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	pObj->SetScale(XMVectorSet(pObj->m_xmf4Scale.x, pObj->m_xmf4Scale.y, pObj->m_xmf4Scale.z, *static_cast<const float *>(value)));
}
void TW_CALL GetScaleToTwBar(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	*static_cast<float *>(value) = pObj->m_xmf4Scale.w;
}
//x
void TW_CALL SetScaleXToTwBar(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	pObj->SetScale(XMVectorSet(*static_cast<const float *>(value), pObj->m_xmf4Scale.y, pObj->m_xmf4Scale.z, pObj->m_xmf4Scale.w));
}
void TW_CALL GetScaleXToTwBar(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	*static_cast<float *>(value) = pObj->m_xmf4Scale.x;
}
//y
void TW_CALL SetScaleYToTwBar(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	pObj->SetScale(XMVectorSet(pObj->m_xmf4Scale.x, *static_cast<const float *>(value), pObj->m_xmf4Scale.z, pObj->m_xmf4Scale.w));
}
void TW_CALL GetScaleYToTwBar(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	*static_cast<float *>(value) = pObj->m_xmf4Scale.y;
}
//z
void TW_CALL SetScaleZToTwBar(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	pObj->SetScale(XMVectorSet(pObj->m_xmf4Scale.x, pObj->m_xmf4Scale.y, *static_cast<const float *>(value), pObj->m_xmf4Scale.w));
}
void TW_CALL GetScaleZToTwBar(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	*static_cast<float *>(value) = pObj->m_xmf4Scale.z;
}

//void TW_CALL SetBoundingBoxActiveToTwBar(const void * value, void * clientData){
//	if (nullptr == clientData) return;
//	CFbxJointData* pObj = reinterpret_cast<CFbxJointData*>(clientData);
//	pObj->SetActive((*static_cast<const bool *>(value)));
//	if ((*static_cast<const bool *>(value))) {
//		//들어온 값이 true면 off 중에 on을 누른것 이다.
//		pObj->ProcOn(pObj->GetCurFrame());
//	}
//	else {
//		pObj->ProcOff(pObj->GetCurFrame());
//	}
//}

void TW_CALL GetBoundingBoxActiveToTwBar(void * value, void * clientData){
	if (nullptr == clientData) return;
	CFbxJointData* pObj = reinterpret_cast<CFbxJointData*>(clientData);
	*static_cast<bool *>(value) = pObj->GetActive();
}
