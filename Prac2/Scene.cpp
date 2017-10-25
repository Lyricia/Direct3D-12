//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::BuildLightsAndMaterials()
{
	m_pLights = new LIGHTS;
	::ZeroMemory(m_pLights, sizeof(LIGHTS));

	m_pLights->m_xmf4GlobalAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);


	m_pLights->m_pLights[0].m_bEnable = true;
	m_pLights->m_pLights[0].m_nType = SPOT_LIGHT;
	m_pLights->m_pLights[0].m_fRange = 100.0f;
	m_pLights->m_pLights[0].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[0].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights->m_pLights[0].m_xmf4Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pLights->m_pLights[0].m_xmf3Position = XMFLOAT3(-50.0f, 20.0f, -5.0f);
	m_pLights->m_pLights[0].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[0].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights->m_pLights[0].m_fFalloff = 8.0f;
	m_pLights->m_pLights[0].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights->m_pLights[0].m_fTheta = (float)cos(XMConvertToRadians(20.0f));
	
	// Sun, Moon
	m_pLights->m_pLights[1].m_bEnable = true;
	m_pLights->m_pLights[1].m_nType = DIRECTIONAL_LIGHT;
	m_pLights->m_pLights[1].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[1].m_xmf4Diffuse = XMFLOAT4(0.0008f, 0.0008f, 0.0008f, 1.0f);
	m_pLights->m_pLights[1].m_xmf4Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.1f);
	
	m_pLights->m_pLights[2].m_bEnable = true;
	m_pLights->m_pLights[2].m_nType = DIRECTIONAL_LIGHT;
	m_pLights->m_pLights[2].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[2].m_xmf4Diffuse = XMFLOAT4(0.001f, 0.001f, 0.001f, 1.0f);
	m_pLights->m_pLights[2].m_xmf4Specular = XMFLOAT4(0.01f, 0.01f, 0.01f, 0.01f);

	// Lights in Building
	int i = 0, j = 0;
	int k = 5;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (k % 2) {
				m_pLights->m_pLights[k].m_bEnable = true;
				m_pLights->m_pLights[k].m_nType = POINT_LIGHT;
				m_pLights->m_pLights[k].m_fRange = 50;
				m_pLights->m_pLights[k].m_xmf4Ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
				m_pLights->m_pLights[k].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
				m_pLights->m_pLights[k].m_xmf4Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
				m_pLights->m_pLights[k].m_xmf3Position = XMFLOAT3(50.f * (i + 1), 45.f, 50.f * (j + 1));
				m_pLights->m_pLights[k].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
				m_pLights->m_pLights[k].m_xmf3Attenuation = XMFLOAT3(1.f, 0.0001f, 0.00001f);
			}
			else {
				m_pLights->m_pLights[k].m_bEnable = true;
				m_pLights->m_pLights[k].m_nType = SPOT_LIGHT;
				m_pLights->m_pLights[k].m_fRange = 80.0f;
				m_pLights->m_pLights[k].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
				m_pLights->m_pLights[k].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
				m_pLights->m_pLights[k].m_xmf4Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
				m_pLights->m_pLights[k].m_xmf3Position = XMFLOAT3(50.f * (i + 1), 45.f, 50.f * (j + 1));
				m_pLights->m_pLights[k].m_xmf3Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
				m_pLights->m_pLights[k].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.001f, 0.0001f);
				m_pLights->m_pLights[k].m_fFalloff = 8.0f;
				m_pLights->m_pLights[k].m_fPhi = (float)cos(XMConvertToRadians(90.0f));
				m_pLights->m_pLights[k].m_fTheta = (float)cos(XMConvertToRadians(30.0f));
			}
			k++;
		}
	}

	m_pMaterials = new MATERIALS;
	::ZeroMemory(m_pMaterials, sizeof(MATERIALS));

	m_pMaterials->m_pReflections[0] = { XMFLOAT4(0.0f, 1.0f, 0.5f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 0.01f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[1] = { XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 10.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[2] = { XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 15.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[3] = { XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 20.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[4] = { XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f), XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 25.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[5] = { XMFLOAT4(0.0f, 0.5f, 0.5f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 30.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[6] = { XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f), XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 35.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[7] = { XMFLOAT4(1.0f, 0.5f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 40.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
}

void CScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);
	BuildLightsAndMaterials();

	BuildTerrain(pd3dDevice, pd3dCommandList);

	m_nShaders = 1;
	m_pShaders = new CObjectsShader[m_nShaders];
	m_pShaders[0].CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pShaders[0].BuildObjects(pd3dDevice, pd3dCommandList, m_pTerrain);
	
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CScene::BuildTerrain(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	XMFLOAT3 xmf3Scale(3.0f, 0.5f, 3.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.2f, 0.0f, 0.0f);

	int mapsizex = 513, mapsizez = 513;

#ifdef _WITH_TERRAIN_PARTITION
	/*하나의 격자 메쉬의 크기는 가로x세로(17x17)이다. 지형 전체는 가로 방향으로 16개, 세로 방향으로 16의 격자 메
	쉬를 가진다. 지형을 구성하는 격자 메쉬의 개수는 총 256(16x16)개가 된다.*/
	m_pTerrain = new CHeightMapTerrain(
		pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, 
		_T("../Assets/Image/Terrain/HeightMap.raw"), 257, 257, 17, 17, xmf3Scale, xmf4Color);
#else
	m_pTerrain = new CHeightMapTerrain(
		pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, 
		_T("Assets/Image/Terrain/Height2.raw"), mapsizex, mapsizez, mapsizex, mapsizez, xmf3Scale, xmf4Color);
#endif

}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();

	if (m_pShaders)
	{
		for (int i = 0; i < m_nShaders; i++) m_pShaders[i].ReleaseShaderVariables();
		for (int i = 0; i < m_nShaders; i++) m_pShaders[i].ReleaseObjects();
		delete[] m_pShaders;
	}

	ReleaseShaderVariables();

	if (m_pTerrain) delete m_pTerrain;
	if (m_pLights) delete m_pLights;
	if (m_pMaterials) delete m_pMaterials;
}



ID3D12RootSignature *CScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;

	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[1];
	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	pd3dDescriptorRanges[0].NumDescriptors = 1;
	pd3dDescriptorRanges[0].BaseShaderRegister = 2; //Game Objects
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = 0;

	D3D12_ROOT_PARAMETER pd3dRootParameters[5];
	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 0; //Player
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[1].Descriptor.ShaderRegister = 1; //Camera
	pd3dRootParameters[1].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[2].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[2].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[0];
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	pd3dRootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[3].Descriptor.ShaderRegister = 3; //Materials
	pd3dRootParameters[3].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[4].Descriptor.ShaderRegister = 4; //Lights
	pd3dRootParameters[4].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}

void CScene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256의 배수
	m_pd3dcbLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);

	UINT ncbMaterialBytes = ((sizeof(MATERIALS) + 255) & ~255); //256의 배수
	m_pd3dcbMaterials = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbMaterialBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbMaterials->Map(0, NULL, (void **)&m_pcbMappedMaterials);
}

void CScene::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	::memcpy(m_pcbMappedLights, m_pLights, sizeof(LIGHTS));
	::memcpy(m_pcbMappedMaterials, m_pMaterials, sizeof(MATERIALS));
}

void CScene::ReleaseShaderVariables()
{
	if (m_pd3dcbLights)
	{
		m_pd3dcbLights->Unmap(0, NULL);
		m_pd3dcbLights->Release();
	}

	if (m_pd3dcbMaterials)
	{
		m_pd3dcbMaterials->Unmap(0, NULL);
		m_pd3dcbMaterials->Release();
	}
}

void CScene::ReleaseUploadBuffers()
{
	if (m_pTerrain) m_pTerrain->ReleaseUploadBuffers();

	for (int i = 0; i < m_nShaders; i++) m_pShaders[i].ReleaseUploadBuffers();
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::ProcessInput(UCHAR *pKeysBuffer)
{
	if (::GetKeyboardState(pKeysBuffer))
	{
		if (pKeysBuffer['P'] & 0xF0) 		m_pPlayer->SetPosition(XMFLOAT3(400, 0, 400));
		if (pKeysBuffer['O'] & 0xF0) 		m_pLights->m_pLights[0].m_bEnable = false;
		if (pKeysBuffer['I'] & 0xF0) 		m_pLights->m_pLights[0].m_bEnable = true;
	}
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	for (int i = 0; i < m_nShaders; i++)
	{
		m_pShaders[i].AnimateObjects(fTimeElapsed);
	}

	if (m_pLights)
	{
		m_pLights->m_pLights[0].m_xmf3Position = m_pPlayer->GetPosition();
		m_pLights->m_pLights[0].m_xmf3Direction = m_pPlayer->GetLookVector();
	}

	if (m_pPlayer->GetPosition().x < 300 && m_pPlayer->GetPosition().z < 300) {
		m_pLights->m_pLights[1].m_bEnable = false;
		m_pLights->m_pLights[2].m_bEnable = false;
		for (int i = 5; i < MAX_LIGHTS; ++i) {
			m_pLights->m_pLights[i].m_bEnable = true;
		}
	}
	else {
		for (int i = 5; i < MAX_LIGHTS; ++i)
			m_pLights->m_pLights[i].m_bEnable = false;

		XMFLOAT3 Pos = m_pShaders[0].getObject(0)->GetPosition();
		XMFLOAT3 Dir = Vector3::Subtract(XMFLOAT3(512.f, 0.f, 512.f), Pos);
		m_pLights->m_pLights[1].m_bEnable = true;
		m_pLights->m_pLights[1].m_xmf3Position = Pos;
		m_pLights->m_pLights[1].m_xmf3Direction = Dir;

		Pos = m_pShaders[0].getObject(1)->GetPosition();
		Dir = Vector3::Subtract(XMFLOAT3(512.f, 0.f, 512.f), Pos);
		m_pLights->m_pLights[2].m_bEnable = true;
		m_pLights->m_pLights[2].m_xmf3Position = Pos;
		m_pLights->m_pLights[2].m_xmf3Direction = Dir;
	}

	int objend = 8 * 8 + 2;
	int objstart = 2;
	//int objend = 2 +2;
	// Wall Collision
	for (int i = objstart; i < objend; i++) {
		CGameObject* tmp = m_pShaders[0].getObject(i);
		XMFLOAT3 pos = tmp->GetPosition();
		if (pos.x < 10 || pos.x > 1275){
			XMFLOAT3 dir = tmp->GetDirection();
			tmp->SetDirection(XMFLOAT3(dir.x * (-1), 0, dir.z));
		}
		if (pos.z < 10 || pos.z > 1275) {
			XMFLOAT3 dir = tmp->GetDirection();
			tmp->SetDirection(XMFLOAT3(dir.x, 0, dir.z * (-1)));
		}
		if (pos.z < 310 && pos.x < 310) {
			XMFLOAT3 dir = tmp->GetDirection();
			if(pos.x>pos.z)
				tmp->SetDirection(XMFLOAT3(dir.x * (-1), 0, dir.z));
			else if(pos.x < pos.z)
				tmp->SetDirection(XMFLOAT3(dir.x, 0, dir.z * (-1)));
		}
	}

	for (int i = objstart; i < objend; i++)
	{
		for (int j = objstart; j < objend; j++)
		{
			if (i != j) {
				CGameObject* Obj1 = m_pShaders[0].getObject(i);
				BoundingOrientedBox ObjOOBB1 = Obj1->GetBoundingBox();
				CGameObject* Obj2 = m_pShaders[0].getObject(j);
				BoundingOrientedBox ObjOOBB2 = Obj2->GetBoundingBox();

				if (Obj1->GetCollider() == NULL || Obj1->GetCollider() == Obj2){
					if (ObjOOBB1.Intersects(ObjOOBB2)) {
						Obj1->SetCollider(Obj2);
						Obj2->SetCollider(Obj1);
					}
				}
			}
		}
	}

	for (int i = objstart; i < objend; i++)
	{
		CGameObject* Obj = m_pShaders[0].getObject(i);
		if (Obj->GetCollider() && Obj->GetCollider()->GetCollider())
		{
			CGameObject* Collider = Obj->GetCollider();
			XMFLOAT3 xmf3ObjPos = Obj->GetPosition();
			XMFLOAT3 xmf3ColliderPos = Collider->GetPosition();

			XMFLOAT3 direct = Vector3::Normalize(Vector3::Add(xmf3ObjPos, xmf3ColliderPos, -1.0f));
			XMFLOAT3 reflect;
			XMStoreFloat3(&reflect, XMVector3Reflect(XMLoadFloat3(&Obj->GetDirection()), XMLoadFloat3(&direct)));
			Obj->SetDirection(reflect);

			direct = Vector3::Normalize(Vector3::Add(xmf3ColliderPos, xmf3ObjPos, -1.0f));
			XMStoreFloat3(&reflect, XMVector3Reflect(XMLoadFloat3(&Collider->GetDirection()), XMLoadFloat3(&direct)));
			Collider->SetDirection(reflect);

			Obj->SetCollider(NULL);
			Collider->SetCollider(NULL);
		}
	}
}


void CScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(4, d3dcbLightsGpuVirtualAddress); //Lights

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbMaterialsGpuVirtualAddress = m_pd3dcbMaterials->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(3, d3dcbMaterialsGpuVirtualAddress);

	if (m_pTerrain) m_pTerrain->Render(pd3dCommandList, pCamera);

	for (int i = 0; i < m_nShaders; i++)
	{
		m_pShaders[i].Render(pd3dCommandList, pCamera);
	}
}

