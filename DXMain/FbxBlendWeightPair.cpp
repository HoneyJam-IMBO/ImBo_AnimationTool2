#include "stdafx.h"
#include "FbxBlendWeightPair.h"

void CFbxBlendWeightPair::GetBlendWeightPair(UINT & nIndex, double& Weight){
	nIndex = m_nIndex;
	Weight = m_Weight;
}

void CFbxBlendWeightPair::SetBlendWeightPair(UINT nIndex, double Weight){
	m_nIndex = nIndex;
	m_Weight = Weight;
}

CFbxBlendWeightPair::CFbxBlendWeightPair(UINT nIndex, double Weight) : CObject("fbxblendweightpair"){
	m_nIndex = nIndex;
	m_Weight = Weight;
}

CFbxBlendWeightPair::~CFbxBlendWeightPair(){

}
