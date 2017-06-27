#pragma once
#include "ComputeShader.h"

class CDownScaleSecondPassShader : public CComputeShader {
public:
	//---------------------------------shader---------------------------
	virtual void UpdateShaderState();
	//---------------------------------shader---------------------------
	void SwapAdaptationBuff();
private:

	//begin func
	virtual bool CreateShaderValues();//texture, constbuffer etc.
	virtual bool CreateCS();
	//begin func

	//end func 
	virtual bool ReleaseShaderValues();
	//end func

	//set func
	virtual void SetShaderValues();
	//clean func
	virtual void CleanShaderValues();

	
private:
	ID3D11Buffer* m_pSecondPassBuffer{ nullptr };
	ID3D11UnorderedAccessView* m_pSecondPassUAV{ nullptr };
	ID3D11ShaderResourceView* m_pSecondPassSRV{ nullptr };

	//¿Ã¿¸ ∆Ú±’»÷µµ ∞™ 
	ID3D11Buffer* m_pPrevAvgLumBuffer{ nullptr };
	ID3D11UnorderedAccessView* m_pPrevAvgLumUAV{ nullptr };
	ID3D11ShaderResourceView* m_pPrevAvgLumSRV{ nullptr };
public:
	CDownScaleSecondPassShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CDownScaleSecondPassShader();
};
