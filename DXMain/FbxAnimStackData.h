#pragma once
#include "Object.h"

class CFbxAnimStackData : public CObject {
public:
	bool Begin();
	virtual bool End();

	void SetpAnimStack(FbxAnimStack* pStack) { m_pAnimStack = pStack; }
	void SetAnimationName(string name) { m_sName = name; }
	void SetTimeMode(FbxTime::EMode& timeMode) { m_TimeMode = timeMode; }
	void SetTimeStart(FbxTime& time) { m_tStart = time; }
	void SetTimeEnd(FbxTime& time) { m_tEnd = time; }

	FbxAnimStack* GetpAnimStack() { return m_pAnimStack; }
	FbxTime::EMode& GetTimeMode() { return m_TimeMode; }
	FbxLongLong GetTimeStart() { return m_tStart.GetFrameCount(FbxTime::eFrames30); }
	FbxLongLong GetTimeEnd() { return m_tEnd.GetFrameCount(FbxTime::eFrames30); }
	string GetAnimationName() { return m_sName; }

private:
	string m_sName;//¾²·¹±â
	FbxAnimStack* m_pAnimStack;
	FbxTime::EMode m_TimeMode;
	FbxTime m_tStart;
	FbxTime m_tEnd;

public:
	CFbxAnimStackData() : CObject("fbxanimstackdata") {}
	~CFbxAnimStackData() {}
};