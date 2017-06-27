#include "stdafx.h"
#include "AnimationData.h"

bool CAnimationData::Begin()
{
	return true;
}

bool CAnimationData::End(){
	for (auto data : m_vvKeyFrame) {
		data.clear();
	}
	m_vvKeyFrame.clear();
	return true;
}
