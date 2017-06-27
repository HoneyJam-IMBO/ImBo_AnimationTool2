#pragma once
#include "Shader.h"

class CComputeShader : public CShader {
public:
	//----------------------------dxobject-----------------------------
	bool Begin();
	bool End();
	//----------------------------dxobject-----------------------------

	//---------------------------------shader---------------------------
	virtual void UpdateShaderState();
	void ExcuteShaderState();//나중 cs실행

	void SetThreadGroup(UINT x, UINT y, UINT z);
	//---------------------------------shader---------------------------


	virtual void ResizeBuffer(UINT nWidth, UINT nHeight);
protected:
	//dxobject
	void SetShaderState();
	void CleanShaderState();
	//dxobject

	//begin func
	virtual bool CreateShaderValues() { return true; } //texture, constbuffer etc.
	virtual bool CreateCS() { return true; }
	//begin func

	//end func
	virtual bool ReleaseShaderValues() { return true; }
	//end func

	//set func
	virtual void SetShaderValues();
	//clean func
	virtual void CleanShaderValues();


	//-------------------------------shaders-----------------------------
	ID3D11ComputeShader* m_computeShader{ nullptr };

	UINT m_ThreadGroupX{ 0 };
	UINT m_ThreadGroupY{ 0 };
	UINT m_ThreadGroupZ{ 0 };

	UINT m_nBufferWidth{ 0 };
	UINT m_nBufferHeight{ 0 };
	//-------------------------------shaders-----------------------------

public:
	CComputeShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CComputeShader();
};
