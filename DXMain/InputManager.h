#pragma once

#include "SingleTon.h"

//for drag&drop
#include <shellapi.h>
#define DROP_FILE_NAME_BUFF_SIZE 100

//방향
#define DIR_FORWARD			0x01
#define DIR_BACKWARD		0x02
#define DIR_LEFT			0x04
#define DIR_RIGHT			0x08
#define DIR_UP				0x10
#define DIR_DOWN			0x20
#define DIR_RUN				0x21
enum YT_MouseWheel{
	WHEEL_UP = 1,
	WHEEL_DOWN = -1,
	WHEEL_NON = 0,
};

enum eVK {
	VK_0 = 48,
	VK_1 = 49,
	VK_2,
	VK_3,
	VK_4,
	VK_5,
	VK_6,
	VK_7,
	VK_8,
	VK_9,
	VK_A = 0x41,
	VK_B,
	VK_C,
	VK_D,
	VK_E,
	VK_F,
	VK_G,
	VK_H,
	VK_I,
	VK_J,
	VK_K,
	VK_L,
	VK_M,
	VK_N,
	VK_O,
	VK_P,
	VK_Q,
	VK_R,
	VK_S,
	VK_T,
	VK_U,
	VK_V,
	VK_W,
	VK_X,
	VK_Y,
	VK_Z,
	VK_SHIFT_= 0x10
};

class CInputManager : public CSingleTonBase<CInputManager> {

public:
	bool Begin(HWND hWnd);
	bool End();

	void Update(float fTimeEleasped);
	void UpdateKeyBoard();
	void UpdateMouse();

	bool KeyDown(eVK input);
	bool KeyBoardDown(eVK input);

	bool KeyUp(eVK input);
	bool KeyBoardUp(eVK input);

	bool OnlyKeyDown(eVK input);
	bool OnlyKeyBoardDown(eVK input);

	BYTE AnyOneKeyDown(int n, eVK ...);

	//Mouse
	/*  Wheel Down < WheelNon < Wheel Up  */
	void SetWheel(int wheel);
	int GetWheel() { return m_sWheel; }
	void MouseWheel();
	void SetMousePoint(int qx, int y);
	void SetMousePoint(POINT _pt);
	bool MouseLeftDown();
	bool MouseRightDown();
	bool MouseLeftUp();
	bool MouseRightUp();
	void SetMouseLeft(bool click) { m_bLeftCheck = click; }
	void SetMouseRight(bool click) { m_bRightCheck = click; }
	POINT GetMousePoint() { return pt; }

	//void InputDataSending();


	//mouse input
	void SetbCapture(bool b);
	//void ReleaseCapture() { m_bCapture = false; };
	void SetOldCursorPos() { GetCursorPos(&m_ptOldCursorPos); }
	POINT GetOldCursorPos() const { return m_ptOldCursorPos; }

	float GetcxDelta() { return m_cxDelta; }
	float GetcyDelta() { return m_cyDelta; }
	//void SetOldcxDelta(float cxdelta) { m_OldcxDelta = cxdelta; }
	//void SetOldcyDelta(float cydelta) { m_OldcyDelta = cydelta; }

	void SetDebugMode(bool b) { m_bDebug = b; }
	bool GetDebugMode() { return m_bDebug; }
	//Mode 나중에 전여 singleton으로 빼자
	bool m_bDebug{ false };

	//drag & drop
	void ProcDropFile(WPARAM wParam);
	const char* GetDropFileName();
	const bool GetUsedDropFileName() { return m_bCanUseDropFileName; }

private:
	HWND m_hWnd;
	bool							m_bKeyBoardPressCheck{ false };
	UCHAR m_pKeyBuffers[256];
	bool m_bUpCheck[256];
	bool m_bDownCheck[256];

	//mouse
	int									m_sWheel{ WHEEL_NON };
	POINT								pt;

	bool								m_bCapture{ false };
	bool								m_bLeftCheck{ false };
	bool								m_bRightCheck{ false };

	POINT							m_ptOldCursorPos;
	float m_cxDelta{ 0.f }, m_cyDelta{ 0.f };

	//for drag&drop
	HDROP m_hDropInfo{ NULL };
	UINT m_DropFileBuffSize{ DROP_FILE_NAME_BUFF_SIZE };
	char m_DropFileNameBuf[DROP_FILE_NAME_BUFF_SIZE];
	bool m_bCanUseDropFileName{ false };
public:
	CInputManager();
	virtual ~CInputManager();

};
