#pragma once

#include "Object.h"
//font
#include "../../Assets/import/FW1FontWrapper_1_1/FW1FontWrapper.h"

using namespace std;

//font data
enum YT_TEXT
{
	PIVOT_TOP = FW1_TOP,
	PIVOT_LEFT = FW1_LEFT,
	PIVOT_HORIZONTAL_CENTER = FW1_CENTER,
	PIVOT_RIGHT = FW1_RIGHT,
	PIVOT_VERTICLE_CENTER = FW1_VCENTER,
	PIVOT_BOTTOM = FW1_BOTTOM
};

class YT_Color
{
private:
	UINT32						m_color32;

public:

	YT_Color() {};

	~YT_Color() {};

	YT_Color(FLOAT Red, FLOAT Green, FLOAT Blue, FLOAT Alpha = 255.0f)
	{
		UINT32 color32;
		BYTE *colorBytes = reinterpret_cast<BYTE*>(&color32);
		colorBytes[0] = static_cast<BYTE>(Red);
		colorBytes[1] = static_cast<BYTE>(Green);
		colorBytes[2] = static_cast<BYTE>(Blue);
		colorBytes[3] = static_cast<BYTE>(Alpha);

		m_color32 = color32;

	}


	void operator()(UINT32 color32) { m_color32 = color32; }

	void operator=(UINT32 color32) { m_color32 = color32; }

	void operator()(FLOAT Red, FLOAT Green, FLOAT Blue, FLOAT Alpha = 255.0f)
	{
		UINT32 color32;
		BYTE *colorBytes = reinterpret_cast<BYTE*>(&color32);
		colorBytes[0] = static_cast<BYTE>(Red);
		colorBytes[1] = static_cast<BYTE>(Green);
		colorBytes[2] = static_cast<BYTE>(Blue);
		colorBytes[3] = static_cast<BYTE>(Alpha);


		m_color32 = color32;
	}

	operator UINT32() const { return m_color32; }

	UINT32 ytRand_rgb_bri()
	{
		float rgb[3];
		rgb[0] = (float)((rand() % 255) *1.5);
		rgb[1] = (float)((rand() % 255) *1.2);
		rgb[2] = (float)((rand() % 255) *0.9);
		SetColor(rgb[0], rgb[1], rgb[2]);
	}

	//Set the color
	void SetColor(FLOAT Red, FLOAT Green, FLOAT Blue, FLOAT Alpha = 255.0f)
	{
		UINT32 color32;
		BYTE *colorBytes = reinterpret_cast<BYTE*>(&color32);
		colorBytes[0] = static_cast<BYTE>(Red);
		colorBytes[1] = static_cast<BYTE>(Green);
		colorBytes[2] = static_cast<BYTE>(Blue);
		colorBytes[3] = static_cast<BYTE>(Alpha);

		m_color32 = color32;
	}

};

class CDebugFontData : public CObject {
public:

public:
	wstring	  msg;
	float x{ 0 };
	float y{ 0 };
	float fontSize{ 0 };
	UINT color{ YT_Color(0, 0, 0) };
	UINT pivot{ (FW1_LEFT | FW1_TOP) };

public:
	CDebugFontData(wstring msg, float fontSize, float x, float y, UINT color, UINT pivot) :
		msg(msg), fontSize(fontSize), x(x), y(y), color(color), pivot(pivot), CObject("fontdata") {};
	CDebugFontData(TCHAR* msg, float fontSize, float x, float y, UINT color, UINT pivot) :
		msg(msg), fontSize(fontSize), x(x), y(y), color(color), pivot(pivot), CObject("fontdata") {};
	virtual ~CDebugFontData() {}
};