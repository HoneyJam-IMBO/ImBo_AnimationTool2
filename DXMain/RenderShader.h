#pragma once
#include "Shader.h"

class CRenderShader : public CShader{
public:
	//----------------------------dxobject-----------------------------
	bool Begin();
	bool End();

	void SetShaderState();
	void CleanShaderState();
	//----------------------------dxobject-----------------------------

	//---------------------------------shader---------------------------
	virtual void UpdateShaderState();
	virtual void ExcuteShaderState() {};//나중 cs를 위한 함수

										//begin func
	virtual bool CreateShaderValues() { return true; } //texture, constbuffer etc.
	bool CreateVS(LPCTSTR name, D3D11_INPUT_ELEMENT_DESC* vsDesc = nullptr, UINT nElement = 0);

	bool CreateGS(LPCTSTR name);
	
	bool CreateHS(LPCTSTR name);
	bool CreateDS(LPCTSTR name);

	bool CreatePS(LPCTSTR name);
	//begin func
	//---------------------------------shader---------------------------

	void SetShaderData(LPCTSTR vsName, D3D11_INPUT_ELEMENT_DESC* vsDesc, UINT nElement, LPCTSTR gsName, LPCTSTR hsName,
		LPCTSTR dsName, LPCTSTR psName);


protected:
	
	//-------------------------------shaders-----------------------------
	ID3D11VertexShader* m_vertexShader{ nullptr };
	ID3D11InputLayout* m_inputLayout{ nullptr };

	ID3D11GeometryShader* m_geometryShader{ nullptr };
	ID3D11HullShader* m_hullShader{ nullptr };
	ID3D11DomainShader* m_domainShader{ nullptr };
	
	ID3D11PixelShader* m_pixelShader{ nullptr };
	//-------------------------------shaders-----------------------------


public:
	CRenderShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CRenderShader();
};
