//-----------------------------------------------------------------------------
// File: CGameObject.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Object.h"
#include "Shader.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CMaterial::CMaterial()
{
	m_xmf4Albedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

CMaterial::~CMaterial()
{
	if (m_pShader)
	{
		m_pShader->ReleaseShaderVariables();
		m_pShader->Release();
	}
}

void CMaterial::SetShader(CShader *pShader)
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CGameObject::CGameObject(int nMeshes)
{
	m_xmf4x4World = Matrix4x4::Identity();
	m_nMeshes = nMeshes;
	m_ppMeshes = NULL;
	m_Velocity = XMFLOAT3(0, 0, 0);
	m_d3dCbvGPUDescriptorHandle.ptr = NULL;

	if (m_nMeshes > 0)
	{
		m_ppMeshes = new CMesh*[m_nMeshes];
		for (int i = 0; i < m_nMeshes; i++) m_ppMeshes[i] = NULL;
	}
}

CGameObject::~CGameObject()
{
	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i]) m_ppMeshes[i]->Release();
			m_ppMeshes[i] = NULL;
		}
		delete[] m_ppMeshes;
	}

	if (m_pMaterial) m_pMaterial->Release();
}

void CGameObject::SetMesh(int nIndex, CMesh *pMesh)
{
	if (m_ppMeshes)
	{
		if (m_ppMeshes[nIndex]) m_ppMeshes[nIndex]->Release();
		m_ppMeshes[nIndex] = pMesh;
		if (pMesh) pMesh->AddRef();
	}
}

void CGameObject::SetShader(CShader *pShader)
{
	if (!m_pMaterial)
	{
		m_pMaterial = new CMaterial();
		m_pMaterial->AddRef();
	}
	if (m_pMaterial) m_pMaterial->SetShader(pShader);
}

void CGameObject::SetMaterial(CMaterial *pMaterial)
{
	if (m_pMaterial) m_pMaterial->Release();
	m_pMaterial = pMaterial;
	if (m_pMaterial) m_pMaterial->AddRef();
}

void CGameObject::SetMaterial(UINT nReflection)
{
	if (!m_pMaterial) m_pMaterial = new CMaterial();
	m_pMaterial->m_nReflection = nReflection;
}

void CGameObject::Animate(float fTimeElapsed)
{
}

void CGameObject::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	OnPrepareRender();
	if (IsVisible(pCamera)) {
		if (m_pMaterial)
		{
			if (m_pMaterial->m_pShader)
			{
				m_pMaterial->m_pShader->Render(pd3dCommandList, pCamera);
				m_pMaterial->m_pShader->UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);
			}
		}

		if (m_d3dCbvGPUDescriptorHandle.ptr)
			pd3dCommandList->SetGraphicsRootDescriptorTable(2, m_d3dCbvGPUDescriptorHandle);

		if (m_ppMeshes)
		{
			for (int i = 0; i < m_nMeshes; i++)
			{
				if (m_ppMeshes[i]) m_ppMeshes[i]->Render(pd3dCommandList);
			}
		}
	}
}

void CGameObject::ReleaseUploadBuffers()
{
	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i]) m_ppMeshes[i]->ReleaseUploadBuffers();
		}
	}
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4World._41 = x;
	m_xmf4x4World._42 = y;
	m_xmf4x4World._43 = z;
}

void CGameObject::SetPosition(XMFLOAT3 xmf3Position)
{
	SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
}

XMFLOAT3 CGameObject::GetPosition()
{
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43));
}

XMFLOAT3 CGameObject::GetLook()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33)));
}

XMFLOAT3 CGameObject::GetUp()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23)));
}

XMFLOAT3 CGameObject::GetRight()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13)));
}

void CGameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Right, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Up, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Look = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Look, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Rotate(XMFLOAT3 *pxmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis), XMConvertToRadians(fAngle));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

bool CGameObject::IsVisible(CCamera * pCamera)
{
	OnPrepareRender();
	bool bIsVisible = false;
	BoundingOrientedBox xmBoundingBox = m_ppMeshes[0]->GetBoundingBox();

	//모델 좌표계의 바운딩 박스를 월드 좌표계로 변환한다.
	xmBoundingBox.Transform(xmBoundingBox, XMLoadFloat4x4(&m_xmf4x4World));
	if (pCamera) bIsVisible = pCamera->IsInFrustum(xmBoundingBox);
	return(bIsVisible);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CRotatingObject::CRotatingObject(int nMeshes) : CGameObject(nMeshes)
{
	m_xmf3RotationAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_fRotationSpeed = 15.0f;
}

CRotatingObject::~CRotatingObject()
{
}

void CRotatingObject::Animate(float fTimeElapsed)
{
	XMFLOAT3 pos = GetPosition();
	SetPosition(pos.x+m_Velocity.x, m_terrain->GetHeight(pos.x, pos.z) + 10, pos.z+m_Velocity.z);
	CGameObject::Rotate(&m_xmf3RotationAxis, m_fRotationSpeed * fTimeElapsed);
}

void CRotatingObject::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	CGameObject::Render(pd3dCommandList, pCamera);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CRevolvingObject::CRevolvingObject(int nMeshes) : CGameObject(nMeshes)
{
	m_xmf3RevolutionAxis = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_fRevolutionSpeed = 45.f;
	m_OrbitRaidus = 512.f;
	m_CenterPos = XMFLOAT3(256.f, 0.f, 256.f);
}

CRevolvingObject::~CRevolvingObject()
{
}

void CRevolvingObject::Animate(float fTimeElapsed)
{
	/*
	공전하는 행성을 맵의 중심으로 옮긴다.
	해당 행성이 얼마나 돌았는지 각도를 speed와 Time elapsed를 통해서 구하고
	행성에 주어진 Radius에 따라서 공전좌표를 구한다.
	중심좌표에 공전좌표를 더하여 SetPostion한다.
	*/

	m_OrbitAngle += XMConvertToRadians(m_fRevolutionSpeed * fTimeElapsed);
	if (m_OrbitAngle > 6.24) m_OrbitAngle -= 6.24;

	m_OrbitPos = XMFLOAT3(cos(m_OrbitAngle)*m_OrbitRaidus, sin(m_OrbitAngle)*m_OrbitRaidus, 0.f);
	SetPosition(Vector3::Add(m_CenterPos, m_OrbitPos));
}

void CRevolvingObject::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
	CGameObject::Render(pd3dCommandList, pCamera);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//

CHeightMapTerrain::CHeightMapTerrain(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
	*pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, LPCTSTR pFileName, 
	int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color)
{
	//지형에 사용할 높이 맵의 가로, 세로의 크기이다.
	m_nWidth = nWidth;
	m_nLength = nLength;
	
	/*지형 객체는 격자 메쉬들의 배열로 만들 것이다. nBlockWidth, nBlockLength는 격자 메쉬 하나의 가로, 세로 크
	기이다. cxQuadsPerBlock, czQuadsPerBlock은 격자 메쉬의 가로 방향과 세로 방향 사각형의 개수이다.*/
	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;

	//xmf3Scale는 지형을 실제로 몇 배 확대할 것인가를 나타낸다.
	m_xmf3Scale = xmf3Scale;

	//지형에 사용할 높이 맵을 생성한다.
	m_pHeightMapImage = new CHeightMapImage(pFileName, nWidth, nLength, xmf3Scale);

	//지형에서 가로 방향, 세로 방향으로 격자 메쉬가 몇 개가 있는 가를 나타낸다.
	long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	long czBlocks = (m_nLength - 1) / czQuadsPerBlock;

	//지형 전체를 표현하기 위한 격자 메쉬의 개수이다.
	m_nMeshes = cxBlocks * czBlocks;

	//지형 전체를 표현하기 위한 격자 메쉬에 대한 포인터 배열을 생성한다.
	m_ppMeshes = new CMesh*[m_nMeshes];
	for (int i = 0; i < m_nMeshes; i++) m_ppMeshes[i] = NULL;
	CHeightMapGridMesh *pHeightMapGridMesh = NULL;
	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			//지형의 일부분을 나타내는 격자 메쉬의 시작 위치(좌표)이다.
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);
			//지형의 일부분을 나타내는 격자 메쉬를 생성하여 지형 메쉬에 저장한다.
			pHeightMapGridMesh = new CHeightMapGridMesh(pd3dDevice, pd3dCommandList, xStart,
				zStart, nBlockWidth, nBlockLength, xmf3Scale, xmf4Color, m_pHeightMapImage);
			SetMesh(x + (z*cxBlocks), pHeightMapGridMesh);
		}
	}

	//지형을 렌더링하기 위한 셰이더를 생성한다.
	CTerrainShader *pShader = new CTerrainShader();
	pShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	pShader->BuildObjects(pd3dDevice, pd3dCommandList, this);
	SetShader(pShader);

	SetCbvGPUDescriptorHandle(pShader->GetGPUDescriptorHandleForHeapStart().ptr);
}

CHeightMapTerrain::~CHeightMapTerrain(void)
{
	if (m_pHeightMapImage) delete m_pHeightMapImage;
}


