#include "stdafx.h"
#include "AnimationMesh.h"

bool CAnimationMesh::Begin(UINT index){
	//mesh의 정점 인덱스, 노말 데이터 추출
	CFileBasedMesh::Begin(index);
	//m_MeshIndex = index;

	return true;
}

bool CAnimationMesh::End(){
	//delete[] m_pxmf4BoneIndex;
	//delete[] m_pxmf3Weight;
	
	return CFileBasedMesh::End();
}
