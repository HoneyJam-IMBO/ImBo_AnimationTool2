#pragma once

#include "SingleTon.h"
#include "DirectXFramework.h"

#include "Animater.h"//animater!!

#include "DirectionalLightMesh.h"
#include "PointLightMesh.h"
#include "SpotLightMesh.h"
#include "CapsuleLightMesh.h"
#include "TestMesh.h"
#include "PlaneMesh.h"
#include "TestDeferredMesh.h"
#include "SkyBoxMesh.h"//skybox
#include "SpaceMesh.h"//space
#include "TerrainMesh.h"//terrain
#include "BoundingBoxMesh.h"//BoundingBox mesh
#include "DebugTextureMesh.h"//debug texture mesh
#include "CoordinateSysMesh.h"//CoordinateSystem Mesh
#include "FileBasedMesh.h"//fbx mesh
//#include "AnimationMesh.h"//animation mesh
#include "MultiMesh.h"//multiMesh

#include "TestObject.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CapsuleLight.h"
#include "SkyBox.h"//skybox
#include "Space.h"//space
#include "Terrain.h"//terrain
#include "DebugTexture.h"//debug texture

//buffer
#include "InstanceBuffer.h"
#include "ConstantBuffer.h"
//global buffer
#include "GlobalBuffer.h"
#include "GlobalObject.h"
#include "GlobalTerrain.h"


#include "Material.h"

#include "Texture.h"
#include "Sampler.h"
#include "RenderShader.h"


class CResourceManager : public CSingleTonBase<CResourceManager> {

public:
	bool Begin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool End();

	//begin func
	void CreateTextures();
	shared_ptr<CSampler> CreateSampler(UINT Slot = 0, UINT BindFlags = BIND_PS, D3D11_TEXTURE_ADDRESS_MODE Mode = D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_COMPARISON_FUNC ComparisionFunc = D3D11_COMPARISON_NEVER, float MinLOD = 0, float MaxLOD = 0);
	shared_ptr<CTexture> CreateTexture(string name, const TCHAR* pstrTextureNames, shared_ptr<CSampler> pSampler, UINT Slot = 0, UINT BindFlag = BIND_PS);
	void CreateRenderShaders();
	void CreateMeshs();
	void CreateAnimater(string path, string animaterName);
	void CreateBuffers();
	void CreateGlobalBuffers();
	void CreateMaterials();
	void CreateAnimaters();

	UINT CreateMultiMesh(string path, string name);
	//.gjm file load
	void CreateGJMResources();
	UINT CreateGJMResource(string path, string name);
	//.gjm file load
	//fbx file load
	void CreateFBXResources();
	UINT CreateFBXResource(string path, string name);
	//fbx file load
	//dotx file load
	UINT CreateDotXResource(string path, string name);
	//end func
	void ReleaseTextures();
	void ReleaseRenderShaders();
	void ReleaseMeshs();
	void ReleaseMesh(string name);
	void ReleaseAnimater(string name);
	void ReleaseBuffers();
	void ReleaseGlobalBuffers();
	void ReleaseMaterials();
	void ReleaseAnimaters();

	shared_ptr<CTexture> GetTexture(string name) { return m_mTexture[name]; }
	shared_ptr<CSampler> GetSampler(string name) { return m_mSampler[name]; }
	shared_ptr<CRenderShader> GetRenderShader(string name) { return m_mRenderShader[name]; }
	shared_ptr<CMesh> GetMesh(string name) { return m_mMesh[name]; }
	shared_ptr<CBuffer> GetBuffer(string name) { return m_mBuffer[name]; }
	shared_ptr<CGlobalBuffer> GetGlobalBuffer(string name) { return m_mGlobalBuffer[name]; }
	shared_ptr<CMaterial> GetMaterial(string name) { return m_mMaterial[name]; }
	shared_ptr<CAnimater> GetAnimater(string name) {
		if (m_mAnimater.end() != m_mAnimater.find(name))
			return m_mAnimater[name];
		return nullptr;
	}
private:
	ID3D11Device* m_pd3dDevice{ nullptr };
	ID3D11DeviceContext* m_pd3dDeviceContext{ nullptr };

	map<string, shared_ptr<CTexture>> m_mTexture;
	using pairTexture = pair<string, shared_ptr<CTexture>>;
	map<string, shared_ptr<CRenderShader>> m_mRenderShader;
	using pairShader = pair<string, shared_ptr<CRenderShader>>;
	map<string, shared_ptr<CMesh>> m_mMesh;
	using pairMesh = pair<string, shared_ptr<CMesh>>;
	map<string, shared_ptr<CBuffer>> m_mBuffer;
	using pairBuffer = pair<string, shared_ptr<CBuffer>>;
	map<string, shared_ptr<CGlobalBuffer>> m_mGlobalBuffer;
	using pairGlobalBuffer = pair<string, shared_ptr<CGlobalBuffer>>;
	map<string, shared_ptr<CMaterial>> m_mMaterial;
	using pairMaterial = pair<string, shared_ptr<CMaterial>>;
	map<string, shared_ptr<CSampler>> m_mSampler;
	using pairSampler = pair<string, shared_ptr<CSampler>>;
	//animater
	map<string, shared_ptr<CAnimater>> m_mAnimater;
	using pairAnimater = pair<string, shared_ptr<CAnimater>>;
public:
	CResourceManager();
	virtual ~CResourceManager();


};
