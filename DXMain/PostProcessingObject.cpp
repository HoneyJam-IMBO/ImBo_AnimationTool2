#include "stdafx.h"
#include "PostProcessingObject.h"

bool CPostProcessingObject::Begin() {
	m_objectID = object_id::OBJECT_POSTPROCESSING;

	return CObject::Begin();
};
