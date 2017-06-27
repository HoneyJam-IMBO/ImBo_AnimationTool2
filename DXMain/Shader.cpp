#include "stdafx.h"
#include "Shader.h"


bool CShader::Begin() {
	return true;
}
bool CShader::End() {
	return true;
}
void CShader::SetShaderState() {

}
void CShader::CleanShaderState() {

}
CShader::CShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : DXObject("shader", pd3dDevice, pd3dDeviceContext) {

}
CShader::~CShader() {

}