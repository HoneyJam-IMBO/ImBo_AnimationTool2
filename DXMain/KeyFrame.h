#pragma once
#include "Object.h"

class CKeyFrame : public CObject {
public:
	void SetKeyFrameTransformMtx(XMMATRIX& mtx) { m_xmmtxTransform = mtx; }
	XMMATRIX& GetKeyFrameTransformMtx() { return m_xmmtxTransform; }
	void SetFrameTime(double time) { m_dFrameTime = time; }
	double GetFrameTime() { return m_dFrameTime; }

private:
	XMMATRIX m_xmmtxTransform;
	double m_dFrameTime;

public:
	CKeyFrame() : CObject("keyframe") {};
	CKeyFrame(double curFrameTime, XMMATRIX mtx) : CObject("keyframe"),
		m_dFrameTime(curFrameTime), m_xmmtxTransform(mtx) {};
	~CKeyFrame() {};
};