#pragma once
#include "Object.h"
/*
---------------------------------------------------------------------------------------
ΩÃ±€≈Ê ≈€«√∏¥ ≈¨∑πΩ∫
---------------------------------------------------------------------------------------
*/

template <class ClassName>
class CSingleTonBase : public CObject
{
private:
	static ClassName* m_sInstance;
	

protected:
	
	CSingleTonBase(string name) : CObject(name){}

	~CSingleTonBase(){};


public:
	static ClassName* GetInstance()
	{
		if (nullptr == m_sInstance)
			return m_sInstance = new ClassName;

		return m_sInstance;
	}

	void ReleseInstance()
	{
		if (m_sInstance)
		{
			delete m_sInstance;
			m_sInstance = nullptr;
		}
	}
};

//ΩÃ±€≈œ ¿ŒΩ∫≈œΩ∫ √ ±‚»≠
template <class ClassName>
ClassName* CSingleTonBase<ClassName>::m_sInstance = nullptr;
