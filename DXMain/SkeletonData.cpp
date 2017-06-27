#include "stdafx.h"
#include "SkeletonData.h"

bool CSkeletonData::Begin(){

	return true;
}

bool CSkeletonData::End(){
	m_JointDatas.clear();

	return true;
}
