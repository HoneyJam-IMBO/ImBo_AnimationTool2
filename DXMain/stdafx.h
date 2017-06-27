// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

//console
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <timeapi.h>
#include <ctime>
#include <time.h>
#include <timeapi.h>
#include <queue>
#include <unordered_map>

#include "DXMain.h"

// D3D11
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <D3DX11tex.h>
#include "DirectXTex.h"

// D2D1
//#include <d2d1_3.h>
//#include <dwrite_3.h>
//#include <d2d1_2helper.h>
//#include <wincodec.h>

// DirectX Math
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

// C++11
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <chrono>
#include <stack>
#include <memory>



//fbx 어차피 이놈은 때어낼꺼야
#include <fbxsdk.h>
#include <AntTweakBar.h>

using namespace std;
using namespace std::chrono;

//using namespace D2D1;
using namespace DirectX;
using namespace DirectX::PackedVector;

template<typename Ty, size_t N>
constexpr size_t GetArraySize(Ty(&)[N]) noexcept
{
	return N;
}

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

//CB
//vs
#define VS_CB_CAMERA			0
#define VS_CB_MODEL				1
#define VS_CB_TEXTURE_MATRIX	2
#define VS_GLOBAL_BUFFER_SLOT	10
//ds
#define DS_CB_CAMERA			0		
//gs
#define GS_CB_CAMERA			0
#define GS_CB_AABB				1
//ps
#define PS_CB_MATERIAL 3
//CB

//TEXTURE
//ds
#define DS_SLOT_HEIGHTMAP		0
//ps
#define PS_TEXTURE 0x00
#define PS_SLOT_TERRAIN_BASE	0
#define PS_SLOT_TERRAIN_DETAIL	1
#define PS_SLOT_NORMALMAP		5
#define PS_SLOT_SKYBOX 13
//TEXTURE

//SAMPLER
#define PS_TEXTURE_SAMPLER		0
#define PS_NORMALMAP_SAMPLER	5
//SAMPLER

//DEFERRED_LIGHT
#define PS_UNPACKING_SLOT		0x00
#define PS_CAMERA_DATA_SLOT		0x01
#define PS_OBJECT_BUFFER_SLOT	0x02

#define DS_OBJECT_BUFFER_SLOT	0x00

#define MAX(a,b)(a>b? a : b)
#define MIN(a,b)(a<b? a : b)

//texture, constant buffer bind flag
#define BIND_VS 0b000001   // 0x0001 //0001
#define BIND_HS 0b000010   // 0x0002 //0010
#define BIND_DS 0b000100   // 0x0004 //0100
#define BIND_GS 0b001000   // 0x0008 //1000
#define BIND_PS 0b010000   // 0x0010 //...
#define BIND_CS 0b100000   // 0x0020 //..

//for loop를 위한 enum = OBJECT_END
enum object_id {
	OBJECT_SKYBOX,
	OBJECT_SPACE,
	OBJECT_TERRAIN,
	OBJECT_TOOL_NONANIM,
	OBJECT_TOOL_ANIM,
	OBJECT_TEST,
	OBJECT_PLANE,
	OBJECT_FBX_BUNNY,
	OBJECT_END,

	OBJECT_DIRECTIONAL_LIGHT,
	OBJECT_POINT_LIGHT,
	OBJECT_SPOT_LIGHT,
	OBJECT_CAPSULE_LIGHT,
	OBJECT_LIGHT_END,

	OBJECT_POSTPROCESSING,
	OBJECT_POSTPROCESSING_END,

	OBJECT_DEBUG_AABB,
	OBJECT_DEBUG_COORD,
	OBJECT_DEBUG_POINT_LIGHT,
	OBJECT_DEBUG_CAPSULE_LIGHT,
	OBJECT_DEBUG_SPOT_LIGHT,
	OBJECT_DEBUG_TEXTURE,
	OBJECT_DEBUG_END
};

enum tag {
	TAG_DEFAULT,
	TAG_TERRAIN,
	TAG_STATIC_OBJECT,
	TAG_SPACE,
	TAG_DYNAMIC_OBJECT,
	TAG_LIGHT,
	TAG_SSLR,
	TAG_POSTPROCESSING,
	TAG_DEBUG,
	TAG_END
};

enum shader_value_num {
	SVN_DEFAULT,
	SVN_DEBUG,
	SVN_END
};


//space
//space main info
#define SPACE_SIZE 256.0f
#define SPACE_LEVEL 0.0f
//space main info

//한 사이드에 있는 공간의 개수
#define ONESIDE_SPACE_NUM  pow(2.0f, SPACE_LEVEL)
//공간 하나의 크기
#define ONESPACE_SIZE SPACE_SIZE / ONESIDE_SPACE_NUM
//공간의 수
#define SPACE_NUM  pow(ONESIDE_SPACE_NUM, 2.0f)
//space

//aabb
#define BOUNDINGBOX_NUM 500
//coordinatesystem
#define COORD_NUM 500
//singleton
#include "GlobalValueManager.h"
#define			GLOBALVALUEMGR	CGlobalValueManager::GetInstance()
#include "Timer.h"
#define			TIMEMGR			CGameTimer::GetInstance()
#include "ResourceManager.h"
#define			RESOURCEMGR		CResourceManager::GetInstance()
#include "RenderContainerSeller.h"
#define			RCSELLER		CRenderContainerSeller::GetInstance()
#include "Debuger.h"
#define			DEBUGER			CDebuger::GetInstance()
#include "FbxImporter.h"
#define			FBXIMPORTER		CFbxImporter::GetInstance()
#include "DotXImporter.h"
#define			DOTXIMPORTER	CDotXImporter::GetInstance()
#include "InputManager.h"
#define			INPUTMGR		CInputManager::GetInstance()
#include "TwBarManager.h"
#define			TWBARMGR		CTwBarManager::GetInstance()
#include "DirectoryFinder.h"
#define			DIRECTORYFINDER	CDirectoryFinder::GetInstance()
#include "Exporter.h"
#define			EXPORTER	CExporter::GetInstance()
#include "Importer.h"
#define			IMPORTER	CImporter::GetInstance()
//#define USE_ANIM
